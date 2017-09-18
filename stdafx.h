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
#define DBJ_ASSERT _ASSERTE

// temporary place for a commomn core code
namespace dbj {

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

}

#include "dbjtrace.h"

