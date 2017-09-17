// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <winsock.h>
#include <tchar.h>

#include <objidl.h>
#include <gdiplus.h>

// TODO: reference additional headers your program requires here
#define DBJ_USE_SOUNDS  1==0
#include "dbjtrace.h"
#define DBJ_ASSERT _ASSERTE
#include <string>
