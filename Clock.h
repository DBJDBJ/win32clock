#pragma once
/*
Copyright 2017 by dbj@dbj.org

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
constexpr static auto OPACITY = 180;
constexpr static auto FACTOR = 40;
constexpr static auto POINT_DIAMETER = 4;
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

	/*
	NOTE for later:
	Show hide icons on the taskbar (of anything we have HWND of)
	https://msdn.microsoft.com/en-us/library/windows/desktop/bb774652(v=vs.85).aspx
	*/


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
		pHour.y = (LONG)((radius - 28) * cos(localTime.wHour * 30 * RADIAN));
		pHour.x = (LONG)((radius - 28) * sin(localTime.wHour * 30 * RADIAN));


		// ... draw hands ...
		dbj::win32::LINE liner_(hdc, Gdiplus::Color::DarkGray);

			 liner_(0, 0, pHour.x, pHour.y, HOUR_WIDTH  );
			 liner_(0, 0, pMin.x,  pMin.y,  MINUTE_WIDTH);
				liner_.color(Gdiplus::Color::Black);
			 liner_(0, 0, pSec.x,  pSec.y,  SECOND_WIDTH);
/*
		Line(hdc, 0, 0, pHour.x, pHour.y, Gdiplus::Color::DarkGray, HOUR_WIDTH);
		Line(hdc, 0, 0, pMin.x, pMin.y, Gdiplus::Color::DarkGray, MINUTE_WIDTH);
		Line(hdc, 0, 0, pSec.x, pSec.y, Gdiplus::Color::Black, SECOND_WIDTH);
*/
	}
	
}