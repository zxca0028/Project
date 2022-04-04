#include "Key_Manager.h"

IMPLEMENT_SINGLETON(CKey_Manager)

CKey_Manager::CKey_Manager()
{
}

HRESULT CKey_Manager::NativeConstruct(HINSTANCE hInst, HWND hWnd)
{
    if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
    {
        return E_FAIL;
    }

    m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    m_pMouse->SetDataFormat(&c_dfDIMouse);
    m_pMouse->Acquire();

    return S_OK;
}

_int CKey_Manager::Key_Update()
{
    m_dwKey = 0;
    
    if (GetAsyncKeyState('W')         & 0x8000) { m_dwKey |= KEY_W;     }
    if (GetAsyncKeyState('A')         & 0x8000) { m_dwKey |= KEY_A;     }
    if (GetAsyncKeyState('S')         & 0x8000) { m_dwKey |= KEY_S;     }
    if (GetAsyncKeyState('D')         & 0x8000) { m_dwKey |= KEY_D;     }
    if (GetAsyncKeyState('1')         & 0x8000) { m_dwKey |= KEY_1;     }
    if (GetAsyncKeyState('2')         & 0x8000) { m_dwKey |= KEY_2;     }
    if (GetAsyncKeyState('3')         & 0x8000) { m_dwKey |= KEY_3;     }
    if (GetAsyncKeyState('4')         & 0x8000) { m_dwKey |= KEY_4;     }
    if (GetAsyncKeyState('Z')         & 0x8000) { m_dwKey |= KEY_Z;     }
    if (GetAsyncKeyState('X')         & 0x8000) { m_dwKey |= KEY_X;     }
    if (GetAsyncKeyState('C')         & 0x8000) { m_dwKey |= KEY_C;     }
    if (GetAsyncKeyState('V')         & 0x8000) { m_dwKey |= KEY_V;     }
    if (GetAsyncKeyState('Q')         & 0x8000) { m_dwKey |= KEY_Q;     }
    if (GetAsyncKeyState('E')         & 0x8000) { m_dwKey |= KEY_E;     }
    if (GetAsyncKeyState(VK_UP)       & 0x8000) { m_dwKey |= KEY_UP;    }
    if (GetAsyncKeyState(VK_DOWN)     & 0x8000) { m_dwKey |= KEY_DN;    }
    if (GetAsyncKeyState(VK_LEFT)     & 0x8000) { m_dwKey |= KEY_LE;    }
    if (GetAsyncKeyState(VK_RIGHT)    & 0x8000) { m_dwKey |= KEY_RI;    }
    if (GetAsyncKeyState(VK_RETURN)   & 0x8000) { m_dwKey |= KEY_ENTER; }
    if (GetAsyncKeyState(VK_SPACE)    & 0x8000) { m_dwKey |= KEY_SPACE; }
    if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) { m_dwKey |= KEY_LCTRL; }
    if (GetAsyncKeyState(VK_MENU)     & 0x8000) { m_dwKey |= KEY_L_ALT; }
    if (GetAsyncKeyState(VK_LBUTTON)  & 0x8000) { m_dwKey |= KEY_LB;    }
    if (GetAsyncKeyState(VK_RBUTTON)  & 0x8000) { m_dwKey |= KEY_RB;    }
    if (GetAsyncKeyState(VK_ESCAPE)   & 0x8000) { m_dwKey |= KEY_ESC;   }


    m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

    return _int();
}

_bool CKey_Manager::Key_Up(DWORD dwKey)
{
    if (m_dwKey & dwKey)
    {
        m_dwKeyUp |= dwKey;
        return false;
    }
    else if (m_dwKeyUp & dwKey)
    {
        m_dwKeyUp ^= dwKey;
        return false;
    }

    return false;
}

_bool CKey_Manager::Key_Down(DWORD dwKey)
{
    if (m_dwKey & dwKey && !(m_dwKeyDown & dwKey))
    {
        m_dwKeyDown |= dwKey;
        return true;
    }
    else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
    {
        m_dwKeyDown ^= dwKey;
        return false;
    }

    return false;
}

_bool CKey_Manager::Key_Pressing(DWORD dwKey)
{
    if (m_dwKey & dwKey) { return true; }

    return false;
}

void CKey_Manager::Free()
{
    Safe_Release(m_pMouse);
    Safe_Release(m_pInputSDK);
}
