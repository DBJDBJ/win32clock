// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// #include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

#include "resource.h"

#include <crtdbg.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <memory>
#include <optional>
/*
dbj begins here
*/
// inline is the keyword, in C++ and C99.
#define DBJ_INLINE inline
#define DBJ_USE_SOUNDS  1==0
#define DBJ_ASSERT /*_ASSERTE*/

#ifdef __cpp_lib_is_final
#define DBJ_FINAL final
#else
#define DBJ_FINAL
#endif




