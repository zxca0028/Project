#include "pch.h"
#include "BackGround_BT.h"
#include "BitmapManager.h"
#include "GameObjectManager.h"

CBackGround_BT::CBackGround_BT()
{
}

CBackGround_BT::~CBackGround_BT()
{
    Release_GameObject();
}

int CBackGround_BT::Ready_GameObject()
{
    m_bIsDead = false;
    m_bIsActive = false;

    m_tInfo.iCX = 1878;
    m_tInfo.iCY = 740;

    return READY_OK;
}

int CBackGround_BT::Update_GameObject()
{
    if (true == m_bIsDead) { return OBJ_DEAD; }

    m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
    m_tInfo.fX -= m_fSpeed / 4;

    return OBJ_NO_EVENT;
}

void CBackGround_BT::Late_Update_GameObject()
{
    if (m_tInfo.fX <= m_tInfo.iCX / -2) { m_bIsDead = true; }
}

void CBackGround_BT::Render_GameObject(HDC hDC)
{
    HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"BonusTime_BG");
    if (nullptr == hMemDC_BG) { return; }
    BitBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC_BG, 0, 0, SRCCOPY);
}

void CBackGround_BT::Release_GameObject()
{}

CGameObject* CBackGround_BT::Create(float fX, float fY)
{
    CGameObject* pInstance = new CBackGround_BT;

    if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

    pInstance->Set_Info(fX, fY);

    return pInstance;
}
