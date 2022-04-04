#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput final : public CBase
{
	DECLARE_SINGLETON(CInput)

public:

	enum MOUSE_CLICK { LB, RB, WB, XB };
	enum MOUSE_MOVE  { X, Y, WHEEL};

private:

	explicit CInput();
	virtual ~CInput() = default;

public:

	HRESULT NativeConstruct(HINSTANCE hInst, HWND hWnd);
	_int	Update_Input();

public:

	_byte	Get_KeyState	   (_byte bKey);
	_byte	Get_MouseClickState(MOUSE_CLICK eClick);
	_long	Get_MouseMoveState (MOUSE_MOVE eMove);
	
private:

	LPDIRECTINPUT8		 m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8 m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8 m_pMouse    = nullptr;

private:

	DIMOUSESTATE m_MouseState;
	_byte		 m_KeyState[256];

public:

	virtual void Free() override;

};

END	