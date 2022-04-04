#pragma once

enum class LEVEL { LEVEL_STATIC, LEVEL_START, LEVEL_LOADING, LEVEL_MAIN, LEVEL_END };

const unsigned int g_iWinCX = 1920;
const unsigned int g_iWinCY = 1080;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

namespace Client {}

using namespace Client;