#include "pch.h"
#include "Obstacles.h"
#include "GameObjectManager.h"
#include "BitmapManager.h"
#include "Player.h"
#include "UI.h"

CObstacles::CObstacles()
	:m_eType(OBSTACLE::TYPE::END)
{}

CObstacles::~CObstacles()
{
	Release_GameObject();
}

int CObstacles::Ready_GameObject()
{
	m_bIsDead = false;
	m_bIsActive = true;
	m_bCrash = false;

	m_tCollisionTime = GetTickCount64();

	return READY_OK;
}

int CObstacles::Update_GameObject()
{
	if (true == m_bIsDead) { return OBJ_DEAD; }

	// 장애물 타입에 따른 리소스 크기 설정
	if (m_eType == OBSTACLE::BOTTOM_1_J1)
	{
		m_tInfo.iCX = 87;
		m_tInfo.iCY = 118;
		m_iState = 0;
	}
	else if (m_eType == OBSTACLE::BOTTOM_2_J1)
	{
		m_tInfo.iCX = 87;
		m_tInfo.iCY = 118;
		m_iState = 118;
	}
	else if (m_eType == OBSTACLE::BOTTOM_1_J2)
	{
		m_tInfo.iCX = 125;
		m_tInfo.iCY = 227;
		m_iState = 236;
	}
	else if (m_eType == OBSTACLE::BOTTOM_2_J2)
	{
		m_tInfo.iCX = 125;
		m_tInfo.iCY = 227;
		m_iState = 463;
	}
	else if (m_eType == OBSTACLE::TOP_1)
	{
		m_tInfo.iCX = 182;
		m_tInfo.iCY = 544;
		m_iState = 690;
	}
	else if (m_eType == OBSTACLE::TOP_2)
	{
		m_tInfo.iCX = 182;
		m_tInfo.iCY = 544;
		m_iState = 1234;
	}
	else if (m_eType == OBSTACLE::TYPE::END)
	{
		m_bIsDead = true;
	}

	// 장애물 공통
	// 장애물 이동속도

	if (m_bCrash == true)
	{
		m_tInfo.fX += 50 * cosf(30 * DEG_TO_RAD);
		m_tInfo.fY -= 25 * sinf(30 * DEG_TO_RAD);
	}
	else if (m_bCrash == false)
	{
		m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
		m_tInfo.fX -= m_fSpeed;
	}
	

	// 충돌체 업데이트
	if (m_eType == OBSTACLE::BOTTOM_1_J1 || m_eType == OBSTACLE::BOTTOM_2_J1)
	{
		m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX / 4.f));
		m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY / 3.5f));
		m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX / 4.f));
		m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 2.f));
	}
	else if (m_eType == OBSTACLE::BOTTOM_1_J2 || m_eType == OBSTACLE::BOTTOM_2_J2)
	{
		m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX / 3.5f));
		m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY / 2.5f));
		m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX / 3.8f));
		m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 2));
	}
	else if (m_eType == OBSTACLE::TOP_1 || m_eType == OBSTACLE::TOP_2)
	{
		m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX >> 2));
		m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY >> 2));
		m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX >> 2));
		m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 2.f));
	}
	else if (m_eType == OBSTACLE::TOP_3)
	{
		m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX / 2.3f));
		m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY >> 2));
		m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX / 2.3f));
		m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 2.f));
	}

	return OBJ_NO_EVENT;

}

void CObstacles::Late_Update_GameObject()
{
	OnCollision();

	// 장애물 공통
	if (m_tInfo.fX <= -m_tInfo.iCX / 2) { m_bIsDead = true; }
	if (m_bCrash == true)
	{
		if (m_tInfo.fX >= 1500.f) { m_bIsDead = true; }
	}
	
}

void CObstacles::Render_GameObject(HDC hDC)
{
	if (CGameObjectManager::Get_Instance()->Get_Player()->Get_CollisionView())
	{
		if (m_eType == OBSTACLE::BOTTOM_1_J1 || m_eType == OBSTACLE::BOTTOM_2_J1)
		{
			m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX / 4.f));
			m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY / 3.5f));
			m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX / 4.f));
			m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 2.f));
		}
		else if (m_eType == OBSTACLE::BOTTOM_1_J2 || m_eType == OBSTACLE::BOTTOM_2_J2)
		{
			m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX / 3.5f));
			m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY / 2.5f));
			m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX / 3.8f));
			m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 2));
		}
		else if (m_eType == OBSTACLE::TOP_1 || m_eType == OBSTACLE::TOP_2)
		{
			m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX >> 2));
			m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY >> 2));
			m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX >> 2));
			m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 2.f));
		}
		else if (m_eType == OBSTACLE::TOP_3)
		{
			m_tCollisionRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX / 2.3f));
			m_tCollisionRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY >> 2));
			m_tCollisionRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX / 2.3f));
			m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY / 2.f));
		}

		Rectangle(hDC, m_tCollisionRect.left, m_tCollisionRect.top, m_tCollisionRect.right, m_tCollisionRect.bottom);
	}

	if (m_ePhase == PHASE::TYPE::_1ST)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Obstacles_1");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC, 0, m_iState, m_tInfo.iCX, m_tInfo.iCY, RGB(255, 0, 255));
	}
	else if (m_ePhase == PHASE::TYPE::_2ND)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Obstacles_2");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC, 0, m_iState, m_tInfo.iCX, m_tInfo.iCY, RGB(255, 0, 255));
	}
}

void CObstacles::Release_GameObject()
{
}

void CObstacles::OnCollision()
{
	RECT rc = {};
	RECT tPlayerCollisionRect = CGameObjectManager::Get_Instance()->Get_Player()->Get_CollisionRect();
	bool bBiggest = static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Get_Biggest();
	bool bBlast = static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Get_Blasting();
	bool bInvincible = static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Get_Invincible();

	if (IntersectRect(&rc, &tPlayerCollisionRect, &m_tCollisionRect))
	{		
		if (bInvincible == false)
		{
			if (bBiggest == true || bBlast == true)
			{
				CSoundManager::Get_Instance()->PlaySound(L"Crash.mp3", CSoundManager::CHANNELID::CRASH);
				m_bCrash = true;
			}
			else
			{
				if (m_tCollisionTime + 1000 < GetTickCount64())
				{
					CSoundManager::Get_Instance()->PlaySound(L"Crash.mp3", CSoundManager::CHANNELID::CRASH);
					static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Set_Collision();
					static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_UI()->front())->Set_MinusHP();
					m_tCollisionTime = GetTickCount64();
				}
			}
		}				
	}	
}

CGameObject* CObstacles::Create(OBSTACLE::TYPE eType, PHASE::TYPE ePhase, float fX, float fY)
{
	CGameObject* pInstance = new CObstacles;

	if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

	pInstance->Set_Info(fX, fY);
	static_cast<CObstacles*>(pInstance)->Set_IsObstacleType(eType);
	static_cast<CObstacles*>(pInstance)->Set_Phase(ePhase);

	return pInstance;
}
