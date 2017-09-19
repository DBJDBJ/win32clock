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
constexpr static TCHAR szTitle[] = TEXT("Clock");
constexpr static TCHAR szWindowClass[] = TEXT("[dbj.systems-win32.clock]");
constexpr static TCHAR buf[100] = {} ;
// constexpr static TCHAR szUserName[100] = {} ;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	DBJ_TRACE("%s", __argv[0]);

    HWND hWin = FindWindow(TEXT("[dbj.systems-win32.clock]"), TEXT("Clock"));
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
	auto user_name = dbj::sysinfo::user_name(); //  ::GetUserNameA(szUserName, &dwSize);
    UpdateWindow(hApp);
	/*
    HCURSOR hCur = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
    SetProp(hApp, TEXT("CURSOR-3"), hCur);
	*/
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

    int width = GetSystemMetrics(SM_CXSCREEN), 
		height = GetSystemMetrics(SM_CYSCREEN);

    hApp = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, 
		szWindowClass, szTitle, 0, width / 2 - 200, height / 2 - 200, 400, 400, NULL, NULL, hInstance, NULL);

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
	/* LONG radius; */
	UINT sy = 0, sx = 0;
    // UINT uState;
    static POINT del;
    static LONG pStyle;
    PAINTSTRUCT ps;
//    HDC hdc;
//  c obj;
//    static HMENU hMenu;
#if DBJ_USE_SOUNDS
    static BOOL bSound = FALSE;
#endif
    static BOOL fMoving = FALSE;
    static BOOL fCircle = FALSE;
    static BOOL bTopmost = FALSE;
    static BOOL fResized = FALSE;

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
				RECT r;
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
				RECT rectangle = {};
				GetWindowRect(hApp, &rectangle);
				bTopmost = ~bTopmost;
				SetWindowPos(hWnd, bTopmost ? HWND_TOPMOST : HWND_NOTOPMOST, rectangle.left, rectangle.top, WIDTH(rectangle), HEIGHT(rectangle), SWP_FRAMECHANGED);
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
			auto hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING | MF_CHECKED, IDM_CIRCLE, TEXT("&Circle Clock"));
			CheckMenuItem(hMenu, IDM_CIRCLE, fCircle);
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
			RECT rectangle = {} ;
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			auto hdc = BeginPaint(hWnd, &ps);
			DBJ_ASSERT(hdc);
			SetBkMode(hdc, TRANSPARENT);

			// ... set mapping mode ...

			GetClientRect(hWnd, &rectangle);

			auto half_width = WIDTH(rectangle) / 2;
			auto half_height = HEIGHT(rectangle) / 2;

			SetMapMode(hdc, MM_ISOTROPIC);
			SetViewportOrgEx(hdc, half_width, half_height, NULL);
			SetWindowExtEx(hdc, half_width, -half_height, NULL);
			SetViewportExtEx(hdc, half_width, half_height, NULL);


			// ... display time ...
			auto display_time = [&localTime, hdc, rectangle]() {
				SIZE tz;
				TCHAR buf[BUFSIZ] = {};
				SecureZeroMemory(buf, sizeof buf);
				_stprintf(buf, TEXT("%02i:%02i:%02i"), localTime.wHour, localTime.wMinute, localTime.wSecond);
				SIZE_T len = _tcslen(buf);
				GetTextExtentPoint32(hdc, buf, len, &tz);
				TextOut(hdc, -tz.cx / 2, min(WIDTH(rectangle), HEIGHT(rectangle)) / 4, buf, len);

				SecureZeroMemory(buf, sizeof buf);
				if (HEIGHT(rectangle) >= 300 && WIDTH(rectangle) >= 300) {
					_stprintf(buf, TEXT("Hi %s, %s !"), dbj::sysinfo::user_name().data(), dbj::GetWelcomeMessage());
				}
				else {
					_stprintf(buf, TEXT("Hi %s"), dbj::sysinfo::user_name().data());
				}
				len = _tcslen(buf);
				GetTextExtentPoint32(hdc, buf, len, &tz);
				TextOut(hdc, -tz.cx / 2, min(WIDTH(rectangle), HEIGHT(rectangle)) / 8, buf, len);
			};
			// ... display day ...
			auto display_day = [&localTime, hdc, rectangle]() {
				SIZE tz = {} ;
				TCHAR buf[BUFSIZ] = {};
				SecureZeroMemory(buf, sizeof buf);
				if (HEIGHT(rectangle) < 300 || WIDTH(rectangle) < 300) {
					_stprintf(buf, TEXT("%.3s"), days(localTime));
				}
				else {
					_tcscpy(buf, days(localTime));
				}
				auto len = lstrlen(buf);
				GetTextExtentPoint32(hdc, buf, len, &tz);
				TextOut(hdc, -tz.cx / 2, -min(WIDTH(rectangle), HEIGHT(rectangle)) / 4, buf, len);
			};
			// ... display date ...
			auto display_date = [&localTime, hdc, rectangle]() {
				SIZE tz = {};
				TCHAR buf[BUFSIZ] = {};

				SecureZeroMemory(buf, sizeof buf);
				if (HEIGHT(rectangle) < 300 || WIDTH(rectangle) < 300) {
					_stprintf(buf, TEXT("%02d-%.3s-%d"), localTime.wDay, months(localTime), localTime.wYear);
				}
				else {
					_stprintf(buf, TEXT("%s %02d, %d"), months(localTime), localTime.wDay, localTime.wYear);
				}
				auto len = _tcsclen(buf);
				GetTextExtentPoint32(hdc, buf, len, &tz);
				TextOut(hdc, -tz.cx / 2, -min(WIDTH(rectangle), HEIGHT(rectangle)) / 8, buf, len);
			};
			// draw points ...
			auto draw_points = [&localTime, hdc, rectangle]() {
				SIZE tz = {};
				TCHAR buf[BUFSIZ] = {};

				LONG radius = min(WIDTH(rectangle) / 2, HEIGHT(rectangle) / 2) - 10;
				for (int theta = 6; theta <= 360; theta += 6) {
					auto sx = (UINT)(radius * sin(theta * RADIAN));
					auto sy = (UINT)(radius * cos(theta * RADIAN));
					if (theta % 30 == 0) {
						Ellipse(hdc, sx - 2 * POINT_DIAMETER, sy - 2 * POINT_DIAMETER, sx + 2 * POINT_DIAMETER, sy + 2 * POINT_DIAMETER);
						//if (theta % 45 == 0) {
						SecureZeroMemory(buf, sizeof buf);
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
						auto obj = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
						DBJ_ASSERT(obj != NULL);
						DBJ_ASSERT(obj != HGDI_ERROR);
						Ellipse(hdc, sx - POINT_DIAMETER, sy - POINT_DIAMETER, sx + POINT_DIAMETER, sy + POINT_DIAMETER);
						DeleteObject(GetStockObject(BLACK_BRUSH));
					}
				}
			};

			LOGFONT log_font = {};
			// SecureZeroMemory(&lf, sizeof lf);
			log_font.lfHeight = min(WIDTH(rectangle), HEIGHT(rectangle)) >> 4;
			_tcscpy(log_font.lfFaceName, TEXT("Comic Sans MS"));

			auto the_font = CreateFontIndirect(&log_font);
			DBJ_ASSERT(the_font != NULL);

			DBJ_ASSERT(HGDI_ERROR != SelectObject(hdc, the_font ));

			display_time();
			display_day();
			display_date();
			draw_points();

			DeleteObject(GetStockObject(SYSTEM_FIXED_FONT));

			auto obj = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
				clock_hands_draw(hdc);
			Ellipse(hdc, -(POINT_DIAMETER * 2), -(POINT_DIAMETER * 2), POINT_DIAMETER * 2, POINT_DIAMETER * 2);
			DeleteObject(GetStockObject(BLACK_BRUSH));

			// DBJ_ASSERT( HGDI_ERROR != SelectObject(hdc, obj));
			if (obj != NULL && obj != HGDI_ERROR)
				::DeleteObject(obj);

			EndPaint(hWnd, &ps);
		} 
		break;

		case WM_TIMER:
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		
		case WM_SIZE: {
			RECT rectangle = {};
			if (~fCircle && fResized) {
				SetWindowLong(hApp, GWL_STYLE, WS_BORDER | WS_VISIBLE | WS_SIZEBOX);
				GetWindowRect(hApp, &rectangle);
				HRGN hEll = CreateRectRgn(0, 0, rectangle.right - rectangle.left, rectangle.bottom - rectangle.top);
				SetWindowRgn(hWnd, hEll, TRUE);
				SetWindowPos(hWnd, NULL, rectangle.left, rectangle.top, WIDTH(rectangle), HEIGHT(rectangle), SWP_FRAMECHANGED);
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

