#include "pch.h"
#include "Ground.h"
#include "GameObjectManager.h"
#include "BitmapManager.h"

CGround::CGround()
	:m_bCreateJellyLock(false)
{}

CGround::~CGround()
{
	Release_GameObject();
}

int CGround::Ready_GameObject()
{
	m_bIsDead = false;
	m_bIsActive = true;
	m_bSet_IsObstacle = false;
	m_bSet_IsJelly = false;
	m_bSet_IsVoid = false;
	m_bSet_IsVoid2 = false;
	m_bSetJ2 = false;

	m_ePhase = PHASE::TYPE::_1ST;

	m_bFinish = false;

	return READY_OK;
}

int CGround::Update_GameObject()
{
	if (true == m_bIsDead) { return OBJ_DEAD; }

	if (m_eType == GROUND::TYPE::NORMAL)
	{
		m_iType = INDEX::NORMAL;

		m_tInfo.iCX = 496;
		m_tInfo.iCY = 140;
	}
	else if (m_eType == GROUND::TYPE::_VOID)
	{
		m_iType = INDEX::_VOID;

		m_tInfo.iCX = 248;
		m_tInfo.iCY = 140;
	}
	else if (m_eType == GROUND::TYPE::_VOID2)
	{
		m_iType = INDEX::_VOID2;

		m_tInfo.iCX = 496;
		m_tInfo.iCY = 140;
	}

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

void CGround::Late_Update_GameObject()
{
	if (m_tInfo.fX <= m_tInfo.iCX / -2) { m_bIsDead = true; }
}

void CGround::Render_GameObject(HDC hDC)
{
	if (m_ePhase == PHASE::TYPE::_1ST)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Ground");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC, 0, m_iType, m_tInfo.iCX, m_tInfo.iCY, RGB(255, 0, 255));
	}
	else if (m_ePhase == PHASE::TYPE::_2ND)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Ground2");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC, 0, m_iType, m_tInfo.iCX, m_tInfo.iCY, RGB(255, 0, 255));
	}
}

void CGround::Release_GameObject()
{}

CGameObject* CGround::Create(GROUND::TYPE eType, PHASE::TYPE ePhase, float fX, float fY)
{
	CGameObject* pInstance = new CGround;

	if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

	pInstance->Set_Info(fX, fY);
	static_cast<CGround*>(pInstance)->Set_GroundType(eType);
	static_cast<CGround*>(pInstance)->Set_Phase(ePhase);


	return pInstance;
}
