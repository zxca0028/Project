#pragma once

#define KEY_W     0x00000001
#define KEY_A     0x00000002
#define KEY_S     0x00000004
#define KEY_D     0x00000008
			      
#define KEY_1     0x00000010
#define KEY_2     0x00000020
#define KEY_3     0x00000040
#define KEY_4     0x00000080
			      
#define KEY_Z     0x00000100
#define KEY_X     0x00000200
#define KEY_C     0x00000400
#define KEY_V     0x00000800
			      
#define KEY_UP    0x00001000
#define KEY_DN    0x00002000
#define KEY_LE    0x00004000
#define KEY_RI    0x00008000

#define KEY_ENTER 0x00010000
#define KEY_SPACE 0x00020000
#define KEY_LCTRL 0x00040000
#define KEY_L_ALT 0x00080000

#define KEY_LB    0x00100000
#define KEY_RB    0x00200000
#define KEY_Q     0x00400000
#define KEY_E     0x00800000

#define KEY_ESC   0x01000000

#include "Base.h"

BEGIN(Engine)

class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager)

public:

	enum class MOUSE { X, Y, WHEEL, MOUSE_END };

private:

	explicit CKey_Manager();
	virtual ~CKey_Manager() = default;

public:

	HRESULT NativeConstruct(HINSTANCE hInst, HWND hWnd);

public:

	_int  Key_Update();
	_bool Key_Up      (DWORD dwKey);
	_bool Key_Down    (DWORD dwKey);
	_bool Key_Pressing(DWORD dwKey);

public:

	_long Mouse_Move(MOUSE eMouseState)
	{
		return ((_long*)&m_MouseState)[(_uint)eMouseState];
	}

private:

	LPDIRECTINPUT8       m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8 m_pMouse    = nullptr;

private:

	DWORD	m_dwKey		 = 0;
	DWORD	m_dwKeyDown	 = 0;
	DWORD	m_dwKeyUp	 = 0;

private:

	DIMOUSESTATE m_MouseState;

public:

	virtual void Free() override;

};

END