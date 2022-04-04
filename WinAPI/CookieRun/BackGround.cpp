#include "pch.h"
#include "BackGround.h"

#include "GameObjectManager.h"
#include "BitmapManager.h"

CBackGround::CBackGround()
{}

CBackGround::~CBackGround()
{
	Release_GameObject();
}

int CBackGround::Ready_GameObject()
{
	m_bIsDead = false;
	m_bIsActive = true;

	m_tInfo.iCX = 2401;
	m_tInfo.iCY = 720;

	m_ePhase = PHASE::TYPE::_1ST;

	m_bFinish = false;

	return READY_OK;
}

int CBackGround::Update_GameObject()
{
	if (true == m_bIsDead) { return OBJ_DEAD; }

	if (m_bFinish == true)
	{
		m_tInfo.fX -= 0;
	}
	else if (m_bFinish == false)
	{
		if (m_eType == BACKGROUND::TYPE::BG_1)
		{
			m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
			m_tInfo.fX -= m_fSpeed / 10;
		}
		else if (m_eType == BACKGROUND::TYPE::BG_2)
		{
			m_fSpeed = CGameObjectManager::Get_Instance()->Get_Player()->Get_Speed();
			m_tInfo.fX -= m_fSpeed / 6;
		}
	}
	

	return OBJ_NO_EVENT;
}

void CBackGround::Late_Update_GameObject()
{
	if (m_tInfo.fX <= m_tInfo.iCX / -2) { m_bIsDead = true; }
}

void CBackGround::Render_GameObject(HDC hDC)
{
	if (m_ePhase == PHASE::TYPE::_1ST)
	{
		if (m_eType == BACKGROUND::TYPE::BG_1)
		{
			HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"BG_1");
			if (nullptr == hMemDC_BG) { return; }
			BitBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC_BG, 0, 0, SRCCOPY);
		}
		else if (m_eType == BACKGROUND::TYPE::BG_2)
		{
			HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"BG_2");
			if (nullptr == hMemDC_BG) { return; }
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC_BG, 0, 0, m_tInfo.iCX, m_tInfo.iCY, RGB(255, 0, 255));
		}
	}
	else if (m_ePhase == PHASE::TYPE::_2ND)
	{
		if (m_eType == BACKGROUND::TYPE::BG_1)
		{
			HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"BG2_1");
			if (nullptr == hMemDC_BG) { return; }
			BitBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC_BG, 0, 0, SRCCOPY);
		}
		else if (m_eType == BACKGROUND::TYPE::BG_2)
		{
			HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"BG2_2");
			if (nullptr == hMemDC_BG) { return; }
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC_BG, 0, 0, m_tInfo.iCX, m_tInfo.iCY, RGB(255, 0, 255));
		}
	}

	
}

void CBackGround::Release_GameObject()
{}

CGameObject* CBackGround::Create(BACKGROUND::TYPE eType, PHASE::TYPE ePhase, float fX, float fY)
{
	CGameObject* pInstance = new CBackGround;

	if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

	pInstance->Set_Info(fX, fY);
	static_cast<CBackGround*>(pInstance)->Set_BG_Type(eType);
	static_cast<CBackGround*>(pInstance)->Set_Phase(ePhase);

	return pInstance;
}
