#pragma once
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
// #include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
// GDI+
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

#ifndef __argc
#define __argc  (*__p___argc())  // Pointer to number of command line arguments
#endif // !__argc

#ifndef __argv
#define __argv  (*__p___argv())  // Pointer to table of narrow command line arguments
#endif // !__argv

#ifndef _wargv
#define __wargv (*__p___wargv()) // Pointer to table of wide command line arguments
#endif // !_wargv




#include "resource.h"
/*
#include <crtdbg.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <iostream>
*/
/*
dbj begins here
*/
#include <dbj++.h>

#define DBJ_USE_SOUNDS (1==0)
#include "Clock.h"
