#include "Input.h"

IMPLEMENT_SINGLETON(CInput)

CInput::CInput()
{}

HRESULT CInput::NativeConstruct(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
	{
		return E_FAIL;
	}

	m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyboard->Acquire();

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
	{
		return E_FAIL;
	}

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->Acquire();

    return S_OK;
}

_int CInput::Update_Input()
{
	m_pKeyboard->GetDeviceState(256, m_KeyState);

	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

    return _int();
}

_byte CInput::Get_KeyState(_byte bKey)
{
    return m_KeyState[bKey];
}

_byte CInput::Get_MouseClickState(MOUSE_CLICK eClick)
{
	return m_MouseState.rgbButtons[eClick];
}

_long CInput::Get_MouseMoveState(MOUSE_MOVE eMove)
{
	switch (eMove)
	{
	case X:
		return m_MouseState.lX;

	case Y:
		return m_MouseState.lY;

	case WHEEL:
		return m_MouseState.lZ;

	default:
		return 0;
	}
}

void CInput::Free()
{
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pMouse);

	Safe_Release(m_pInputSDK);
}
