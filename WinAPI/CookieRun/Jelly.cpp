#include "pch.h"
#include "Jelly.h"
#include "Controller.h"
#include "GameObjectManager.h"
#include "CreateManager.h"
#include "BitmapManager.h"
#include "Player.h"
#include "Pet.h"
#include "Score.h"
#include "Effect.h"

CJelly::CJelly()
	: m_eType(JELLY::TYPE::END)
	, m_bRise(false)
{}

CJelly::~CJelly()
{
	Release_GameObject();
}

int CJelly::Ready_GameObject()
{
	m_bIsDead = false;
	m_bIsActive = true;

	m_tFrame.iFrameStart = 0;

	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount64();

	return READY_OK;
}

int CJelly::Update_GameObject()
{
	if (true == m_bIsDead) { return OBJ_DEAD; }

	if (CCreateManager::Get_Instance()->Get_Cookie() == L"BraveCookie")
	{
		m_fScale = 1.f;
	}
	else if (CCreateManager::Get_Instance()->Get_Cookie() == L"CherryCookie")
	{
		m_fScale = 1.618f;
	}
	else if (CCreateManager::Get_Instance()->Get_Cookie() == L"BlackBerryCookie")
	{
		m_fScale = 2.618f;
	}
	else if (CCreateManager::Get_Instance()->Get_Cookie() == L"ChessChocoCookie")
	{
		m_fScale = 4.235f;
	}
	else if (CCreateManager::Get_Instance()->Get_Cookie() == L"MoonlightCookie")
	{
		m_fScale = 6.853f;
	}


	if (static_cast<CPet*>(CGameObjectManager::Get_Instance()->Get_Pet())->Get_MagneticForce())
	{
		SizingObject();
		
		INFO	tPetInfo = CGameObjectManager::Get_Instance()->Get_Pet()->Get_Info();

		if (tPetInfo.fX - 100 > m_tInfo.fX)
		{
			m_bIsDead = true;
		}
		if (m_tInfo.fX <= tPetInfo.fX && m_tInfo.fX >= tPetInfo.fX - 100)
		{
			m_tInfo.fX = tPetInfo.fX;
		}

		if (m_tInfo.fX - tPetInfo.fX < 400)
		{
			float fAngle = atan2f((m_tInfo.fY - tPetInfo.fY),(m_tInfo.fX - tPetInfo.fX));
			m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();

			m_tInfo.fX -= m_fSpeed * cos(fAngle);
			m_tInfo.fY -= m_fSpeed * sin(fAngle);
		}
	}

	SizingObject();

	if (m_bRise == false)
	{
		m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
		m_tInfo.fX -= m_fSpeed;
	}
	else if (m_bRise == true)
	{
		m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
		m_tInfo.fX -= m_fSpeed;
		m_tInfo.fY -= 10.f;
	}

	return OBJ_NO_EVENT;
}

void CJelly::Late_Update_GameObject()
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

	// 플레이어, 펫 충돌 판단
	if (OnCollision())
	{ 
		m_bIsDead = true;
	}

	// 젤리 공통
	if (m_tInfo.fX <= m_tInfo.iCX / -2) 
	{ 
		m_bIsDead = true;
	}
}

void CJelly::Render_GameObject(HDC hDC)
{
	HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Jellies");
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
		m_tFrame.iFrameState,
		m_tInfo.iCX,
		m_tInfo.iCY,
		RGB(255, 0, 255)
	);

	if (CGameObjectManager::Get_Instance()->Get_Player()->Get_CollisionView())
	{
		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	}
}

void CJelly::Release_GameObject()
{
}

bool CJelly::OnCollision()
{
	RECT rc = {};
	RECT tPlayerCollisionRect = CGameObjectManager::Get_Instance()->Get_Player()->Get_CollisionRect();
	RECT tPetCollisionRect = CGameObjectManager::Get_Instance()->Get_Pet()->Get_CollisionRect();

	if (IntersectRect(&rc, &tPlayerCollisionRect, &m_tRect) || IntersectRect(&rc, &tPetCollisionRect, &m_tRect))
	{
		CSoundManager::Get_Instance()->PlaySound(L"Jelly.mp3", CSoundManager::CHANNELID::JELLY);
		CGameObject* pEffect = CEffect::Create(ITEM::TYPE::JELLY_EFFECT, m_tInfo.fX, m_tInfo.fY);
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::EFFECT, pEffect);
		
		if (m_eType == JELLY::TYPE::NORMAL)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_NORMAL_JELLY * m_fScale;
		}
		else if (m_eType == JELLY::TYPE::GOLD_BEAR)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_GOLD_BEAR * m_fScale;
		}
		else if (m_eType == JELLY::TYPE::PINK_BEAR)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_PINK_BEAR * m_fScale;
		}
		else if (m_eType == JELLY::TYPE::BIG_GOLD_BEAR)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_BIG_GOLD_BEAR * m_fScale;
		}
		else if (m_eType == JELLY::TYPE::BIG_PINK_BEAR)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_BIG_PINK_BEAR * m_fScale;
		}
		else if (m_eType == JELLY::TYPE::RAINBOW_BEAR)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_RAINBOW_BEAR * m_fScale;
		}
		else if (m_eType == JELLY::TYPE::SILVER_COIN)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_SILVER_COIN * m_fScale;
			CScore::m_iGetCoin += 1 * m_fScale;
		}
		else if (m_eType == JELLY::TYPE::BIG_SILVER_COIN)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_BIG_SILVER_COIN * m_fScale;
			CScore::m_iGetCoin += 20 * m_fScale;
		}
		else if (m_eType == JELLY::TYPE::GOLD_COIN)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_GOLD_COIN * m_fScale;
			CScore::m_iGetCoin += 20 * m_fScale;
		}
		else if (m_eType == JELLY::TYPE::BIG_GOLD_COIN)
		{
			CScore::m_iCurrentScore += SCORE::SCORE_BIG_GOLD_COIN * m_fScale;
			CScore::m_iGetCoin += 100 * m_fScale;
		}
		return true;
	}
	else
	{
		return false;
	}
}

void CJelly::SizingObject()
{
	if (m_eType == JELLY::TYPE::BIG_GOLD_BEAR)
	{
		m_tFrame.iFrameState = INDEX::BIG_GOLD_BEAR;
		m_tFrame.iFrameEnd = 7;

		m_tInfo.iCX = 150;
		m_tInfo.iCY = 140;
	}
	else if (m_eType == JELLY::TYPE::BIG_PINK_BEAR)
	{
		m_tFrame.iFrameState = INDEX::BIG_PINK_BEAR;
		m_tFrame.iFrameEnd = 7;

		m_tInfo.iCX = 150;
		m_tInfo.iCY = 140;
	}
	else if (m_eType == JELLY::TYPE::RAINBOW_BEAR)
	{
		m_tFrame.iFrameState = INDEX::RAINBOW_BEAR;
		m_tFrame.iFrameEnd = 7;

		m_tInfo.iCX = 180;
		m_tInfo.iCY = 130;
	}
	else if (m_eType == JELLY::TYPE::BIG_SILVER_COIN)
	{
		m_tFrame.iFrameState = INDEX::BIG_SILVER_COIN;
		m_tFrame.iFrameEnd = 3;

		m_tInfo.iCX = 160;
		m_tInfo.iCY = 160;
	}
	else if (m_eType == JELLY::TYPE::BIG_GOLD_COIN)
	{
		m_tFrame.iFrameState = INDEX::BIG_GOLD_COIN;
		m_tFrame.iFrameEnd = 3;

		m_tInfo.iCX = 160;
		m_tInfo.iCY = 160;
	}
	else if (m_eType == JELLY::TYPE::SILVER_COIN)
	{
		m_tFrame.iFrameState = INDEX::SILVER_COIN;
		m_tFrame.iFrameEnd = 3;

		m_tInfo.iCX = 60;
		m_tInfo.iCY = 60;
	}
	else if (m_eType == JELLY::TYPE::GOLD_COIN)
	{
		m_tFrame.iFrameState = INDEX::GOLD_COIN;
		m_tFrame.iFrameEnd = 3;

		m_tInfo.iCX = 60;
		m_tInfo.iCY = 60;
	}
	else if (m_eType == JELLY::TYPE::GOLD_BEAR)
	{
		m_tFrame.iFrameState = INDEX::GOLD_BEAR;
		m_tFrame.iFrameEnd = 0;

		m_tInfo.iCX = 60;
		m_tInfo.iCY = 60;
	}
	else if (m_eType == JELLY::TYPE::PINK_BEAR)
	{
		m_tFrame.iFrameState = INDEX::PINK_BEAR;
		m_tFrame.iFrameEnd = 0;

		m_tInfo.iCX = 60;
		m_tInfo.iCY = 60;
	}
	else if (m_eType == JELLY::TYPE::NORMAL)
	{
		m_tFrame.iFrameState = INDEX::NORMAL;
		m_tFrame.iFrameEnd = 0;

		m_tInfo.iCX = 40;
		m_tInfo.iCY = 40;
	}
	else if (m_eType == JELLY::TYPE::END)
	{
		m_bIsDead = true;
	}
}

CGameObject* CJelly::Create(JELLY::TYPE eType, float fX, float fY)
{
	CGameObject* pInstance = new CJelly;

	if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

	pInstance->Set_Info(fX, fY);
	static_cast<CJelly*>(pInstance)->Set_JellyType(eType);

	return pInstance;
}
