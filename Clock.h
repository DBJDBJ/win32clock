#pragma once

#pragma comment(lib, "winmm.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "resource.h"
#include <math.h>
#include <stdio.h>
#include <mmsystem.h>

// If you want a Circle Clock, uncomment the following comment
/*
  #define CIRCLE_CLOCK
*/

#define POINT_DIAMETER    4
#define WIDTH(r)          (r.right-r.left)
#define HEIGHT(r)         (r.bottom-r.top)
#define IDM_QUIT          1729
#define IDM_SOUND         1024

#ifndef M_PI
#define M_PI              acos(-1.0)
#endif
