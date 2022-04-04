#include "pch.h"
#include "Ground_BT.h"
#include "BitmapManager.h"
#include "GameObjectManager.h"

CGround_BT::CGround_BT()
{}

CGround_BT::~CGround_BT()
{
    Release_GameObject();
}

int CGround_BT::Ready_GameObject()
{
    m_bIsDead = false;
    m_bIsActive = false;

    m_tInfo.iCX = 1878;
    m_tInfo.iCY = 200;

    return READY_OK;
}

int CGround_BT::Update_GameObject()
{
    if (true == m_bIsDead) { return OBJ_DEAD; }

    m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
    m_tInfo.fX -= m_fSpeed;

    return OBJ_NO_EVENT;
}

void CGround_BT::Late_Update_GameObject()
{
    if (m_tInfo.fX <= m_tInfo.iCX / -2) { m_bIsDead = true; }
}

void CGround_BT::Render_GameObject(HDC hDC)
{
    HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BonusTime_Ground");
    if (nullptr == hMemDC) { return; }
    GdiTransparentBlt
    (
        hDC,
        m_tRect.left,
        m_tRect.top,
        m_tInfo.iCX,
        m_tInfo.iCY,
        hMemDC,
        0,
        0,
        m_tInfo.iCX,
        m_tInfo.iCY,
        RGB(255, 0, 255)
    );
}

void CGround_BT::Release_GameObject()
{}

CGameObject* CGround_BT::Create(float fX, float fY)
{
    CGameObject* pInstance = new CGround_BT;

    if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

    pInstance->Set_Info(fX, fY);

    return pInstance;
}
