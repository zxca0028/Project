#include "pch.h"
#include "Pet.h"
#include "Controller.h"
#include "GameObjectManager.h"
#include "BitmapManager.h"
#include "Player.h"
#include "CreateManager.h"

CPet::CPet()
    : m_bMagneticForce(false)
    , m_bPetOnOff(true)
    , m_tFrame({})
{}

CPet::~CPet()
{
    Release_GameObject();
}

int CPet::Ready_GameObject()
{
    m_bIsActive = true;
    m_bMagneticForce = false;
    m_bPetOnOff = true;

    m_tInfo.fX = 120;
    m_tInfo.fY = 460;

    m_tInfo.iCX = 180;
    m_tInfo.iCY = 180;

    m_tMagneticTime = GetTickCount64();

    // 프레임 기본 설정
    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 3;

    m_tFrame.dwFrameSpeed = 200;
    m_tFrame.dwFrameTime = GetTickCount64();

    m_bFinish = false;

    return READY_OK;
}

int CPet::Update_GameObject()
{
    m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed() * 2;

    if (m_bFinish == true)
    {
        m_tInfo.fX = CGameObjectManager::Get_Instance()->Get_Player()->Get_Info().fX - 80;
        m_tInfo.fY = CGameObjectManager::Get_Instance()->Get_Player()->Get_Rect().bottom - 120;
    }
    else if (m_bFinish == false)
    {
        if (m_bMagneticForce == false)
        {
            if (m_tInfo.fX > 125)
            {
                m_tInfo.fX -= m_fSpeed * cos(15 * 3.14 / 180);
                m_tInfo.fY += m_fSpeed * sin(15 * 3.14 / 180);
            }
            else
            {
                m_tInfo.fX = CGameObjectManager::Get_Instance()->Get_Player()->Get_Info().fX - 80;

                if (static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Get_Sliding())
                {
                    m_tInfo.fY = CGameObjectManager::Get_Instance()->Get_Player()->Get_Rect().bottom - 70;
                }
                else
                {
                    m_tInfo.fY = CGameObjectManager::Get_Instance()->Get_Player()->Get_Rect().bottom - 120;
                }
            }
        }
        else if (m_bMagneticForce == true)
        {
            if (m_tInfo.fX <= 500 || m_tInfo.fY >= 350)
            {
                m_tInfo.fX += m_fSpeed * cos(15 * 3.14 / 180);
                m_tInfo.fY -= m_fSpeed * sin(15 * 3.14 / 180);
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_P))
        {
            if (m_bPetOnOff == false)
            {
                m_bPetOnOff = true;
            }
            else if (m_bPetOnOff == true)
            {
                m_bPetOnOff = false;
            }
        }
    }

    m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX / 5.f));
    m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY / 5.f));
    m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX / 5.f));
    m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 5.f));

    return OBJ_NO_EVENT;
}

void CPet::Late_Update_GameObject()
{
    MoveFrame();

    if (m_tMagneticTime + MAGNET < GetTickCount64())
    {
        m_bMagneticForce = false;
        m_tMagneticTime = GetTickCount64();
    }
}

void CPet::Render_GameObject(HDC hDC)
{
    if (CCreateManager::Get_Instance()->Get_Cookie() == L"MoonlightCookie")
    {
        m_tFrame.iFrameState = 0;
    }
    else if (CCreateManager::Get_Instance()->Get_Cookie() == L"ChessChocoCookie")
    {
        m_tFrame.iFrameState = 1;
    }
    else if (CCreateManager::Get_Instance()->Get_Cookie() == L"BlackBerryCookie")
    {
        m_tFrame.iFrameState = 2;
    }
    else if (CCreateManager::Get_Instance()->Get_Cookie() == L"CherryCookie")
    {
        m_tFrame.iFrameState = 3;
    }
    else if (CCreateManager::Get_Instance()->Get_Cookie() == L"BraveCookie")
    {
        m_tFrame.iFrameState = 4;
    }

    if (m_bPetOnOff == true)
    {
        HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Pets");
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
            m_tFrame.iFrameState * m_tInfo.iCY,
            m_tInfo.iCX,
            m_tInfo.iCY,
            RGB(255, 0, 255)
        );
    }

    if (CGameObjectManager::Get_Instance()->Get_Player()->Get_CollisionView())
    {
        m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX / 5.f));
        m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY / 5.f));
        m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX / 5.f));
        m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 5.f));

        Rectangle(hDC, m_tCollisionRect.left, m_tCollisionRect.top, m_tCollisionRect.right, m_tCollisionRect.bottom);
    }
}

void CPet::Release_GameObject()
{
}

void CPet::MoveFrame()
{
    if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount64())
    {
        ++m_tFrame.iFrameStart;
        m_tFrame.dwFrameTime = GetTickCount64();
    }

    if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
    {
        m_tFrame.iFrameStart = 0;
    }
}

CGameObject* CPet::Create()
{
    CGameObject* pInstance = new CPet;
    
    if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

    return pInstance;
}