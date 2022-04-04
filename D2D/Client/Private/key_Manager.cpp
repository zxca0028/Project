#include "pch.h"
#include "Key_Manager.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(CKey_Manager)

CKey_Manager::CKey_Manager()
{
}

_int CKey_Manager::Key_Update()
{
    m_dwKey = 0;

    if (GetAsyncKeyState(VK_SPACE) & 0x8000) { m_dwKey |= KEY_SPACE; }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { m_dwKey |= KEY_RIGHT; }
    if (GetAsyncKeyState(VK_LEFT)  & 0x8000) { m_dwKey |= KEY_LEFT;  }
    if (GetAsyncKeyState(VK_UP)    & 0x8000) { m_dwKey |= KEY_UP;    }
    if (GetAsyncKeyState(VK_DOWN)  & 0x8000) { m_dwKey |= KEY_DOWN;  }
    if (GetAsyncKeyState('W')      & 0x8000) { m_dwKey |= KEY_W;     }
    if (GetAsyncKeyState('A')      & 0x8000) { m_dwKey |= KEY_A;     }
    if (GetAsyncKeyState('S')      & 0x8000) { m_dwKey |= KEY_S;     }
    if (GetAsyncKeyState('D')      & 0x8000) { m_dwKey |= KEY_D;     }


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
}

END