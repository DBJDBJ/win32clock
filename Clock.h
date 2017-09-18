#pragma once

constexpr static auto OPACITY = 180;
constexpr static auto FACTOR = 40;
/*
#define HOUR_WIDTH			10
#define MINUTE_WIDTH		6
#define SECOND_WIDTH		3
*/
constexpr static INT POINT_DIAMETER = 4;
constexpr static auto WIDTH(RECT r) {	return		(r.right - r.left);}
constexpr static auto HEIGHT(RECT r) {	return		(r.bottom - r.top);}
constexpr static auto IDM_TOPMOST = 2016;
constexpr static auto IDM_QUIT = 1729;
constexpr static auto IDM_SOUND = 1024;
constexpr static auto IDM_CIRCLE = 0xbabe;
constexpr static auto IDM_RECT = 0xcafe;

#ifndef M_PI
constexpr static auto  M_PI = 3.141592653589793;
#endif

#ifndef RADIAN
constexpr static auto  RADIAN = 0.017453292519943295;
#endif

namespace dbj {


	//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
	DBJ_INLINE std::string getLastErrorMessage
	(
		DWORD errorMessageID = ::GetLastError()
	)
	{
		//Get the error message, if any.
		if (errorMessageID == 0)
			return std::string(); //No error message has been recorded

		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);

		//Free the buffer.
		LocalFree(messageBuffer);

		return message;
	}

	DBJ_INLINE void Line(HDC hDC, int sx, int sy, int ex, int ey, Gdiplus::ARGB clr, Gdiplus::REAL w)
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

	DBJ_INLINE  const TCHAR * const days( SYSTEMTIME & time_ ) noexcept {
		constexpr static TCHAR * days[] = { TEXT("Sunday"), TEXT("Monday"), TEXT("Tuesday"), TEXT("Wednesday"), TEXT("Thursday"), TEXT("Friday"), TEXT("Saturday") };
		return days[ time_.wDayOfWeek ];
    }

	DBJ_INLINE  const TCHAR * const months(SYSTEMTIME & time_) noexcept {
		constexpr static TCHAR * months[] = {
		NULL, TEXT("January"), TEXT("February"), TEXT("March"), TEXT("April"), TEXT("May"), TEXT("June"),
		TEXT("July"), TEXT("August"), TEXT("September"), TEXT("October"), TEXT("November"), TEXT("December") };
		return months[time_.wMonth];
	}

	DBJ_INLINE  const TCHAR * const GetWelcomeMessage() noexcept
	{
		constexpr static TCHAR * szWelcome[] = { TEXT("Good Morning"), TEXT("Good Afternoon"), TEXT("Good Evening"), TEXT("Good Night") };

		SYSTEMTIME time;
		GetLocalTime(&time);
		if (time.wHour >= 4 && time.wHour < 12) {
			return szWelcome[0];
		}
		if (time.wHour >= 12 && time.wHour < 16) {
			return szWelcome[1];
		}
		if (time.wHour >= 16 && time.wHour < 20) {
			return szWelcome[2];
		}
			return szWelcome[3];
	}

	DBJ_INLINE  POINT get_width_height(HWND hwnd)
	{
		POINT widhgt = {}; RECT rect = {};
		
		DBJ_ASSERT(hwnd != NULL);
		DBJ_ASSERT(GetWindowRect(hwnd, &rect));
			widhgt.x = rect.right - rect.left;
			widhgt.y = rect.bottom - rect.top;
		return widhgt;
	}

	DBJ_INLINE  void clock_hands_draw(HDC hdc)
	{
		constexpr static Gdiplus::REAL HOUR_WIDTH = 10;
		constexpr static Gdiplus::REAL MINUTE_WIDTH = 6;
		constexpr static Gdiplus::REAL SECOND_WIDTH = 3;

		POINT winsize = get_width_height(WindowFromDC(hdc));

		LONG radius = min(winsize.x,winsize.y) / 3 ; // {};
		POINT pMin = {}, pSec = {}, pHour = {};
		SYSTEMTIME  localTime = {} ;
		GetLocalTime(&localTime);

		// radius -= 4 * POINT_DIAMETER;
		pSec.y = (LONG)(radius * cos(localTime.wSecond * 6 * RADIAN));
		pSec.x = (LONG)(radius * sin(localTime.wSecond * 6 * RADIAN));
		
		// radius -= 6 * POINT_DIAMETER;
		pMin.y = (LONG)(radius * cos(localTime.wMinute * 6 * RADIAN));
		pMin.x = (LONG)(radius * sin(localTime.wMinute * 6 * RADIAN));
		
		// radius -= 8 * POINT_DIAMETER;
		pHour.y = (LONG)(radius * cos(localTime.wHour * 30 * RADIAN));
		pHour.x = (LONG)(radius * sin(localTime.wHour * 30 * RADIAN));

		// ... draw hands ...
		Line(hdc, 0, 0, pHour.x, pHour.y, Gdiplus::Color::DarkGray, HOUR_WIDTH);
		Line(hdc, 0, 0, pMin.x, pMin.y, Gdiplus::Color::DarkGray, MINUTE_WIDTH);
		Line(hdc, 0, 0, pSec.x, pSec.y, Gdiplus::Color::Black, SECOND_WIDTH);
	}
}