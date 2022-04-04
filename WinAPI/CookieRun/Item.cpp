#include "pch.h"
#include "Item.h"
#include "GameObjectManager.h"
#include "BitmapManager.h"

#include "Player.h"
#include "Pet.h"
#include "UI.h"
#include "Effect.h"

CItem::CItem()
	: m_eType(ITEM::TYPE::BLAST_EFFECT)
	, m_iFrameStart(0)
	, m_tFrame({})
	, m_tBonusTimeIntro(0)
{}

CItem::~CItem()
{
	Release_GameObject();
}

int CItem::Ready_GameObject()
{
	m_bIsDead = false;
	m_bIsActive = true;

	// 프레임 기본 설정
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;

	m_tFrame.dwFrameSpeed = 150;
	m_tFrame.dwFrameTime = GetTickCount64();

	m_tBonusTimeIntro = GetTickCount64();

	return READY_OK;
}

int CItem::Update_GameObject()
{
	if (true == m_bIsDead) { return OBJ_DEAD; }

	if (static_cast<CPet*>(CGameObjectManager::Get_Instance()->Get_Pet())->Get_MagneticForce())
	{
		SizingObject();

		INFO	tPetInfo = CGameObjectManager::Get_Instance()->Get_Pet()->Get_Info();

		if (tPetInfo.fX - 100> m_tInfo.fX)		
		{
			m_bIsDead = true;
		}
		if (m_tInfo.fX <= tPetInfo.fX && m_tInfo.fX >= tPetInfo.fX - 100)
		{
			m_tInfo.fX = tPetInfo.fX;
		}

		if (m_tInfo.fX - tPetInfo.fX < 400)
		{
			float fAngle = atan2f((m_tInfo.fY - tPetInfo.fY) , (m_tInfo.fX - tPetInfo.fX));
			m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();

			m_tInfo.fX -= m_fSpeed * cos(fAngle);
			m_tInfo.fY -= m_fSpeed * sin(fAngle);
		}
	}

	SizingObject();

	// 아이템 공통
	// 아이템 이동속도
	m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
	m_tInfo.fX -= m_fSpeed;

	// 충돌체 업데이트
	m_tCollisionRect.left = LONG(m_tInfo.fX - (m_tInfo.iCX >> 2));
	m_tCollisionRect.top = LONG(m_tInfo.fY - (m_tInfo.iCY >> 2));
	m_tCollisionRect.right = LONG(m_tInfo.fX + (m_tInfo.iCX >> 2));
	m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY >> 2));

	return OBJ_NO_EVENT;
}

void CItem::Late_Update_GameObject()
{
	MoveFrame();

	// 플레이어와 충돌 판단
	if (OnCollision()) { m_bIsDead = true; }

	// 아이템 공통
	if (m_tInfo.fX <= m_tInfo.iCX / -2) { m_bIsDead = true; }
}

void CItem::Render_GameObject(HDC hDC)
{
	if (m_eType == ITEM::TYPE::B || m_eType == ITEM::TYPE::O || m_eType == ITEM::TYPE::N || m_eType == ITEM::TYPE::U || m_eType == ITEM::TYPE::S ||
		m_eType == ITEM::TYPE::T || m_eType == ITEM::TYPE::I || m_eType == ITEM::TYPE::M || m_eType == ITEM::TYPE::E)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BonusTime_Item");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt
		(
			hDC,
			m_tRect.left,
			m_tRect.top,
			m_tInfo.iCX,
			m_tInfo.iCY,
			hMemDC,
			m_iFrameStart * m_tInfo.iCX,
			0,
			m_tInfo.iCX,
			m_tInfo.iCY,
			RGB(255, 0, 255)
		);
	}
	else if (m_eType != ITEM::TYPE::BIG_ENERGY_JELLY)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Items");
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
	else if (m_eType == ITEM::TYPE::BIG_ENERGY_JELLY)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Big_HP");
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
		m_tCollisionRect.left = LONG(m_tInfo.fX - (m_tInfo.iCX >> 2));
		m_tCollisionRect.top = LONG(m_tInfo.fY - (m_tInfo.iCY >> 2));
		m_tCollisionRect.right = LONG(m_tInfo.fX + (m_tInfo.iCX >> 2));
		m_tCollisionRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY >> 2));

		Rectangle(hDC, m_tCollisionRect.left, m_tCollisionRect.top, m_tCollisionRect.right, m_tCollisionRect.bottom);
	}
}

void CItem::Release_GameObject()
{}

void CItem::MoveFrame()
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

void CItem::SizingObject()
{
	// 아이템 타입에 따른 리소스 크기 설정
	if (m_eType == ITEM::TYPE::BLAST_JELLY)
	{
		m_tInfo.iCX = 90;
		m_tInfo.iCY = 90;
		m_tFrame.iFrameState = ITEM::TYPE::BLAST_JELLY;
	}
	else if (m_eType == ITEM::TYPE::MAGNET_JELLY)
	{
		m_tInfo.iCX = 90;
		m_tInfo.iCY = 90;
		m_tFrame.iFrameState = ITEM::TYPE::MAGNET_JELLY;
	}
	else if (m_eType == ITEM::TYPE::BIGGEST_JELLY)
	{
		m_tInfo.iCX = 90;
		m_tInfo.iCY = 90;
		m_tFrame.iFrameState = ITEM::TYPE::BIGGEST_JELLY;
	}
	else if (m_eType == ITEM::TYPE::ENERGY_JELLY)
	{
		m_tInfo.iCX = 90;
		m_tInfo.iCY = 90;
		m_tFrame.iFrameState = ITEM::TYPE::ENERGY_JELLY;

	}
	else if (m_eType == ITEM::TYPE::BIG_ENERGY_JELLY)
	{
		m_tInfo.iCX = 144;
		m_tInfo.iCY = 144;
		m_tFrame.iFrameState = 0;
	}
	else if (m_eType == ITEM::TYPE::B)
	{
		m_tInfo.iCX = 42;
		m_tInfo.iCY = 46;
		m_iFrameStart = 0;
	}
	else if (m_eType == ITEM::TYPE::O)
	{
		m_tInfo.iCX = 42;
		m_tInfo.iCY = 46;
		m_iFrameStart = 1;
	}
	else if (m_eType == ITEM::TYPE::N)
	{
		m_tInfo.iCX = 42;
		m_tInfo.iCY = 46;
		m_iFrameStart = 2;
	}
	else if (m_eType == ITEM::TYPE::U)
	{
		m_tInfo.iCX = 42;
		m_tInfo.iCY = 46;
		m_iFrameStart = 3;
	}
	else if (m_eType == ITEM::TYPE::S)
	{
		m_tInfo.iCX = 42;
		m_tInfo.iCY = 46;
		m_iFrameStart = 4;
	}
	else if (m_eType == ITEM::TYPE::T)
	{
		m_tInfo.iCX = 42;
		m_tInfo.iCY = 46;
		m_iFrameStart = 5;
	}
	else if (m_eType == ITEM::TYPE::I)
	{
		m_tInfo.iCX = 42;
		m_tInfo.iCY = 46;
		m_iFrameStart = 6;
	}
	else if (m_eType == ITEM::TYPE::M)
	{
		m_tInfo.iCX = 42;
		m_tInfo.iCY = 46;
		m_iFrameStart = 7;
	}
	else if (m_eType == ITEM::TYPE::E)
	{
		m_tInfo.iCX = 42;
		m_tInfo.iCY = 46;
		m_iFrameStart = 8;
	}
	else if (m_eType == ITEM::TYPE::BLAST_EFFECT)
	{
		m_bIsDead = true;
	}
}

bool CItem::OnCollision()
{
	RECT rc = {};
	RECT tPlayerCollisionRect = CGameObjectManager::Get_Instance()->Get_Player()->Get_CollisionRect();
	RECT tPetCollisionRect = CGameObjectManager::Get_Instance()->Get_Pet()->Get_CollisionRect();

	if (IntersectRect(&rc, &tPlayerCollisionRect, &m_tCollisionRect) || IntersectRect(&rc, &tPetCollisionRect, &m_tCollisionRect))
	{
		CSoundManager::Get_Instance()->PlaySound(L"Item.mp3", CSoundManager::CHANNELID::ITEM);
		if (m_eType == ITEM::TYPE::BLAST_JELLY)
		{
			static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Set_Blasting();

			CGameObject* pBlastEffect = CEffect::Create(m_eType, m_tInfo.fX, m_tInfo.fY);
			if (nullptr == pBlastEffect) { return false; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::EFFECT, pBlastEffect);
		}
		if (m_eType == ITEM::TYPE::BIGGEST_JELLY)
		{
			static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Set_Biggest();

			CGameObject* pBiggestEffect = CEffect::Create(m_eType, m_tInfo.fX, m_tInfo.fY);
			if (nullptr == pBiggestEffect) { return false; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::EFFECT, pBiggestEffect);
		}
		if (m_eType == ITEM::TYPE::MAGNET_JELLY)
		{
			static_cast<CPet*>(CGameObjectManager::Get_Instance()->Get_Pet())->Set_MagneticForce();

			CGameObject* pMagnetEffect = CEffect::Create(m_eType, m_tInfo.fX, m_tInfo.fY);
			if (nullptr == pMagnetEffect) { return false; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::EFFECT, pMagnetEffect);
		}
		if (m_eType == ITEM::TYPE::ENERGY_JELLY)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Set_PlusHP(ITEM::TYPE::ENERGY_JELLY);

			CGameObject* pEnergyEffect = CEffect::Create(m_eType, m_tInfo.fX, m_tInfo.fY);
			if (nullptr == pEnergyEffect) { return false; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::EFFECT, pEnergyEffect);
		}
		if (m_eType == ITEM::TYPE::BIG_ENERGY_JELLY)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Set_PlusHP(ITEM::TYPE::BIG_ENERGY_JELLY);

			CGameObject* pBigEnergyEffect = CEffect::Create(m_eType, m_tInfo.fX, m_tInfo.fY);
			if (nullptr == pBigEnergyEffect) { return false; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::EFFECT, pBigEnergyEffect);
		}

		if (m_eType == ITEM::TYPE::B)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Get_BonusTime()->B = true;
		}
		if (m_eType == ITEM::TYPE::O)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Get_BonusTime()->O = true;
		}
		if (m_eType == ITEM::TYPE::N)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Get_BonusTime()->N = true;
		}
		if (m_eType == ITEM::TYPE::U)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Get_BonusTime()->U = true;
		}
		if (m_eType == ITEM::TYPE::S)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Get_BonusTime()->S = true;
		}
		if (m_eType == ITEM::TYPE::T)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Get_BonusTime()->T = true;
		}
		if (m_eType == ITEM::TYPE::I)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Get_BonusTime()->I = true;
		}
		if (m_eType == ITEM::TYPE::M)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Get_BonusTime()->M = true;
		}
		if (m_eType == ITEM::TYPE::E)
		{
			static_cast<CUI*>(CGameObjectManager::Get_Instance()->Get_BonusTime())->Get_BonusTime()->E = true;
		}

		return true;
	}
	else
	{
		return false;
	}
}

CGameObject* CItem::Create(ITEM::TYPE eType, float fX, float fY)
{
	CGameObject* pInstance = new CItem;

	if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

	pInstance->Set_Info(fX, fY);
	static_cast<CItem*>(pInstance)->Set_ItemType(eType);

	return pInstance;
}
