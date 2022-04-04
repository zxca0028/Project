#pragma once

enum LEVEL
{
	LEVEL_STATIC,
	LEVEL_LOGO,
	LEVEL_LOADING,
	LEVEL_GAMEPLAY,
	LEVEL_END
};

const unsigned int	g_iWinCX = 1280;
const unsigned int  g_iWinCY = 720;

//const unsigned int g_iWinCX = 1680;
//const unsigned int g_iWinCY = 720;

//const unsigned int	g_iWinCX = 1920;
//const unsigned int  g_iWinCY = 822;

extern HINSTANCE g_hInst;
extern HWND      g_hWnd;

extern bool m_CollisionView;
extern bool m_PlayerControl;
extern bool m_CameraChange;
extern bool m_NaviView;
extern bool m_bTargetRender;
extern float X;
extern float Y;
extern float Z;

namespace Client {}

using namespace Client;

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "fmod.h"

#pragma comment(lib, "fmodex64_vc.lib")

#include<io.h>

//

#include<tchar.h>
#include<random>