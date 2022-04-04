#include "pch.h"
#include "Flag.h"
#include "BitmapManager.h"
#include "GameObjectManager.h"

CFlag::CFlag()
{}

CFlag::~CFlag()
{
    Release_GameObject();
}

int CFlag::Ready_GameObject()
{
    m_bIsActive = true;

    m_tInfo.iCX = 429;
    m_tInfo.iCY = 490;

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 10;

    m_tFrame.dwFrameSpeed = 150;
    m_tFrame.dwFrameTime = GetTickCount64();

    return READY_OK;
}

int CFlag::Update_GameObject()
{
    if (m_bFinish == true)
    {
        m_tInfo.fX -= 0;
    }
    else if (m_bFinish == false)
    {
        m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
        m_tInfo.fX -= m_fSpeed;
    }
    
    return OBJ_NO_EVENT;
}

void CFlag::Late_Update_GameObject()
{
    if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount64())
    {
        ++m_tFrame.iFrameStart;
        m_tFrame.dwFrameTime = GetTickCount64();
    }

    if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
    {
        m_tFrame.iFrameStart = 6;
    }
}

void CFlag::Render_GameObject(HDC hDC)
{
    HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Flag");
    if (nullptr == hMemDC) { return; }
    GdiTransparentBlt
    (
        hDC,
        m_tRect.left,
        m_tRect.top,
        m_tInfo.iCX,
        m_tInfo.iCY,
        hMemDC,
        m_tFrame.iFrameStart * m_tInfo.iCX,
        0, 
        m_tInfo.iCX, 
        m_tInfo.iCY,
        RGB(255, 0, 255)
    );
    
}

void CFlag::Release_GameObject()
{
}

CGameObject* CFlag::Create(float fX, float fY)
{
    CGameObject* pInstance = new CFlag;
    
    if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

    pInstance->Set_Info(fX, fY);

    return pInstance;
}
