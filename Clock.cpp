// Clock.cpp : Defines the entry point for the application.
// @author : x0r

#include "stdafx.h"
#include "SoundPlayer.h"
#include "Clock.h"

// Global Variables:
HINSTANCE hInst;
SoundPlayer *tick, *tock;
TCHAR szTitle[] = TEXT("Clock");
TCHAR szWindowClass[] = TEXT("[x0r-Cl0ck]");
TCHAR buf[100];
TCHAR* days[] = { NULL, TEXT("Sunday"), TEXT("Monday"), TEXT("Tuesday"), TEXT("Wednesday"), TEXT("Thursday"), TEXT("Friday"), TEXT("Saturday") };
TCHAR* months[] = { 
	NULL, TEXT("January"), TEXT("February"), TEXT("March"), TEXT("April"), TEXT("May"), TEXT("June"),
	TEXT("July"), TEXT("August"), TEXT("September"), TEXT("October"), TEXT("November"), TEXT("December")
};

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
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
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	//tick = new SoundPlayer(IDR_TICK);
	//tock = new SoundPlayer(IDR_TOCK);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_DROPSHADOW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLOCK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance;

   int width = GetSystemMetrics(SM_CXSCREEN), height = GetSystemMetrics(SM_CYSCREEN);
   hWnd = CreateWindowEx(0 | WS_EX_LAYERED, szWindowClass, szTitle, 0,
      width/2-150, height/2-150, 400, 400, NULL, NULL, hInstance, NULL);

   int dwFlags = WS_BORDER | WS_VISIBLE;

#ifndef CIRCLE_CLOCK
   dwFlags |= WS_SIZEBOX;
#endif
   
   SetWindowLong(hWnd, GWL_STYLE, dwFlags);
   SetWindowPos(hWnd, NULL, width-420, 20, 400, 400, SWP_FRAMECHANGED);

   if (!hWnd)
   {
      return FALSE;
   }

   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, radius, sy, sx;
	static LONG pStyle;
	PAINTSTRUCT ps;
	HDC hdc;
	HGDIOBJ obj;
	static HMENU hMenu;
	static BOOL bSound = FALSE;
	RECT r;

	switch (message)
	{
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDM_QUIT:
				if (MessageBox(hWnd, TEXT("Quit Application ?"), szTitle, MB_ICONINFORMATION | MB_YESNO) == IDYES) {
					DestroyWindow(hWnd);
				}
				break;
			case IDM_SOUND:
				{
					UINT uState = GetMenuState(hMenu, IDM_SOUND, MF_BYCOMMAND);
					if ((uState & MF_CHECKED) == MF_CHECKED) {
						CheckMenuItem(hMenu, IDM_SOUND, FALSE);
					} else {
						CheckMenuItem(hMenu, IDM_SOUND, TRUE);
					}
					bSound = ~bSound;
				}
				break;
			}
		}
		break;
	case WM_CREATE:
		{
			SetLayeredWindowAttributes(hWnd, 0x00ffffff, 160, LWA_ALPHA);

#ifdef CIRCLE_CLOCK
			GetWindowRect(hWnd, &r);
			HRGN hEll = CreateEllipticRgn(0, 0, r.right-r.left, r.bottom-r.top);
			SetWindowRgn(hWnd, hEll, TRUE);
#endif
			SetTimer(hWnd, 1, 1000, NULL);
		}
		break;
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		pStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
		SetLayeredWindowAttributes(hWnd, 0x00ffffff, 100, LWA_ALPHA);
		break;
	case WM_LBUTTONUP:
		if (GetCapture() == hWnd) {
			SetWindowLong(hWnd, GWL_EXSTYLE, pStyle);
			SetLayeredWindowAttributes(hWnd, 0x00ffffff, 160, LWA_ALPHA);
			ReleaseCapture();
		}
		break;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {
			RECT r;
			GetWindowRect(hWnd, &r);
			POINT pt = {0};
			GetCursorPos(&pt);
			MoveWindow(hWnd, pt.x-(r.right-r.left)/2, pt.y-(r.bottom-r.top)/2, r.right-r.left, r.bottom-r.top, TRUE);
		}
		break;
	case WM_RBUTTONUP:
		{
			hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING | MF_CHECKED, IDM_SOUND, TEXT("Enable &Sound"));
			CheckMenuItem(hMenu, IDM_SOUND, bSound);
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(hMenu, MF_STRING, IDM_QUIT, TEXT("E&xit Application ..."));
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
			ClientToScreen(hWnd, &pt);
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
		}
		break;
	case WM_PAINT:
		{
			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);

			hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);

#ifdef CIRCLE_CLOCK
			GetWindowRect(hWnd, &r);
			HPEN hBorder = CreatePen(PS_NULL, 5, 0x000000);
			SelectObject(hdc, hBorder);
			Ellipse(hdc, 0, 0, r.right-r.left, r.bottom-r.top);
			DeleteObject(hBorder);
#endif

			// ... set mapping mode ...

			GetClientRect(hWnd, &r);
			SetMapMode(hdc, MM_ISOTROPIC);
			SetViewportOrgEx(hdc, r.left+(r.right-r.left)/2, r.top+(r.bottom-r.top)/2, NULL);
			XFORM xf;
			ZeroMemory(&xf, sizeof(xf));
			xf.eM11 = 1;
			xf.eM22 = -1;
			SetWorldTransform(hdc, &xf);
			
			SIZE vy, wy;
			GetViewportExtEx(hdc, &vy);
			GetWindowExtEx(hdc, &wy);
			wy.cy = -1;
			vy.cy = 1;
			wy.cx = wy.cx = 1;
			SetViewportExtEx(hdc, vy.cx, vy.cy, NULL);
			SetWindowExtEx(hdc, wy.cx, wy.cy, NULL);
			
			LOGFONT lf;
			ZeroMemory(&lf, sizeof lf);
			lf.lfHeight = 24;
			_tcscpy(lf.lfFaceName, TEXT("Comic Sans MS"));
			SelectObject(hdc, CreateFontIndirect(&lf));

			// ... display time ...

			SIZE tz;
			ZeroMemory(buf, sizeof buf);
			_stprintf(buf, TEXT("%02i:%02i:%02i"), sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			int len = lstrlen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx/2, min(WIDTH(r), HEIGHT(r))/4, buf, len);

			// ... display day ...

			ZeroMemory(buf, sizeof buf);
			if (HEIGHT(r) < 209 || WIDTH(r) < 209) {
				_stprintf(buf, TEXT("%.3s"), days[sysTime.wDayOfWeek]);
			} else {
				_tcscpy(buf, days[sysTime.wDayOfWeek]);
			}
			len = lstrlen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx/2, -min(WIDTH(r), HEIGHT(r))/4, buf, len);

			// ... display date ...

			RtlZeroMemory(buf, sizeof buf);
			if (HEIGHT(r) < 209 || WIDTH(r) < 209) {
				_stprintf(buf, TEXT("%02d-%.3s-%d"), sysTime.wDay, months[sysTime.wMonth], sysTime.wYear);
			} else {
				_stprintf(buf, TEXT("%s %02d, %d"), months[sysTime.wMonth], sysTime.wDay, sysTime.wYear);
			}
			len = lstrlen(buf);
			GetTextExtentPoint32(hdc, buf, len, &tz);
			TextOut(hdc, -tz.cx/2, -min(WIDTH(r), HEIGHT(r))/8, buf, len);

			// draw points ...

			radius = min((r.right - r.left) / 2, (r.bottom - r.top) / 2) - 10;
			for (int theta = 0; theta <= 360; theta += 6) {
				sx = (int) radius * sin(theta * M_PI / 180.0);
				sy = (int) radius * cos(theta * M_PI / 180.0);
				if (theta % 30 == 0) {
					Ellipse(hdc, sx-2*POINT_DIAMETER, sy-2*POINT_DIAMETER, sx+2*POINT_DIAMETER, sy+2*POINT_DIAMETER);
				} else {
					obj = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
					Ellipse(hdc, sx-POINT_DIAMETER, sy-POINT_DIAMETER, sx+POINT_DIAMETER, sy+POINT_DIAMETER);
					DeleteObject(GetStockObject(BLACK_BRUSH));
				}
			}
			DeleteObject(GetStockObject(SYSTEM_FIXED_FONT));

			POINT pMin, pSec, pHour;
			radius -= 3*POINT_DIAMETER;
			pSec.y = (LONG) radius * cos(sysTime.wSecond * 6 * M_PI / 180.0);
			pSec.x = (LONG) radius * sin(sysTime.wSecond * 6 * M_PI / 180.0);
			radius -= 4*POINT_DIAMETER;
			pMin.y = (LONG) radius * cos(sysTime.wMinute * 6 * M_PI / 180.0);
			pMin.x = (LONG) radius * sin(sysTime.wMinute * 6 * M_PI / 180.0);
			radius -= 5*POINT_DIAMETER;
			pHour.y = (LONG) radius * cos(sysTime.wHour * 30 * M_PI / 180.0);
			pHour.x = (LONG) radius * sin(sysTime.wHour * 30 * M_PI / 180.0);

			HPEN hSPen = CreatePen(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 5, 0x003686f7);
			HPEN hMPen = CreatePen(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 5, 0x003ec182);
			HPEN hHPen = CreatePen(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 5, 0x00f8bc0c);

			// ... draw hands ...

			HGDIOBJ hObj = SelectObject(hdc, hHPen);
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, pHour.x, pHour.y);
			DeleteObject(hHPen);

			SelectObject(hdc, hMPen);
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, pMin.x, pMin.y);
			DeleteObject(hMPen);

			SelectObject(hdc, hSPen);
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, pSec.x, pSec.y);
			DeleteObject(hSPen);

			obj = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Ellipse(hdc, -POINT_DIAMETER*2, -POINT_DIAMETER*2, POINT_DIAMETER*2, POINT_DIAMETER*2);
			DeleteObject(GetStockObject(BLACK_BRUSH));

			SelectObject(hdc, obj);
			// ... play sound ...
			if (bSound) {
				if (sysTime.wSecond & 1) {
					PlaySound(MAKEINTRESOURCE(IDR_TOCK), hInst, SND_SYNC | SND_RESOURCE);
					//tock->Stop();
					//tick->Play();
				} else {
					PlaySound(MAKEINTRESOURCE(IDR_TICK), hInst, SND_SYNC | SND_RESOURCE);
					//tick->Stop();
					//tock->Play();
				}
			}

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_TIMER:
		{			
			// ... repaint ...
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostMessage(hWnd, WM_QUIT, 0, 0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}