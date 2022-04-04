#include "pch.h"
#include "Stage.h"
#include "Controller.h"

#include "SceneManager.h"
#include "BitmapManager.h"
#include "GameObjectManager.h"
#include "CreateManager.h"
#include "SoundManager.h"

#include "Player.h"
#include "Ground.h"
#include "Ground_BT.h"
#include "BackGround.h"
#include "BackGround_BT.h"
#include "Obstacles.h"
#include "Item.h"
#include "Jelly.h"
#include "UI.h"
#include "Pet.h"
#include "Score.h"
#include "Result.h"
#include "Flag.h"

CStage::CStage()
	: m_bIsPlayerDead(false)
	, m_bIsBonusTime(false)
	, m_tBonusTime(0)
	, m_iDistance(400)
	, m_bFeverTime(false)
	, m_bLock(false)
{
}

CStage::~CStage()
{
	Release_Scene();
}

int CStage::Ready_Scene()
{
	if (CCreateManager::Get_Instance()->Get_Phase() == PHASE::TYPE::_1ST)
	{
		CSoundManager::Get_Instance()->PlayBGM(L"PHASE_1.mp3");
	}
	else if (CCreateManager::Get_Instance()->Get_Phase() == PHASE::TYPE::_2ND)
	{
		CSoundManager::Get_Instance()->PlayBGM(L"PHASE_2.mp3");
	}

	m_tFeverTimeStart = GetTickCount64();	

	if (0 > CCreateManager::Get_Instance()->Create_Objects())
	{
		return READY_FAILED;
	}
	else
	{
		return READY_OK;
	}
}

void CStage::Update_Scene()
{
	if (m_iFeverCount > 4)
	{
		if (m_bFlagLock == false)
		{
			CGameObject* pFlag = CFlag::Create(1500, 335);
			if (nullptr == pFlag) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::FLAG, pFlag);
			CSoundManager::Get_Instance()->PlaySound(L"Flag.mp3", CSoundManager::CHANNELID::FLAG);
			m_bFlagLock = true;
		}		

		float fPlayerRight = CGameObjectManager::Get_Instance()->Get_Player()->Get_Rect().right;
		float fFlagLeft = CGameObjectManager::Get_Instance()->Get_Flag()->Get_Rect().left;

		if (fPlayerRight >= fFlagLeft + 200)
		{
			CGameObjectManager::Get_Instance()->Get_Player()->Set_Finish();
			CGameObjectManager::Get_Instance()->Get_Flag()->Set_Finish();
			CGameObjectManager::Get_Instance()->Get_Pet()->Set_Finish();

			for (auto pGround : *(CGameObjectManager::Get_Instance()->Get_Ground()))
			{
				pGround->Set_Finish();
			}
			for (auto pBackGround : *(CGameObjectManager::Get_Instance()->Get_BackGround()))
			{
				pBackGround->Set_Finish();
			}
			for (auto pBackGround_2 : *(CGameObjectManager::Get_Instance()->Get_BackGround_2()))
			{
				pBackGround_2->Set_Finish();
			}
		}
	}

	Cheat();

	if (m_tFeverTimeStart + 28000 < GetTickCount64())
	{
		m_bFeverTime = true;

		if (m_bLock == false)
		{
			m_tFeverTimeEnd = GetTickCount64();
			m_bLock = true;
		}		

		if (m_tFeverTimeEnd + 5000 < GetTickCount64())
		{
			++m_iFeverCount;
			m_bFeverTime = false;
			m_bLock = false;
			m_tFeverTimeStart = GetTickCount64();
			m_tFeverTimeEnd = GetTickCount64();
		}
	}

	if (m_iFeverCount <= 4)
	{
		if (m_bFeverTime == true)
		{
			CCreateManager::Get_Instance()->ReCreate_BG_Ground(m_iFeverCount);
		}
		else if (m_bFeverTime == false)
		{
			CCreateManager::Get_Instance()->ReCreate_All_Objects();
		}
	}
	else if (m_iFeverCount > 4)
	{
		CCreateManager::Get_Instance()->ReCreateOnlyGround();
	}
	

	CGameObjectManager::Get_Instance()->Update_GameObjectManager();	

	if (static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Get_Blasting() == true)
	{
		m_iDistance = 200;
	}
	else
	{
		m_iDistance = 400;
	}

	if (true == m_bIsBonusTime)
	{
		if (m_tBonusTime + m_iDistance < GetTickCount64())
		{
			CCreateManager::Get_Instance()->Create_BonusTime_Objects();
			m_tBonusTime = GetTickCount64();
		}		
	}

	if (true == m_bIsPlayerDead)
	{
		m_bIsPlayerDead = false;
		CScore::m_bFirstCycle = true;
	
		if (CScore::m_iHiScoreBuffer <= CScore::m_iCurrentScore)
		{
			CScore::m_iHiScoreBuffer = CScore::m_iCurrentScore;
		}
		
		CSceneManager::Get_Instance()->Scene_Change(SCENE::TYPE::RESULT);
	}
	
}

void CStage::Render_Scene(HDC hDC)
{
	CGameObjectManager::Get_Instance()->Render_GameObjectManager(hDC);
}

void CStage::Release_Scene()
{
	m_iFeverCount = 0;
	CGameObjectManager::Get_Instance()->Delete_All_Object();

	CSoundManager::Get_Instance()->StopSound(CSoundManager::CHANNELID::BGM);
}

void CStage::Cheat()
{
	// BONUSTIME 생성치트
	if (CKeyManager::Get_Instance()->Key_Down(KEY_1))
	{
		CGameObject* pBONUSTIMEObj = CItem::Create(ITEM::TYPE::B, 1300, 500);
		if (nullptr == pBONUSTIMEObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBONUSTIMEObj);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_2))
	{
		CGameObject* pBONUSTIMEObj1 = CItem::Create(ITEM::TYPE::O, 1300, 500);
		if (nullptr == pBONUSTIMEObj1) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBONUSTIMEObj1);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_3))
	{
		CGameObject* pBONUSTIMEObj2 = CItem::Create(ITEM::TYPE::N, 1300, 500);
		if (nullptr == pBONUSTIMEObj2) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBONUSTIMEObj2);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_4))
	{
		CGameObject* pBONUSTIMEObj3 = CItem::Create(ITEM::TYPE::U, 1300, 500);
		if (nullptr == pBONUSTIMEObj3) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBONUSTIMEObj3);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_5))
	{
		CGameObject* pBONUSTIMEObj4 = CItem::Create(ITEM::TYPE::S, 1300, 500);
		if (nullptr == pBONUSTIMEObj4) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBONUSTIMEObj4);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_6))
	{
		CGameObject* pBONUSTIMEObj5 = CItem::Create(ITEM::TYPE::T, 1300, 500);
		if (nullptr == pBONUSTIMEObj5) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBONUSTIMEObj5);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_7))
	{
		CGameObject* pBONUSTIMEObj6 = CItem::Create(ITEM::TYPE::I, 1300, 500);
		if (nullptr == pBONUSTIMEObj6) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBONUSTIMEObj6);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_8))
	{
		CGameObject* pBONUSTIMEObj7 = CItem::Create(ITEM::TYPE::M, 1300, 500);
		if (nullptr == pBONUSTIMEObj7) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBONUSTIMEObj7);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_9))
	{
		CGameObject* pBONUSTIMEObj8 = CItem::Create(ITEM::TYPE::E, 1300, 500);
		if (nullptr == pBONUSTIMEObj8) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBONUSTIMEObj8);
	}

	// 아이템젤리 생성치트
	if (CKeyManager::Get_Instance()->Key_Down(KEY_F2))
	{
		CGameObject* pMagneticJelly = CItem::Create(ITEM::TYPE::MAGNET_JELLY, 1200, 500);
		if (nullptr == pMagneticJelly) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pMagneticJelly);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_F3))
	{
		CGameObject* pBlastJelly = CItem::Create(ITEM::TYPE::BLAST_JELLY, 1200, 500);
		if (nullptr == pBlastJelly) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBlastJelly);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_F4))
	{
		CGameObject* pBiggestJelly = CItem::Create(ITEM::TYPE::BIGGEST_JELLY, 1200, 500);
		if (nullptr == pBiggestJelly) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBiggestJelly);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_F5))
	{
		CGameObject* pHPJelly = CItem::Create(ITEM::TYPE::ENERGY_JELLY, 1200, 500);
		if (nullptr == pHPJelly) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pHPJelly);
	}
	if (CKeyManager::Get_Instance()->Key_Down(KEY_F6))
	{
		CGameObject* pBigHPJelly = CItem::Create(ITEM::TYPE::BIG_ENERGY_JELLY, 1200, 500);
		if (nullptr == pBigHPJelly) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBigHPJelly);
	}
}

CScene* CStage::Create()
{
	CScene* pInstance = new CStage;

	if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

	return pInstance;
}
