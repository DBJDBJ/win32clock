// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

#include "resource.h"

/*
dbj begins here
*/
#include <crtdbg.h>
#include <tchar.h>
#include <string>
#include <vector>
// inline is the keyword, in C++ and C99.
#define DBJ_INLINE inline
#define DBJ_USE_SOUNDS  1==0
#define DBJ_ASSERT /*_ASSERTE*/

// temporary place for a commomn core code
namespace dbj {

	//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
	template<typename CHAR_TYPE>
	DBJ_INLINE auto getLastErrorMessage(
		const CHAR_TYPE * const prompt = 0, DWORD errorMessageID = ::GetLastError()
	)
	{
		using string = std::basic_string<CHAR_TYPE>;
		//Get the error message, if any.
		if (errorMessageID == 0)
			return string(); //No error message has been recorded
#ifdef UNICODE
		LPWSTR messageBuffer = nullptr;
#else
		LPSTR messageBuffer = nullptr;
#endif
		size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		string message(messageBuffer, size);

		//Free the buffer.
		::LocalFree(messageBuffer);

		if (prompt)
		return string(prompt).append(message);

		return message;
	}

	/* Find the length of S, but scan at most MAXLEN characters.  If no '\0'
	terminator is found within the first MAXLEN characters, return MAXLEN.
	Inspired by: https://opensource.apple.com/source/bash/bash-80/bash/lib/sh/strnlen.c
	*/
	DBJ_INLINE size_t strnlen(register const char *s, size_t maxlen)
	{
		register const char *e = {};
		size_t n = {};

		for (e = s, n = 0; *e && n < maxlen; e++, n++)
			;
		return n;
	}
	/*
	Tame the full path filenames  in __FILE__
	https://stackoverflow.com/questions/8487986/file-macro-shows-full-path/8488201#8488201
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
	*/
	DBJ_INLINE constexpr auto nicer_filename(const char * filename) {
		return (strrchr(filename, '\\') ? strrchr(filename, '\\') + 1 : filename);
	}

	namespace sysinfo {

		constexpr auto INFO_BUFFER_SIZE = 32767;
		static TCHAR  infoBuf[INFO_BUFFER_SIZE] = {};
		static DWORD  bufCharCount = INFO_BUFFER_SIZE;
		// 
		DBJ_INLINE auto computer_name () {
			bufCharCount = INFO_BUFFER_SIZE;
			if (!GetComputerName(infoBuf, &bufCharCount))
				throw getLastErrorMessage(TEXT("dbj::sysinfo::computer_name() -- "));
			return std::basic_string<TCHAR>(infoBuf, bufCharCount);
		}

		DBJ_INLINE auto user_name() {
			bufCharCount = INFO_BUFFER_SIZE;
			if (!GetUserName(infoBuf, &bufCharCount))
				throw getLastErrorMessage(TEXT("dbj::sysinfo::user_name() -- "));
			return std::basic_string<TCHAR>(infoBuf, bufCharCount);
		}

		DBJ_INLINE auto system_directory () {
			bufCharCount = INFO_BUFFER_SIZE;
			if (!GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE))
				throw getLastErrorMessage(TEXT("dbj::sysinfo::system_directory() -- "));
			return std::basic_string<TCHAR>(infoBuf, dbj::strnlen(infoBuf, INFO_BUFFER_SIZE-1));
		}

		DBJ_INLINE auto windows_directory() {
			bufCharCount = INFO_BUFFER_SIZE;
			if (!GetWindowsDirectory(infoBuf, INFO_BUFFER_SIZE))
				throw getLastErrorMessage(TEXT("dbj::sysinfo::windows_directory() -- "));
			return std::basic_string<TCHAR>(infoBuf, dbj::strnlen(infoBuf, INFO_BUFFER_SIZE-1));
		}
	}

}

#include "dbjtrace.h"

