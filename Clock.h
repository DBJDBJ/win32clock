#pragma once

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Gdiplus.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "resource.h"
#include <math.h>
#include <stdio.h>
#include <mmsystem.h>
#include <winsock.h>

#define HOUR_WIDTH			10
#define OPACITY				180
#define FACTOR				40
#define MINUTE_WIDTH		6
#define SECOND_WIDTH		3
#define POINT_DIAMETER		4
#define WIDTH(r)			(r.right-r.left)
#define HEIGHT(r)			(r.bottom-r.top)
#define IDM_TOPMOST			2016
#define IDM_QUIT			1729
#define IDM_SOUND			1024
#define IDM_CIRCLE			0xbabe
#define IDM_RECT			0xcafe

#ifndef M_PI
#define M_PI				3.141592653589793
#endif

#ifndef RADIAN
#define RADIAN				0.017453292519943295
#endif

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

namespace dbj {

	__forceinline void Line(HDC hDC, int sx, int sy, int ex, int ey, Gdiplus::ARGB clr, Gdiplus::REAL w)
	{
		////int width = 20;
		//int dx = 0;
		// POINT pt[] = { sx, sy, ex, ey, ex - dx, ey - dx, ex + dx, ey + dx, ex, ey, sx, sy };
		// Polyline(hdc, pt, 6);
		Gdiplus::Graphics g(hDC);
		g.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
		auto calculated_color = Gdiplus::Color::Color(clr);
		Gdiplus::Pen p(calculated_color, w);
		p.SetEndCap(Gdiplus::LineCap::LineCapRound);
		g.DrawLine(&p, sx, sy, ex, ey);
	}

	__forceinline  TCHAR * days( unsigned int ord_ ) {
		constexpr static TCHAR * days[] = { TEXT("Sunday"), TEXT("Monday"), TEXT("Tuesday"), TEXT("Wednesday"), TEXT("Thursday"), TEXT("Friday"), TEXT("Saturday") };
		return days[ord_];
    }
	__forceinline  TCHAR * months(unsigned int ord_) {
		constexpr static TCHAR * months[] = {
		NULL, TEXT("January"), TEXT("February"), TEXT("March"), TEXT("April"), TEXT("May"), TEXT("June"),
		TEXT("July"), TEXT("August"), TEXT("September"), TEXT("October"), TEXT("November"), TEXT("December") };
		return months[ord_];
	}

	__forceinline  TCHAR * GetWelcomeMessage()
	{
		constexpr static TCHAR * szWelcome[] = { TEXT("Good Morning"), TEXT("Good Afternoon"), TEXT("Good Evening"), TEXT("Good Night") };

		SYSTEMTIME time;
		GetLocalTime(&time);
		if (time.wHour >= 4 && time.wHour < 12) {
			return szWelcome[0];
		}
		else if (time.wHour >= 12 && time.wHour < 16) {
			return szWelcome[1];
		}
		else if (time.wHour >= 16 && time.wHour < 20) {
			return szWelcome[2];
		}
		else {
			return szWelcome[3];
		}
	}
}