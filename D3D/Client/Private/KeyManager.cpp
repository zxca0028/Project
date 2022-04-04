#include "pch.h"
#include "KeyManager.h"

IMPLEMENT_SINGLETON(CKeyManager);

CKeyManager::CKeyManager()
{}

void CKeyManager::Key_Update()
{
    m_dwKey = 0;

    if (GetAsyncKeyState(VK_RETURN)  & 0x8000) { m_dwKey |= KEY_ENTER; }
    if (GetAsyncKeyState(VK_SPACE)   & 0x8000) { m_dwKey |= KEY_SPACE; }
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { m_dwKey |= KEY_LB;    }
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) { m_dwKey |= KEY_RB;    }
    if (GetAsyncKeyState('W')        & 0x8000) { m_dwKey |= KEY_W;     }
    if (GetAsyncKeyState('S')        & 0x8000) { m_dwKey |= KEY_S;     }
    if (GetAsyncKeyState('A')        & 0x8000) { m_dwKey |= KEY_A;     }
    if (GetAsyncKeyState('D')        & 0x8000) { m_dwKey |= KEY_D;     }
    if (GetAsyncKeyState(VK_MENU)    & 0x8000) { m_dwKey |= KEY_ALT;   }
    if (GetAsyncKeyState(VK_UP)      & 0x8000) { m_dwKey |= KEY_UP;    }
    if (GetAsyncKeyState(VK_DOWN)    & 0x8000) { m_dwKey |= KEY_DOWN;  }
    if (GetAsyncKeyState(VK_LEFT)    & 0x8000) { m_dwKey |= KEY_LEFT;  }
    if (GetAsyncKeyState(VK_RIGHT)   & 0x8000) { m_dwKey |= KEY_RIGHT; }
    if (GetAsyncKeyState('Z')        & 0x8000) { m_dwKey |= KEY_Z;     }
    if (GetAsyncKeyState('C')        & 0x8000) { m_dwKey |= KEY_C;     }
    if (GetAsyncKeyState('T')        & 0x8000) { m_dwKey |= KEY_T;     }
    if (GetAsyncKeyState('1')        & 0x8000) { m_dwKey |= KEY_1;     }
    if (GetAsyncKeyState('2')        & 0x8000) { m_dwKey |= KEY_2;     }
    if (GetAsyncKeyState(VK_F1)      & 0x8000) { m_dwKey |= KEY_F1;    }
    if (GetAsyncKeyState(VK_F2)      & 0x8000) { m_dwKey |= KEY_F2;    }
    if (GetAsyncKeyState(VK_F3)      & 0x8000) { m_dwKey |= KEY_F3;    }
    if (GetAsyncKeyState(VK_F4)      & 0x8000) { m_dwKey |= KEY_F4;    }
    if (GetAsyncKeyState(VK_F5)      & 0x8000) { m_dwKey |= KEY_F5;    }
    if (GetAsyncKeyState('Q')        & 0x8000) { m_dwKey |= KEY_Q;     }
    if (GetAsyncKeyState('W')        & 0x8000) { m_dwKey |= KEY_W;     }
    if (GetAsyncKeyState('E')        & 0x8000) { m_dwKey |= KEY_E;     }
    if (GetAsyncKeyState('R')        & 0x8000) { m_dwKey |= KEY_R;     }
    if (GetAsyncKeyState('F')        & 0x8000) { m_dwKey |= KEY_F;     }
}

bool CKeyManager::Key_Up(DWORD dwKey)
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

bool CKeyManager::Key_Down(DWORD dwKey)
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

bool CKeyManager::Key_Pressing(DWORD dwKey)
{
    if (m_dwKey & dwKey)
    {
        return true; 
    }

    return false;
}

void CKeyManager::Free()
{
}
