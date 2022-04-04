#pragma once

extern  HWND	g_hWnd;

#define	OBJ_NO_EVENT	0
#define OBJ_DEAD		1

#define READY_OK		0
#define READY_FAILED    -1

#define G				9.81f

namespace CONSTANT
{
	const	LONG	RES_X = 1280;
	const	LONG	RES_Y = 720;

	const	LONG	CT_X = RES_X >> 1;
	const	LONG	CT_Y = RES_Y >> 1;

	const	RECT	BOUNDARY = { 0,0,RES_X,RES_Y };	
}

#define DEG_TO_RAD		3.14159 / 180