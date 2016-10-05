#pragma once

#pragma comment(lib, "winmm.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "resource.h"
#include <math.h>
#include <stdio.h>
#include <mmsystem.h>

#define HOUR_WIDTH                    10
#define OPACITY                       180
#define FACTOR                        40
#define MINUTE_WIDTH                  6
#define SECOND_WIDTH                  3
#define POINT_DIAMETER                4
#define WIDTH(r)                      (r.right-r.left)
#define HEIGHT(r)                     (r.bottom-r.top)

#define IDM_TOPMOST                   0xFACE
#define IDM_QUIT                      0xBABE
#define IDM_SOUND                     0xDEAD
#define IDM_CIRCLE                    0xCAFE

#ifndef M_PI
#define M_PI                          3.141592653589793
#endif

#ifndef RADIAN
#define RADIAN                        0.017453292519943295
#endif
