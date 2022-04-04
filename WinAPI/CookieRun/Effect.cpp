#include "pch.h"
#include "Effect.h"
#include "BitmapManager.h"
#include "GameObjectManager.h"

CEffect::CEffect()
    :m_bFireEffect(false)
    , m_bTwinkleEffect(false)
{}

CEffect::~CEffect()
{
    Release_GameObject();
}

int CEffect::Ready_GameObject()
{
    m_tInfo.iCX = 150;
    m_tInfo.iCY = 60;

    m_bIsDead = false;
    m_fSpeed = 15.f;

    m_bIsActive = true;
    m_bTemp = false;
    m_iAlpha = 255;

    m_bHeartEffect = false;

    m_tHeartInfo.iCX = 60;
    m_tHeartInfo.iCY = 60;

    m_bFireEffect = false;
    m_bTwinkleEffect = false;

    return READY_OK;
}

int CEffect::Update_GameObject()
{
    if (true == m_bIsDead) { return OBJ_DEAD; }

    if (m_eType == ITEM::TYPE::BLAST_JELLY)
    {
        m_tFrame.iFrameState = EFFECT::TYPE::BLAST;
    }
    else if (m_eType == ITEM::TYPE::MAGNET_JELLY)
    {
        m_tFrame.iFrameState = EFFECT::TYPE::MAGNET;
    }
    else if (m_eType == ITEM::TYPE::BIGGEST_JELLY)
    {
        m_tFrame.iFrameState = EFFECT::TYPE::BIGGEST;
    }
    else if (m_eType == ITEM::TYPE::ENERGY_JELLY)
    {
        m_tFrame.iFrameState = EFFECT::TYPE::HEAL;
        m_bHeartEffect = true;

        m_tHeartRect.left   = m_tHeartInfo.fX - (m_tHeartInfo.iCX / 2);
        m_tHeartRect.top    = m_tHeartInfo.fY - (m_tHeartInfo.iCY / 2);
        m_tHeartRect.right  = m_tHeartInfo.fX + (m_tHeartInfo.iCX / 2);
        m_tHeartRect.bottom = m_tHeartInfo.fY + (m_tHeartInfo.iCY / 2);
    }
    else if (m_eType == ITEM::TYPE::BIG_ENERGY_JELLY)
    {
        m_tFrame.iFrameState = EFFECT::TYPE::HEAL;
        m_bHeartEffect = true;

        m_tHeartRect.left   = m_tHeartInfo.fX - (m_tHeartInfo.iCX / 2);
        m_tHeartRect.top    = m_tHeartInfo.fY - (m_tHeartInfo.iCY / 2);
        m_tHeartRect.right  = m_tHeartInfo.fX + (m_tHeartInfo.iCX / 2);
        m_tHeartRect.bottom = m_tHeartInfo.fY + (m_tHeartInfo.iCY / 2);
    }
    else if (m_eType == ITEM::TYPE::BLAST_EFFECT)
    {
        m_bFireEffect = true;
    }
    else if (m_eType == ITEM::TYPE::JELLY_EFFECT)
    {
        m_bTwinkleEffect = true;
    }    

    if (m_bFireEffect == true)
    {
        m_tInfo.fX -= CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed() / 2;
    }
    else if (m_bTwinkleEffect == true)
    {
        m_tInfo.fX -= CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed() / 2;
    }
    else
    {
        float fAngle = atan2f((m_tHeartInfo.fY - 35), (m_tHeartInfo.fX - 291));

        m_tHeartInfo.fX -= 30 * cos(fAngle);
        m_tHeartInfo.fY -= 30 * sin(fAngle);
    }

    if (m_bTemp == false && m_bFireEffect == false && m_bTwinkleEffect == false)
    {
        m_tInfo.fX += m_fSpeed * cos(30 * 3.14 / 180);
        m_tInfo.fY -= m_fSpeed * sin(30 * 3.14 / 180);
    }
    else
    {
        m_tInfo.fX -= CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
        m_iAlpha -= 5;
    }  

    return OBJ_NO_EVENT;
}

void CEffect::Late_Update_GameObject()
{
    if (m_tEffectTime + 400 < GetTickCount64())
    {
        m_bTemp = true; 
    }

    if (m_tEffectTime + 1000 < GetTickCount64())
    {
        m_bIsDead = true;
        m_tEffectTime = GetTickCount64();
    }

    if (m_tHeartRender + 240 < GetTickCount64())
    {
        m_bHeartRender = false;
        m_tHeartRender = GetTickCount64();
    }
}

void CEffect::Render_GameObject(HDC hDC)
{
    if (m_bFireEffect == true)
    {
        HDC hMemDC_Fire = CBitmapManager::Get_Instance()->Find_Image(L"BlastEffect");
        if (nullptr == hMemDC_Fire) { return; }
        GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, 200, 141, hMemDC_Fire, 0, 0, 200, 141, RGB(255, 0, 255));
    }

    if (m_bTwinkleEffect == true)
    {
        HDC hMemDC_Jelly = CBitmapManager::Get_Instance()->Find_Image(L"JellyEffect");
        if (nullptr == hMemDC_Jelly) { return; }
        GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, 86, 46, hMemDC_Jelly, 0, 0, 86, 46, RGB(255, 0, 255));
    }

    if (m_bFireEffect == false)
    {
        BLENDFUNCTION	tBlend = {};
        tBlend.SourceConstantAlpha = 60;

        HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Text");
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
            m_tFrame.iFrameState * m_tInfo.iCY,
            m_tInfo.iCX,
            m_tInfo.iCY,
            RGB(255, 0, 255)
        );
    }

    if (m_bHeartEffect == true && m_bHeartRender == true)
    {
        m_tHeartRect.left = m_tHeartInfo.fX - (m_tHeartInfo.iCX / 2);
        m_tHeartRect.top = m_tHeartInfo.fY - (m_tHeartInfo.iCY / 2);
        m_tHeartRect.right = m_tHeartInfo.fX + (m_tHeartInfo.iCX / 2);
        m_tHeartRect.bottom = m_tHeartInfo.fY + (m_tHeartInfo.iCY / 2);

        HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Heart");
        if (nullptr == hMemDC) { return; }
        GdiTransparentBlt
        (
            hDC,
            m_tHeartRect.left,
            m_tHeartRect.top,
            m_tHeartInfo.iCX,
            m_tHeartInfo.iCY,
            hMemDC,
            0,
            0,
            m_tHeartInfo.iCX,
            m_tHeartInfo.iCY,
            RGB(255, 0, 255)
        );
    }
}

void CEffect::Release_GameObject()
{}

CGameObject* CEffect::Create(ITEM::TYPE eType, float fX, float fY)
{
    CGameObject* pInstance = new CEffect;

    if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

    pInstance->Set_Info(fX, fY);
    static_cast<CEffect*>(pInstance)->Set_HeartInfo(fX, fY);
    static_cast<CEffect*>(pInstance)->Set_ItemType(eType);

    return pInstance;
}
