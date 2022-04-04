#include "pch.h"
#include "KeyManager.h"

CKeyManager* CKeyManager::m_pInstance = nullptr;

CKeyManager::CKeyManager()
    : m_dwKey(0)
    , m_dwKeyUp(0)
    , m_dwKeyDown(0)
{}

CKeyManager::~CKeyManager()
{}

CKeyManager* CKeyManager::Get_Instance()
{
    if (nullptr == m_pInstance) { m_pInstance = new CKeyManager; }
    return m_pInstance;
}

void CKeyManager::Destroy_Instance()
{
    if (nullptr != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

void CKeyManager::Key_Update()
{
    m_dwKey = 0;

    if (GetAsyncKeyState(VK_RETURN)  & 0x8000) { m_dwKey |= KEY_ENTER; }
    if (GetAsyncKeyState(VK_SPACE)   & 0x8000) { m_dwKey |= KEY_SPACE; }
    if (GetAsyncKeyState(VK_DOWN)    & 0x8000) { m_dwKey |= KEY_DOWN;  }
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { m_dwKey |= KEY_LB; }
    if (GetAsyncKeyState(VK_F1)      & 0x8000) { m_dwKey |= KEY_F1; }
    if (GetAsyncKeyState(VK_F2)      & 0x8000) { m_dwKey |= KEY_F2; }
    if (GetAsyncKeyState(VK_F3)      & 0x8000) { m_dwKey |= KEY_F3; }
    if (GetAsyncKeyState(VK_F4)      & 0x8000) { m_dwKey |= KEY_F4; }
    if (GetAsyncKeyState('J')        & 0x8000) { m_dwKey |= KEY_J; }
    if (GetAsyncKeyState('O')        & 0x8000) { m_dwKey |= KEY_O; }
    if (GetAsyncKeyState('P')        & 0x8000) { m_dwKey |= KEY_P; }
    if (GetAsyncKeyState(VK_ESCAPE)  & 0x8000) { m_dwKey |= KEY_ESC; }
    if (GetAsyncKeyState('B')        & 0x8000) { m_dwKey |= KEY_B; }
    if (GetAsyncKeyState(VK_F5)      & 0x8000) { m_dwKey |= KEY_F5; }
    if (GetAsyncKeyState(VK_F6)      & 0x8000) { m_dwKey |= KEY_F6; }
    if (GetAsyncKeyState('F')        & 0x8000) { m_dwKey |= KEY_F; }
    if (GetAsyncKeyState('1')        & 0x8000) { m_dwKey |= KEY_1; }
    if (GetAsyncKeyState('2')        & 0x8000) { m_dwKey |= KEY_2; }
    if (GetAsyncKeyState('3')        & 0x8000) { m_dwKey |= KEY_3; }
    if (GetAsyncKeyState('4')        & 0x8000) { m_dwKey |= KEY_4; }
    if (GetAsyncKeyState('5')        & 0x8000) { m_dwKey |= KEY_5; }
    if (GetAsyncKeyState('6')        & 0x8000) { m_dwKey |= KEY_6; }
    if (GetAsyncKeyState('7')        & 0x8000) { m_dwKey |= KEY_7; }
    if (GetAsyncKeyState('8')        & 0x8000) { m_dwKey |= KEY_8; }
    if (GetAsyncKeyState('9')        & 0x8000) { m_dwKey |= KEY_9; }
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
    if (m_dwKey & dwKey) { return true; }

    return false;
}
