#pragma once

#include<d3d9.h>
#include<d3dx9.h>

#pragma warning(disable : 26451)


#include<vector>
#include<list>
#include<map>
#include<unordered_map>
#include<queue>
#include<string>
#include<process.h>
#include<typeinfo>

#include <io.h>
#include "fmod.hpp"

#ifdef _DEBUG
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#endif

#include "Client_Macro.h"
#include "Client_Typedef.h"
#include "Client_Function.h"

enum LEVEL { LEVEL_STATIC, LEVEL_MAIN, LEVEL_GAMEPLAY, LEVEL_RESULT, LEVEL_END };

#define KEY_UP    0x00000001
#define KEY_DOWN  0x00000002
#define KEY_LEFT  0x00000004
#define KEY_RIGHT 0x00000008

#define KEY_W     0x00000010
#define KEY_A     0x00000020
#define KEY_S     0x00000040
#define KEY_D     0x00000080

#define KEY_SPACE 0x00000100

const unsigned int g_iWinCX = 700;
const unsigned int g_iWinCY = 1000;

const unsigned int g_iCenterX = g_iWinCX / 2;
const unsigned int g_iCenterY = g_iWinCY / 2;

extern HWND g_hWnd;
extern HDC  g_hDC;

using namespace std;