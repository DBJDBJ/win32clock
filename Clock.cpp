// Clock.cpp : Defines the entry point for the application.
// @author : x0r

#include "stdafx.h"
#if DBJ_USE_SOUNDS
#include "SoundPlayer.h"
#endif
#include "Clock.h"

// Global Variables:
HINSTANCE hInst;
HWND hApp;
#if DBJ_USE_SOUNDS
SoundPlayer *tick, *tock;
#endif
static TCHAR szTitle[] = TEXT("Clock");
static TCHAR szWindowClass[] = TEXT("[dbj.systems-win32.clock]");
static TCHAR buf[100] = {} ;
static TCHAR szUserName[100] = {} ;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
/*
int GetWelcomeMessage();
void Line(HDC, int, int, int, int, int, int);
*/

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

    HCURSOR hCur = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
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

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_DROPSHADOW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
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
	using namespace dbj;
	/*int*/ LONG radius; 
	UINT sy = 0, sx = 0;
    // UINT uState;
    static POINT del;
    static LONG pStyle;
    PAINTSTRUCT ps;
    HDC hdc;
    HGDIOBJ obj;
    static HMENU hMenu;
#if DBJ_USE_SOUNDS
    static BOOL bSound = FALSE;
#endif
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
#if DBJ_USE_SOUNDS
			case IDM_SOUND:
				bSound = ~bSound;
				break;
#endif
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
#if DBJ_USE_SOUNDS
			AppendMenu(hMenu, MF_STRING | MF_CHECKED, IDM_SOUND, TEXT("Enable &Sound"));
			CheckMenuItem(hMenu, IDM_SOUND, bSound);
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
#endif
			AppendMenu(hMenu, MF_STRING | MF_CHECKED, IDM_TOPMOST, TEXT("&Always On Top"));
			CheckMenuItem(hMenu, IDM_TOPMOST, bTopmost);
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(hMenu, MF_STRING, IDM_QUIT, TEXT("E&xit"));
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
			ClientToScreen(hWnd, &pt);
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
		} break;
		case WM_PAINT: {
			SYSTEMTIME localTime;
			GetLocalTime(&localTime);

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
			_stprintf(buf, TEXT("%02i:%02i:%02i"), localTime.wHour, localTime.wMinute, localTime.wSecond);
			SIZE_T len = _tcslen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx / 2, min(WIDTH(r), HEIGHT(r)) / 4, buf, len);

			ZeroMemory(buf, sizeof buf);
			if (HEIGHT(r) >= 300 && WIDTH(r) >= 300) {
				_stprintf(buf, TEXT("Hii %s, %s !"), szUserName, dbj::GetWelcomeMessage());
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
				_stprintf(buf, TEXT("%.3s"), days(localTime.wDayOfWeek));
			}
			else {
				_tcscpy(buf, days(localTime.wDayOfWeek));
			}
			len = lstrlen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx / 2, -min(WIDTH(r), HEIGHT(r)) / 4, buf, len);

			// ... display date ...

			RtlZeroMemory(buf, sizeof buf);
			if (HEIGHT(r) < 300 || WIDTH(r) < 300) {
				_stprintf(buf, TEXT("%02d-%.3s-%d"), localTime.wDay, months(localTime.wMonth), localTime.wYear);
			}
			else {
				_stprintf(buf, TEXT("%s %02d, %d"), months(localTime.wMonth), localTime.wDay, localTime.wYear);
			}
			len = _tcsclen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx / 2, -min(WIDTH(r), HEIGHT(r)) / 8, buf, len);

			// draw points ...

			radius = min(WIDTH(r) / 2, HEIGHT(r) / 2) - 10;
			for (int theta = 6; theta <= 360; theta += 6) {
				sx = (UINT)(radius * sin(theta * RADIAN));
				sy = (UINT)(radius * cos(theta * RADIAN));
				if (theta % 30 == 0) {
					Ellipse(hdc, sx - 2 * POINT_DIAMETER, sy - 2 * POINT_DIAMETER, sx + 2 * POINT_DIAMETER, sy + 2 * POINT_DIAMETER);
					//if (theta % 45 == 0) {
					RtlZeroMemory(buf, sizeof buf);
					_stprintf(buf, TEXT("%i"), theta / 30);
					SIZE txtW;
					GetTextExtentPoint32(hdc, buf, _tcsclen(buf), &txtW);
					SetTextAlign(hdc, TA_BASELINE | TA_CENTER);
					sx = (UINT)((radius - 25) * sin(theta * RADIAN));
					sy = (UINT)((radius - 25) * cos(theta * RADIAN));
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
			pSec.y = (LONG)(radius * cos(localTime.wSecond * 6 * RADIAN));
			pSec.x = (LONG)(radius * sin(localTime.wSecond * 6 * RADIAN));
			radius -= 6 * POINT_DIAMETER;
			pMin.y = (LONG)(radius * cos(localTime.wMinute * 6 * RADIAN));
			pMin.x = (LONG)(radius * sin(localTime.wMinute * 6 * RADIAN));
			radius -= 8 * POINT_DIAMETER;

			pHour.y = (LONG)(radius * cos(localTime.wHour * 30 * RADIAN));
			pHour.x = (LONG)(radius * sin(localTime.wHour * 30 * RADIAN));

			// ... draw hands ...
			Line(hdc, 0, 0, pHour.x, pHour.y, Gdiplus::Color::DarkGray, HOUR_WIDTH);
			Line(hdc, 0, 0, pMin.x, pMin.y,   Gdiplus::Color::DarkGray, MINUTE_WIDTH);
			Line(hdc, 0, 0, pSec.x, pSec.y,   Gdiplus::Color::Black, SECOND_WIDTH);

			obj = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Ellipse(hdc, -POINT_DIAMETER * 2, -POINT_DIAMETER * 2, POINT_DIAMETER * 2, POINT_DIAMETER * 2);
			DeleteObject(GetStockObject(BLACK_BRUSH));

			SelectObject(hdc, obj);
#if DBJ_USE_SOUNDS
			// ... play sound ...
			if (bSound & !fMoving) {
				if (localTime.wSecond & 1) {
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
#endif
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

