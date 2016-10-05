// Clock.cpp : Defines the entry point for the application.
// @author : x0r

#include "stdafx.h"
// #include "SoundPlayer.h"
#include "Clock.h"

// Global Variables:
HINSTANCE hInst;
HWND hApp;
// SoundPlayer *tick, *tock;
TCHAR szTitle[] = TEXT("Clock");
TCHAR szWindowClass[] = TEXT("[x0r-Cl0ck]");
TCHAR buf[100];
TCHAR szUserName[100];
TCHAR* szWelcome[] = { TEXT("Good Morning"), TEXT("Good Afternoon"), TEXT("Good Evening"), TEXT("Good Night") };
TCHAR* days[] = { NULL, TEXT("Sunday"), TEXT("Monday"), TEXT("Tuesday"), TEXT("Wednesday"), TEXT("Thursday"), TEXT("Friday"), TEXT("Saturday") };
TCHAR* months[] = {
    NULL, TEXT("January"), TEXT("February"), TEXT("March"), TEXT("April"), TEXT("May"), TEXT("June"),
    TEXT("July"), TEXT("August"), TEXT("September"), TEXT("October"), TEXT("November"), TEXT("December")
};

// Forward declarations of functions included in this code module:
int GetWelcomeMessage();
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Line(HDC, int, int, int, int);

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HWND hWin = FindWindow(TEXT("[x0r-Cl0ck]"), TEXT("Clock"));
    if (hWin != NULL) {
        SetForegroundWindow(hWin);
        return 0;
    }

    // TODO: Place code here.
    MSG msg;

    // Initialize global strings
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    //tick = new SoundPlayer(IDR_TICK);
    //tock = new SoundPlayer(IDR_TOCK);

    DWORD dwSize = 100;
    GetUserName(szUserName, &dwSize);
    UpdateWindow(hApp);

    HCURSOR hCur = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR3));
    SetProp(hApp, TEXT("CURSOR-3"), hCur);

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_DROPSHADOW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLOCK));
    wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = NULL; //LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    int width = GetSystemMetrics(SM_CXSCREEN), height = GetSystemMetrics(SM_CYSCREEN);

    hApp = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, szWindowClass, szTitle, 0, width / 2 - 200, height / 2 - 200, 400, 400, NULL, NULL, hInstance, NULL);

    int dwFlags = WS_BORDER | WS_VISIBLE | WS_SIZEBOX;

    SetWindowLong(hApp, GWL_STYLE, dwFlags);
    SetWindowPos(hApp, NULL, width - 420, 20, 400, 400, SWP_FRAMECHANGED);

    if (!hApp) {
        return FALSE;
    }

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent, radius, sy, sx;
    UINT uState;
    static POINT del;
    static LONG pStyle;
    PAINTSTRUCT ps;
    HDC hdc;
    HGDIOBJ obj;
    static HMENU hMenu;
    static BOOL bSound = FALSE;
    static BOOL fMoving = FALSE;
    static BOOL fCircle = FALSE;
    static BOOL bTopmost = FALSE;
    static BOOL fResized = FALSE;
    RECT r;

    switch (message) {
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
			case IDM_QUIT:
				DestroyWindow(hApp);
				break;
			case IDM_SOUND:
				bSound = ~bSound;
				break;
			case IDM_CIRCLE: {
				fCircle = ~fCircle;
				if (fCircle) {
					SetWindowLong(hApp, GWL_STYLE, WS_BORDER | WS_VISIBLE);
					GetWindowRect(hApp, &r);
					DWORD dwDiam = min(WIDTH(r), HEIGHT(r));
					HRGN hEll = CreateEllipticRgn(0, 0, dwDiam, dwDiam);
					SetWindowRgn(hWnd, hEll, TRUE);
					SetWindowPos(hWnd, NULL, r.left, r.top, dwDiam, dwDiam, SWP_FRAMECHANGED);
				}
				else {
					SetWindowLong(hApp, GWL_STYLE, WS_BORDER | WS_VISIBLE | WS_SIZEBOX);
					GetWindowRect(hApp, &r);
					HRGN hEll = CreateRectRgn(0, 0, r.right - r.left, r.bottom - r.top);
					SetWindowRgn(hWnd, hEll, TRUE);
					SetWindowPos(hWnd, NULL, r.left, r.top, WIDTH(r), HEIGHT(r), SWP_FRAMECHANGED);
					InvalidateRect(hApp, NULL, TRUE);
				}
			} break;
			case IDM_TOPMOST: {
				GetWindowRect(hApp, &r);
				bTopmost = ~bTopmost;
				SetWindowPos(hWnd, bTopmost ? HWND_TOPMOST : HWND_NOTOPMOST, r.left, r.top, WIDTH(r), HEIGHT(r), SWP_FRAMECHANGED);
			} break;
			}
		} break;
		case WM_CREATE: {
			SetLayeredWindowAttributes(hWnd, 0x00ffffff, OPACITY, LWA_ALPHA);
			SetTimer(hWnd, 1, 1000, NULL);
		} break;
		case WM_LBUTTONDOWN: {
			SetCapture(hApp);
			GetCursorPos(&del);
			ScreenToClient(hApp, &del);
			pStyle = GetWindowLong(hApp, GWL_EXSTYLE);
		} break;
		case WM_LBUTTONUP:
			if (GetCapture() == hApp) {
				SetLayeredWindowAttributes(hApp, 0x00ffffff, OPACITY - FACTOR, LWA_ALPHA);
				del.x = del.y = 0;
				fMoving = FALSE;
				SetWindowLong(hApp, GWL_EXSTYLE, pStyle);
				SetLayeredWindowAttributes(hApp, 0x00ffffff, OPACITY, LWA_ALPHA);
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				ReleaseCapture();
			}
			break;
		case WM_MOUSEMOVE:
			if (wParam & MK_LBUTTON) {
				RECT r;
				SetLayeredWindowAttributes(hApp, 0x00ffffff, OPACITY - FACTOR, LWA_ALPHA);
				HCURSOR hCur = (HCURSOR)GetProp(hApp, TEXT("CURSOR-3"));
				if (GetCursor() != hCur) {
					SetCursor(hCur);
				}
				fMoving = TRUE;
				GetWindowRect(hWnd, &r);
				POINT pt = { 0 };
				GetCursorPos(&pt);
				MoveWindow(hWnd, pt.x - del.x, pt.y - del.y, r.right - r.left, r.bottom - r.top, TRUE);
			}
			break;
		case WM_RBUTTONUP: {
			hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING | MF_CHECKED, IDM_CIRCLE, TEXT("&Circle Clock"));
			CheckMenuItem(hMenu, IDM_CIRCLE, fCircle);
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(hMenu, MF_STRING | MF_CHECKED, IDM_SOUND, TEXT("Enable &Sound"));
			CheckMenuItem(hMenu, IDM_SOUND, bSound);
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(hMenu, MF_STRING | MF_CHECKED, IDM_TOPMOST, TEXT("&Always On Top"));
			CheckMenuItem(hMenu, IDM_TOPMOST, bTopmost);
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(hMenu, MF_STRING, IDM_QUIT, TEXT("E&xit"));
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
			ClientToScreen(hWnd, &pt);
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
		} break;
		case WM_PAINT: {
			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);

			hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);

			// ... set mapping mode ...

			GetClientRect(hWnd, &r);
			SetMapMode(hdc, MM_ISOTROPIC);
			SetViewportOrgEx(hdc, WIDTH(r) / 2, HEIGHT(r) / 2, NULL);
			SetWindowExtEx(hdc, WIDTH(r) / 2, -HEIGHT(r) / 2, NULL);
			SetViewportExtEx(hdc, WIDTH(r) / 2, HEIGHT(r) / 2, NULL);

			LOGFONT lf;
			ZeroMemory(&lf, sizeof lf);
			lf.lfHeight = min(WIDTH(r), HEIGHT(r)) >> 4;
			_tcscpy(lf.lfFaceName, TEXT("Comic Sans MS"));
			SelectObject(hdc, CreateFontIndirect(&lf));

			// ... display time ...

			SIZE tz;
			ZeroMemory(buf, sizeof buf);
			_stprintf(buf, TEXT("%02i:%02i:%02i"), sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			SIZE_T len = _tcslen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx / 2, min(WIDTH(r), HEIGHT(r)) / 4, buf, len);

			ZeroMemory(buf, sizeof buf);
			if (HEIGHT(r) >= 300 && WIDTH(r) >= 300) {
				_stprintf(buf, TEXT("Hii %s, %s !"), szUserName, szWelcome[GetWelcomeMessage()]);
			}
			else {
				_stprintf(buf, TEXT("Hii %s"), szUserName);
			}
			len = _tcslen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx / 2, min(WIDTH(r), HEIGHT(r)) / 8, buf, len);
			// ... display day ...

			ZeroMemory(buf, sizeof buf);
			if (HEIGHT(r) < 300 || WIDTH(r) < 300) {
				_stprintf(buf, TEXT("%.3s"), days[sysTime.wDayOfWeek]);
			}
			else {
				_tcscpy(buf, days[sysTime.wDayOfWeek]);
			}
			len = lstrlen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx / 2, -min(WIDTH(r), HEIGHT(r)) / 4, buf, len);

			// ... display date ...

			RtlZeroMemory(buf, sizeof buf);
			if (HEIGHT(r) < 300 || WIDTH(r) < 300) {
				_stprintf(buf, TEXT("%02d-%.3s-%d"), sysTime.wDay, months[sysTime.wMonth], sysTime.wYear);
			}
			else {
				_stprintf(buf, TEXT("%s %02d, %d"), months[sysTime.wMonth], sysTime.wDay, sysTime.wYear);
			}
			len = _tcsclen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx / 2, -min(WIDTH(r), HEIGHT(r)) / 8, buf, len);

			// draw points ...

			radius = min(WIDTH(r) / 2, HEIGHT(r) / 2) - 10;
			for (int theta = 6; theta <= 360; theta += 6) {
				sx = (int)radius * sin(theta * RADIAN);
				sy = (int)radius * cos(theta * RADIAN);
				if (theta % 30 == 0) {
					Ellipse(hdc, sx - 2 * POINT_DIAMETER, sy - 2 * POINT_DIAMETER, sx + 2 * POINT_DIAMETER, sy + 2 * POINT_DIAMETER);
					//if (theta % 45 == 0) {
					RtlZeroMemory(buf, sizeof buf);
					_stprintf(buf, TEXT("%i"), theta / 30);
					SIZE txtW;
					GetTextExtentPoint32(hdc, buf, _tcsclen(buf), &txtW);
					SetTextAlign(hdc, TA_BASELINE | TA_CENTER);
					sx = (int)(radius - 25) * sin(theta * RADIAN);
					sy = (int)(radius - 25) * cos(theta * RADIAN);
					TextOut(hdc, sx - txtW.cx / 2, sy - txtW.cy / 2, buf, _tcsclen(buf));
					//}
				}
				else {
					obj = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
					Ellipse(hdc, sx - POINT_DIAMETER, sy - POINT_DIAMETER, sx + POINT_DIAMETER, sy + POINT_DIAMETER);
					DeleteObject(GetStockObject(BLACK_BRUSH));
				}
			}
			DeleteObject(GetStockObject(SYSTEM_FIXED_FONT));

			POINT pMin, pSec, pHour;
			radius -= 4 * POINT_DIAMETER;
			pSec.y = (LONG)radius * cos(sysTime.wSecond * 6 * RADIAN);
			pSec.x = (LONG)radius * sin(sysTime.wSecond * 6 * RADIAN);
			radius -= 6 * POINT_DIAMETER;
			pMin.y = (LONG)radius * cos(sysTime.wMinute * 6 * RADIAN);
			pMin.x = (LONG)radius * sin(sysTime.wMinute * 6 * RADIAN);
			radius -= 8 * POINT_DIAMETER;
			pHour.y = (LONG)radius * cos(sysTime.wHour * 30 * RADIAN);
			pHour.x = (LONG)radius * sin(sysTime.wHour * 30 * RADIAN);

			HPEN hSPen = CreatePen(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, SECOND_WIDTH, 0x003686f7);
			HPEN hMPen = CreatePen(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, MINUTE_WIDTH, 0x003ec182);
			HPEN hHPen = CreatePen(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, HOUR_WIDTH, 0x00f8bc0c);

			// ... draw hands ...

			HGDIOBJ hObj = SelectObject(hdc, hHPen);
			Line(hdc, 0, 0, pHour.x, pHour.y);
			DeleteObject(hHPen);

			SelectObject(hdc, hMPen);
			Line(hdc, 0, 0, pMin.x, pMin.y);
			DeleteObject(hMPen);

			SelectObject(hdc, hSPen);
			Line(hdc, 0, 0, pSec.x, pSec.y);
			//MakeArrow(hdc, 0, 0, 20, (int) _hypot(pSec.x, pSec.y), sysTime.wHour*30, _hypot(pSec.x, pSec.y));
			DeleteObject(hSPen);

			obj = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Ellipse(hdc, -POINT_DIAMETER * 2, -POINT_DIAMETER * 2, POINT_DIAMETER * 2, POINT_DIAMETER * 2);
			DeleteObject(GetStockObject(BLACK_BRUSH));

			SelectObject(hdc, obj);
			// ... play sound ...
			if (bSound & !fMoving) {
				if (sysTime.wSecond & 1) {
					PlaySound(MAKEINTRESOURCE(IDR_TOCK), hInst, SND_SYNC | SND_RESOURCE);
					//tock->Stop();
					//tick->Play();
				}
				else {
					PlaySound(MAKEINTRESOURCE(IDR_TICK), hInst, SND_SYNC | SND_RESOURCE);
					//tick->Stop();
					//tock->Play();
				}
			}

			EndPaint(hWnd, &ps);
		} break;

		case WM_TIMER:
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		
		case WM_SIZE: {
			if (~fCircle && fResized) {
				SetWindowLong(hApp, GWL_STYLE, WS_BORDER | WS_VISIBLE | WS_SIZEBOX);
				GetWindowRect(hApp, &r);
				HRGN hEll = CreateRectRgn(0, 0, r.right - r.left, r.bottom - r.top);
				SetWindowRgn(hWnd, hEll, TRUE);
				SetWindowPos(hWnd, NULL, r.left, r.top, WIDTH(r), HEIGHT(r), SWP_FRAMECHANGED);
			}
			InvalidateRect(hApp, NULL, TRUE);
			fResized = TRUE;
		} break;

		case WM_DESTROY:
			KillTimer(hWnd, 1);
			PostMessage(hWnd, WM_QUIT, 0, 0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Line(HDC hdc, int sx, int sy, int ex, int ey)
{
    int width = 20;
    int dx = 0;
    POINT pt[] = { sx, sy, ex, ey, ex - dx, ey - dx, ex + dx, ey + dx, ex, ey, sx, sy };
    Polyline(hdc, pt, 6);
}

int GetWelcomeMessage()
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    if (time.wHour >= 4 && time.wHour < 12) {
        return 0;
    }
    else if (time.wHour >= 12 && time.wHour < 16) {
        return 1;
    }
    else if (time.wHour >= 16 && time.wHour < 20) {
        return 2;
    }
    else {
        return 3;
    }
}
