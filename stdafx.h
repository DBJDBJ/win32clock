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