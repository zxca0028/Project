#include "pch.h"
#include "CreateManager.h"

#include "GameObjectManager.h"

#include "GameObject.h"
#include "Player.h"
#include "Pet.h"
#include "Ground.h"
#include "BackGround.h"
#include "Ground_BT.h"
#include "BackGround_BT.h"
#include "UI.h"
#include "Score.h"
#include "Jelly.h"
#include "Obstacles.h"
#include "Item.h"

CCreateManager* CCreateManager::m_pInstance = nullptr;

random_device rd;
mt19937 gen(rd());

// 장애물 랜덤생성 난수 : 1,2 = 1단 장애물 | 3,4 = 2단 장애물 | 5,6 = 천장 장애물
uniform_int_distribution<int> Obstacles(1, 6);

// 크기가 1 또는 3인 빈 바닥 생성 난수
uniform_int_distribution<int> Ground_VOID(1, 10);
uniform_int_distribution<int> Ground_VOID2(1, 10);

// 대사 랜덤 재생
uniform_int_distribution<int> Sentence(1, 10);


CCreateManager::CCreateManager()
	: m_bCreateLock(false)
	, m_bCreateScoreLock(false)
	, m_bCreateJellyLock(false)
	, m_bCreateJellyLock_FeverTime(false)
	, m_bCreateBlastLock(false)
	, m_iCreateBlastLock(0)
{
}

CCreateManager::~CCreateManager()
{
}

CCreateManager* CCreateManager::Get_Instance()
{
	if (nullptr == m_pInstance) { m_pInstance = new CCreateManager; }
	return m_pInstance;
}

void CCreateManager::Destroy_Instance()
{
	if (nullptr != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

int CCreateManager::Create_Objects()
{
	// 플레이어 생성
	CGameObject* pPlayerObj = CPlayer::Create(m_pCookie);
	if (nullptr == pPlayerObj) { return READY_FAILED; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::PLAYER, pPlayerObj);

	// 펫 생성
	CGameObject* pPetObj = CPet::Create();
	if (nullptr == pPetObj) { return READY_FAILED; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::PET, pPetObj);

	// 바닥 생성
	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, 248 * (float)(2 * i), 650);
		if (nullptr == pGroundObj) { return READY_FAILED; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);
	}
	// 배경 생성
	for (int i = 0; i < 2; ++i)
	{
		CGameObject* pBackGroundObj = CBackGround::Create(BACKGROUND::TYPE::BG_1, m_ePhase, 640 + (float)(2401 * i), 360);
		if (nullptr == pBackGroundObj) { return READY_FAILED; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BACKGROUND, pBackGroundObj);
	}
	for (int i = 0; i < 2; ++i)
	{
		CGameObject* pBackGroundObj = CBackGround::Create(BACKGROUND::TYPE::BG_2, m_ePhase, + (float)(2401 * i), 360);
		if (nullptr == pBackGroundObj) { return READY_FAILED; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BACKGROUND_2, pBackGroundObj);
	}

	// 보너스타임 바닥 생성
	for (int i = 0; i < 12; ++i)
	{
		CGameObject* pBTGround = CGround_BT::Create(812 + (float)(1878 * i), 620);
		if (nullptr == pBTGround) { return READY_FAILED; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BT_GROUND, pBTGround);
	}
	// 보너스타임 배경 생성
	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pBTObj = CBackGround_BT::Create(939 + (float)(1878 * i), 360);
		if (nullptr == pBTObj) { return READY_FAILED; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BT_BACKGROUND, pBTObj);
	}

	// UI 생성
	CGameObject* pUIObj_BT = CUI::Create();
	if (nullptr == pUIObj_BT) { return READY_FAILED; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::UI, pUIObj_BT);

	if (m_bCreateScoreLock == false)
	{
		// 스코어보드 생성
		CGameObject* pScoreObj = CScore::Create();
		if (nullptr == pScoreObj) { return READY_FAILED; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::SCORE, pScoreObj);
		m_bCreateScoreLock = true;
	}

	StartJelly();

	return READY_OK;
}

void CCreateManager::ReCreate_All_Objects()
{
	m_bCreateJellyLock_FeverTime = false;

	if (TestOnOff == 1)
	{
		// 실험용 바닥 재생
		if (CGameObjectManager::Get_Instance()->Get_Ground()->size() < 7)
		{
			float fX = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_Info().fX;

			CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 372, 650);
			if (nullptr == pGroundObj) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);
		}
	}
	else if (TestOnOff == 0)
	{
		// 실전용 바닥 재생	
		if (CGameObjectManager::Get_Instance()->Get_Ground()->size() < 7)
		{
			// 마지막 바닥 "다음의 바닥을 생성" 하기 위해 마지막 바닥 정보를 가져온다 ( 마지막 바닥의 X좌표와 바닥 타입 )
			GROUND::TYPE eType = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_GroundType();
			float fX = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_Info().fX;
			bool bIsJ2 = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_IsJ2();
			bool bIsObstacleSeries = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_IsObstacle();

			// 빈 바닥을 생성할건데
			if (Ground_VOID(gen) > 3)
			{
				// 어느 크기의 빈바닥을 생성할꺼냐면
				// 크기 2의 빈 바닥을 생성할꺼야
				if (Ground_VOID2(gen) > 6)
				{
					// 생성할 위치 전의 바닥이 일반 바닥일 때에만, 또 크기2 장애물이 없는 바닥일 때에만 크기2 빈바닥을 생성할께
					if (eType == GROUND::TYPE::NORMAL /*&& false == bIsJ2*/)
					{
						CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::_VOID2, m_ePhase, fX + 496, 650);
						if (nullptr == pGroundObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

						// 크기2 빈 바닥 인증
						static_cast<CGround*>(pGroundObj)->Set_IsVoid2();
					}
					// 전 바닥이 크기1 빈바닥이면 일반 바닥을 생성할께
					else if (eType == GROUND::TYPE::_VOID)
					{
						CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 372, 650);
						if (nullptr == pGroundObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

						//CreateObstaclesWithoutJ2();
						CreateObstacles();
					}
					// 전 바닥이 크기2 빈바닥이면 일반 바닥을 생성할께
					else if (eType == GROUND::TYPE::_VOID2)
					{
						CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 496, 650);
						if (nullptr == pGroundObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

						//if (false == bIsJ2)
						//{
							//CreateObstaclesWithoutJ2();
							CreateObstacles();
						//}
					}
				}
				// 크기 1의 빈 바닥을 생성할꺼야
				else
				{
					// 생성할 위치 전 바닥이 일반 바닥이였다면 빈 바닥을 생성할게
					if (eType == GROUND::TYPE::NORMAL /*&& false == bIsJ2*/)
					{
						CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::_VOID, m_ePhase, fX + 372, 650);
						if (nullptr == pGroundObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

						// 크기1 빈 바닥 인증
						static_cast<CGround*>(pGroundObj)->Set_IsVoid();
						
					}
					// 그런데 전 바닥이 빈 바닥이였으면 빈 바닥 말고 일반 바닥을 생성할게 ( 연속 빈바닥 막기 )
					else if (eType == GROUND::TYPE::_VOID)
					{
						CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 372, 650);
						if (nullptr == pGroundObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

						//if (false == bIsJ2)
						//{
							//CreateObstaclesWithoutJ2();
							CreateObstacles();
						//}
					}
					// 전 바닥이 크기2 빈 바닥일때에도 일반 바닥을 생성할께
					else if (eType == GROUND::TYPE::_VOID2)
					{
						CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 496, 650);
						if (nullptr == pGroundObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

						//if (false == bIsJ2)
						//{
							//CreateObstaclesWithoutJ2();
							CreateObstacles();
						//}
					}
				}
			}
			// 일반 바닥을 생성할건데
			else
			{
				// 일반 바닥은 이전 바닥이 뭐든 상관없이 만들 수 있지
				if (eType == GROUND::TYPE::NORMAL)
				{
					CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 496, 650);
					if (nullptr == pGroundObj) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

					//if (false == bIsObstacleSeries)
					//{
						CreateObstacles();
					//}
				}
				else if (eType == GROUND::TYPE::_VOID)
				{
					CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 372, 650);
					if (nullptr == pGroundObj) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);
					CreateObstacles();
				}
				else if (eType == GROUND::TYPE::_VOID2)
				{
					CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 496, 650);
					if (nullptr == pGroundObj) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);
					CreateObstacles();
				}
			}

			bool bIsVoid = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_IsVoid();
			bool bIsVoid2 = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_IsVoid2();
			bool bIsObstacle = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_IsObstacle();
			INFO fX_Next = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_Info();

			// 이제 바닥에 따라 젤리를 생성할건데, 일반 바닥은 젤리 3개, 빈바닥은 포물선, 장애물이 설치된 바닥은 젤리를 생성 안할꺼야
			// 위에서 생성한 바닥이 뭐야?
			// 크기1 빈 바닥이야
			if (true == bIsVoid)
			{
				for (int i = 30; i < 180; i += 30)
				{
					if (i == 90 && rand() % 10 > 7)
					{
						CGameObject* pJellyObj = CJelly::Create(JELLY::TYPE::SILVER_COIN, fX_Next.fX + 124 * cos(i * DEG_TO_RAD), 520 - 124 * sin(i * DEG_TO_RAD));
						if (nullptr == pJellyObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellyObj);
					}
					else
					{
						CGameObject* pJellyObj = CJelly::Create(JELLY::TYPE::NORMAL, fX_Next.fX + 124 * cos(i * DEG_TO_RAD), 520 - 124 * sin(i * DEG_TO_RAD));
						if (nullptr == pJellyObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellyObj);
					}
				}
			}
			// 크기2 빈 바닥이야
			else if (true == bIsVoid2)
			{
				for (int i = 18; i < 180; i += 18)
				{
					if (i == 90 && rand() % 10 > 7)
					{
						CGameObject* pJellyObj = CJelly::Create(JELLY::TYPE::GOLD_COIN, fX_Next.fX + 248 * cos(i * DEG_TO_RAD), 500 - 220 * sin(i * DEG_TO_RAD));
						if (nullptr == pJellyObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellyObj);
					}
					else
					{
						CGameObject* pJellyObj = CJelly::Create(JELLY::TYPE::NORMAL, fX_Next.fX + 248 * cos(i * DEG_TO_RAD), 500 - 220 * sin(i * DEG_TO_RAD));
						if (nullptr == pJellyObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellyObj);
					}					
				}
				srand(unsigned(time(NULL)));
				int iRandom = rand() % 10;
				if (iRandom < 3)
				{
					for (int i = 0; i < 100; i += 50)
					{
						CGameObject* pJellies = CJelly::Create(JELLY::TYPE::GOLD_BEAR, fX_Next.fX - 25 + i, 600);
						if (nullptr == pJellies) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
					}
					for (int i = 0; i < 100; i += 50)
					{
						CGameObject* pJellies = CJelly::Create(JELLY::TYPE::PINK_BEAR, fX_Next.fX - 25 + i, 550);
						if (nullptr == pJellies) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
					}
				}
				else if (iRandom < 5)
				{
					CGameObject* pHP = CItem::Create(ITEM::TYPE::ENERGY_JELLY, fX_Next.fX, 600);
					if (nullptr == pHP) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pHP);
				}
				else if (iRandom < 8)
				{
					CGameObject* pMagnet = CItem::Create(ITEM::TYPE::MAGNET_JELLY, fX_Next.fX, 600);
					if (nullptr == pMagnet) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pMagnet);
				}
			}
			// 일반 바닥인데 장애물이 없는 바닥이야
			else if (false == bIsObstacle)
			{
				for (int i = -3; i <= 3; ++i)
				{
					CGameObject* pJellyObj = CJelly::Create(JELLY::TYPE::NORMAL, fX_Next.fX + 70 * i, 500);
					if (nullptr == pJellyObj) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellyObj);
				}
			}
		}
	}

	list<CGameObject*>* plistGround = CGameObjectManager::Get_Instance()->Get_Ground();
	RECT m_tPlayerCollision = CGameObjectManager::Get_Instance()->Get_Player()->Get_CollisionRect();

	for (auto pGround : (*plistGround))
	{
		if (m_tPlayerCollision.right >= pGround->Get_Rect().left && m_tPlayerCollision.left <= pGround->Get_Rect().right)
		{
			if (static_cast<CGround*>(pGround)->Get_GroundType() == GROUND::TYPE::_VOID)
			{
				if (static_cast<CGround*>(pGround)->IsCreateJellyLock() == false)
				{
					srand(unsigned(time(NULL)));
					if (rand() % 10 > 7)
					{
						CGameObject* pJellies = CJelly::Create(JELLY::TYPE::RAINBOW_BEAR, pGround->Get_Info().fX, 700);
						if (nullptr == pJellies) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
						static_cast<CJelly*>(pJellies)->IsRise();
					}
				}
				static_cast<CGround*>(pGround)->CreateJellyLock();
			}
		}
	}

	// 배경 재생
	if (CGameObjectManager::Get_Instance()->Get_BackGround()->size() < 2)
	{
		CGameObject* pLastBackGroundObj = CGameObjectManager::Get_Instance()->Get_BackGround()->back();

		CGameObject* pBackGroundObj = CBackGround::Create(BACKGROUND::TYPE::BG_1, m_ePhase, pLastBackGroundObj->Get_Info().fX + 2401, 360);
		if (nullptr == pBackGroundObj) { return; }

		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BACKGROUND, pBackGroundObj);
	}
	if (CGameObjectManager::Get_Instance()->Get_BackGround_2()->size() < 2)
	{
		CGameObject* pLastBackGroundObj = CGameObjectManager::Get_Instance()->Get_BackGround_2()->back();

		CGameObject* pBackGroundObj = CBackGround::Create(BACKGROUND::TYPE::BG_2, m_ePhase, pLastBackGroundObj->Get_Info().fX + 2401, 360);
		if (nullptr == pBackGroundObj) { return; }

		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BACKGROUND_2, pBackGroundObj);
	}
}

void CCreateManager::ReCreate_BG_Ground(int iFinal)
{
	// 실험용 바닥 재생
	if (CGameObjectManager::Get_Instance()->Get_Ground()->size() < 7)
	{
		float fX = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_Info().fX;

		CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 372, 650);
		if (nullptr == pGroundObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);	

		srand(unsigned(time(NULL)));
		int iRandom = rand() % 9 + 1;

		if (iFinal <= 3)
		{
			for (int i = 0; i < 6; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if (i == 5 && j == 5)
					{
						if (iRandom == 1)
						{
							CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::B, fX + 250 + 60 * i, 420 - 60 * j);
							if (nullptr == pBonusTime) { return; }
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
						}
						else if (iRandom == 2)
						{
							CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::O, fX + 250 + 60 * i, 420 - 60 * j);
							if (nullptr == pBonusTime) { return; }
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
						}
						else if (iRandom == 3)
						{
							CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::N, fX + 250 + 60 * i, 420 - 60 * j);
							if (nullptr == pBonusTime) { return; }
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
						}
						else if (iRandom == 4)
						{
							CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::U, fX + 250 + 60 * i, 420 - 60 * j);
							if (nullptr == pBonusTime) { return; }
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
						}
						else if (iRandom == 5)
						{
							CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::S, fX + 250 + 60 * i, 420 - 60 * j);
							if (nullptr == pBonusTime) { return; }
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
						}
						else if (iRandom == 6)
						{
							CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::T, fX + 250 + 60 * i, 420 - 60 * j);
							if (nullptr == pBonusTime) { return; }
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
						}
						else if (iRandom == 7)
						{
							CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::I, fX + 250 + 60 * i, 420 - 60 * j);
							if (nullptr == pBonusTime) { return; }
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
						}
						else if (iRandom == 8)
						{
							CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::M, fX + 250 + 60 * i, 420 - 60 * j);
							if (nullptr == pBonusTime) { return; }
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
						}
						else if (iRandom == 9)
						{
							CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::E, fX + 250 + 60 * i, 420 - 60 * j);
							if (nullptr == pBonusTime) { return; }
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
						}
					}
					else
					{
						CGameObject* pJellies = CJelly::Create(JELLY::TYPE::SILVER_COIN, fX + 250 + 60 * i, 420 - 60 * j);
						if (nullptr == pJellies) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
					}
				}
			}
		}

		
	}

	// 배경 재생
	if (CGameObjectManager::Get_Instance()->Get_BackGround()->size() < 2)
	{
		CGameObject* pLastBackGroundObj = CGameObjectManager::Get_Instance()->Get_BackGround()->back();

		CGameObject* pBackGroundObj = CBackGround::Create(BACKGROUND::TYPE::BG_1, m_ePhase, pLastBackGroundObj->Get_Info().fX + 2401, 360);
		if (nullptr == pBackGroundObj) { return; }

		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BACKGROUND, pBackGroundObj);
	}
	if (CGameObjectManager::Get_Instance()->Get_BackGround_2()->size() < 2)
	{
		CGameObject* pLastBackGroundObj = CGameObjectManager::Get_Instance()->Get_BackGround_2()->back();

		CGameObject* pBackGroundObj = CBackGround::Create(BACKGROUND::TYPE::BG_2, m_ePhase, pLastBackGroundObj->Get_Info().fX + 2401, 360);
		if (nullptr == pBackGroundObj) { return; }

		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BACKGROUND_2, pBackGroundObj);
	}
}

void CCreateManager::Create_BonusTime_Objects()
{
	m_tBlastDistance = GetTickCount64();

	if (m_bCreateBlastLock == false && m_tBlastDistance + 1000 < GetTickCount64())
	{
		CGameObject* pBlast = CItem::Create(ITEM::TYPE::BLAST_JELLY, 2000, 300);
		if (nullptr == pBlast) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pBlast);

		m_tBlastDistance = GetTickCount64();
		++m_iCreateBlastLock;
	}
	if (m_iCreateBlastLock == 3)
	{
		m_bCreateBlastLock = true;
	}

	CGameObject* pGoldCoin = CJelly::Create(JELLY::TYPE::BIG_PINK_BEAR, 1500, 100);
	if (nullptr == pGoldCoin) { return; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pGoldCoin);

	CGameObject* pPinkBear = CJelly::Create(JELLY::TYPE::PINK_BEAR, 1500, 200);
	if (nullptr == pPinkBear) { return; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pPinkBear);

	CGameObject* pRainbowBear = CJelly::Create(JELLY::TYPE::RAINBOW_BEAR, 1500, 300);
	if (nullptr == pRainbowBear) { return; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pRainbowBear);

	CGameObject* pGoldBear = CJelly::Create(JELLY::TYPE::GOLD_BEAR, 1500, 400);
	if (nullptr == pGoldBear) { return; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pGoldBear);

	CGameObject* pSilverCoin = CJelly::Create(JELLY::TYPE::BIG_GOLD_BEAR, 1500, 500);
	if (nullptr == pSilverCoin) { return; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pSilverCoin);
}

void CCreateManager::Create_FeverTime_Object()
{
	
}

void CCreateManager::CreateObstacles()
{
	CGameObject* pGround = CGameObjectManager::Get_Instance()->Get_Ground()->back();
	float fX = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_Info().fX;

	srand(unsigned(time(NULL)));
	int iRandom = rand() % 10;

	if (Obstacles(gen) == 1)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::BOTTOM_1_J1, m_ePhase, fX, 521);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = -1; i <= 1; ++i)
		{
			if (i == 0 && iRandom > 4)
			{
				if (iRandom < 7)
				{
					CGameObject* pBiggest = CItem::Create(ITEM::TYPE::BIGGEST_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pBiggest) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBiggest);
				}
				else if (iRandom < 9)
				{
					CGameObject* pBlast = CItem::Create(ITEM::TYPE::BLAST_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pBlast) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBlast);
				}
				else if (iRandom == 9)
				{
					CGameObject* pHP = CItem::Create(ITEM::TYPE::ENERGY_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pHP) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pHP);
				}				
			}
			else
			{
				CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 70 * i, 421 + 30 * i * i);
				if (nullptr == pJelly) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
			}			
		}

		for (int i = -1; i < 2; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 140 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}

		for (int i = -1; i < 2; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 210 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();

	}
	else if (Obstacles(gen) == 2)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::BOTTOM_2_J1, m_ePhase, fX, 521);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = -1; i <= 1; ++i)
		{
			if (i == 0 && iRandom > 4)
			{
				if (iRandom < 7)
				{
					CGameObject* pBiggest = CItem::Create(ITEM::TYPE::BIGGEST_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pBiggest) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBiggest);
				}
				else if (iRandom < 9)
				{
					CGameObject* pBlast = CItem::Create(ITEM::TYPE::BLAST_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pBlast) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBlast);
				}
				else if (iRandom == 9)
				{
					CGameObject* pHP = CItem::Create(ITEM::TYPE::ENERGY_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pHP) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pHP);
				}
			}
			else
			{
				CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 70 * i, 421 + 30 * i * i);
				if (nullptr == pJelly) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
			}
		}

		for (int i = -1; i < 2; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 140 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}

		for (int i = -1; i < 2; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 210 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();

	}
	else if (Obstacles(gen) == 3)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::BOTTOM_1_J2, m_ePhase, fX, 466);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = 18; i < 180; i += 18)
		{
			if (i == 90 && rand() % 10 < 3)
			{
				CGameObject* pBlast = CItem::Create(ITEM::TYPE::BLAST_JELLY, fX + 186 * cos(i * DEG_TO_RAD), 500 - 225 * sin(i * DEG_TO_RAD));
				if (nullptr == pBlast) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBlast);
			}
			else
			{
				CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 186 * cos(i * DEG_TO_RAD), 500 - 225 * sin(i * DEG_TO_RAD));
				if (nullptr == pJelly) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
			}
		}

		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 220 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();
		static_cast<CGround*>(pGround)->Set_IsJ2();
	}
	else if (Obstacles(gen) == 4)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::BOTTOM_2_J2, m_ePhase, fX, 466);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = 18; i < 180; i += 18)
		{
			if (i == 90 && rand() % 10 < 3)
			{
				CGameObject* pBlast = CItem::Create(ITEM::TYPE::BLAST_JELLY, fX + 186 * cos(i * DEG_TO_RAD), 500 - 225 * sin(i * DEG_TO_RAD));
				if (nullptr == pBlast) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBlast);
			}
			else
			{
				CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 186 * cos(i * DEG_TO_RAD), 500 - 225 * sin(i * DEG_TO_RAD));
				if (nullptr == pJelly) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
			}
		}

		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 220 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();
		static_cast<CGround*>(pGround)->Set_IsJ2();
	}
	else if (Obstacles(gen) == 5)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::TOP_1, m_ePhase, fX, 200);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 150 * i, 525);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 225 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		for (int i = -1; i <= 1; ++i)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 75 * i, 550);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();
	}
	else if (Obstacles(gen) == 6)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::TOP_2, m_ePhase, fX, 200);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 150 * i, 525);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 225 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		for (int i = -1; i <= 1; ++i)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 75 * i, 550);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();
	}
}

void CCreateManager::CreateObstaclesWithoutJ2()
{
	CGameObject* pGround = CGameObjectManager::Get_Instance()->Get_Ground()->back();
	float fX = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_Info().fX;

	srand(unsigned(time(NULL)));
	int iRandom = rand() % 10;

	if (Obstacles(gen) == 1)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::BOTTOM_1_J1, m_ePhase, fX, 521);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = -1; i <= 1; ++i)
		{
			if (i == 0 && iRandom > 4)
			{
				if (iRandom < 7)
				{
					CGameObject* pBiggest = CItem::Create(ITEM::TYPE::BIGGEST_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pBiggest) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBiggest);
				}
				else if (iRandom < 9)
				{
					CGameObject* pBlast = CItem::Create(ITEM::TYPE::BLAST_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pBlast) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBlast);
				}
				else if (iRandom == 9)
				{
					CGameObject* pHP = CItem::Create(ITEM::TYPE::ENERGY_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pHP) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pHP);
				}
			}
			else
			{
				CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 70 * i, 421 + 30 * i * i);
				if (nullptr == pJelly) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
			}
		}
		for (int i = -1; i < 2; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 140 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}

		for (int i = -1; i < 2; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 210 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();
	}
	else if (Obstacles(gen) == 2)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::BOTTOM_2_J1, m_ePhase, fX, 521);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = -1; i <= 1; ++i)
		{
			if (i == 0 && iRandom > 4)
			{
				if (iRandom < 7)
				{
					CGameObject* pBiggest = CItem::Create(ITEM::TYPE::BIGGEST_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pBiggest) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBiggest);
				}
				else if (iRandom < 9)
				{
					CGameObject* pBlast = CItem::Create(ITEM::TYPE::BLAST_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pBlast) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBlast);
				}
				else if (iRandom == 9)
				{
					CGameObject* pHP = CItem::Create(ITEM::TYPE::ENERGY_JELLY, fX + 70 * i, 421 + 30 * i * i);
					if (nullptr == pHP) { return; }
					CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pHP);
				}
			}
			else
			{
				CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 70 * i, 421 + 30 * i * i);
				if (nullptr == pJelly) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
			}
		}
		for (int i = -1; i < 2; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 140 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}

		for (int i = -1; i < 2; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 210 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();
	}
	else if (Obstacles(gen) == 5)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::TOP_1, m_ePhase, fX, 200);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 150 * i, 525);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 225 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		for (int i = -1; i <= 1; ++i)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 75 * i, 550);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();
	}
	else if (Obstacles(gen) == 6)
	{
		CGameObject* pObstacleObj = CObstacles::Create(OBSTACLE::TYPE::TOP_2, m_ePhase, fX, 200);
		if (nullptr == pObstacleObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::OBSTACLE, pObstacleObj);

		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 150 * i, 525);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		for (int i = -1; i <= 1; i += 2)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 225 * i, 500);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		for (int i = -1; i <= 1; ++i)
		{
			CGameObject* pJelly = CJelly::Create(JELLY::TYPE::NORMAL, fX + 75 * i, 550);
			if (nullptr == pJelly) { return; }
			CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJelly);
		}
		static_cast<CGround*>(pGround)->Set_IsObstacle();
	}
}

void CCreateManager::StartJelly()
{
	srand(unsigned(time(NULL)));
	int iRandom = rand() % 10;

	CGameObject* pBlast = CItem::Create(ITEM::TYPE::BLAST_JELLY, 1800, 500);
	if (nullptr == pBlast) { return; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBlast);
	CGameObject* pBiggest = CItem::Create(ITEM::TYPE::BIGGEST_JELLY, 1800, 450);
	if (nullptr == pBiggest) { return; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBiggest);
	CGameObject* pMagnet = CItem::Create(ITEM::TYPE::MAGNET_JELLY, 1800, 400);
	if (nullptr == pMagnet) { return; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pMagnet);

	// 시작 젤리 생성
	for (int i = 0; i < 360; i += 45)
	{
		CGameObject* pJellies = CJelly::Create(JELLY::TYPE::SILVER_COIN, 800 + 70 * cos(i * DEG_TO_RAD), 350 - 70 * sin(i * DEG_TO_RAD));
		if (nullptr == pJellies) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
		if (i == 315)
		{
			if (iRandom < 4)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::PINK_BEAR, 800, 350);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 7)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::BIG_GOLD_BEAR, 800, 350);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 9)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::RAINBOW_BEAR, 800, 350);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom == 9)
			{
				CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::B, 800, 350);
				if (nullptr == pBonusTime) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
			}
		}
	}
	for (int i = 0; i < 360; i += 45)
	{
		CGameObject* pJellies = CJelly::Create(JELLY::TYPE::SILVER_COIN, 1000 + 70 * cos(i * DEG_TO_RAD), 450 - 70 * sin(i * DEG_TO_RAD));
		if (nullptr == pJellies) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
		if (i == 315)
		{
			if (iRandom < 4)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::GOLD_BEAR, 1000, 450);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 7)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::BIG_PINK_BEAR, 1000, 450);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 9)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::RAINBOW_BEAR, 1000, 450);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom == 9)
			{
				CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::O, 1000, 450);
				if (nullptr == pBonusTime) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
			}
		}
	}
	for (int i = 0; i < 360; i += 45)
	{
		CGameObject* pJellies = CJelly::Create(JELLY::TYPE::SILVER_COIN, 1200 + 70 * cos(i * DEG_TO_RAD), 350 - 70 * sin(i * DEG_TO_RAD));
		if (nullptr == pJellies) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
		if (i == 315)
		{
			if (iRandom < 4)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::PINK_BEAR, 1200, 350);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 7)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::BIG_GOLD_BEAR, 1200, 350);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 9)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::RAINBOW_BEAR, 1200, 350);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom == 9)
			{
				CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::N, 1200, 350);
				if (nullptr == pBonusTime) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
			}
		}
	}
	for (int i = 0; i < 360; i += 45)
	{
		CGameObject* pJellies = CJelly::Create(JELLY::TYPE::SILVER_COIN, 1400 + 70 * cos(i * DEG_TO_RAD), 450 - 70 * sin(i * DEG_TO_RAD));
		if (nullptr == pJellies) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
		if (i == 315)
		{
			if (iRandom < 4)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::GOLD_BEAR, 1400, 450);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 7)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::BIG_PINK_BEAR, 1400, 450);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 9)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::RAINBOW_BEAR, 1400, 450);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom == 9)
			{
				CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::U, 1400, 450);
				if (nullptr == pBonusTime) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
			}
		}
	}
	for (int i = 0; i < 360; i += 45)
	{
		CGameObject* pJellies = CJelly::Create(JELLY::TYPE::SILVER_COIN, 1600 + 70 * cos(i * DEG_TO_RAD), 350 - 70 * sin(i * DEG_TO_RAD));
		if (nullptr == pJellies) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
		if (i == 315)
		{
			if (iRandom < 4)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::PINK_BEAR, 1600, 350);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 7)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::BIG_GOLD_BEAR, 1600, 350);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom < 9)
			{
				CGameObject* pJellies = CJelly::Create(JELLY::TYPE::RAINBOW_BEAR, 1600, 350);
				if (nullptr == pJellies) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::JELLY, pJellies);
			}
			else if (iRandom == 9)
			{
				CGameObject* pBonusTime = CItem::Create(ITEM::TYPE::S, 1600, 350);
				if (nullptr == pBonusTime) { return; }
				CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::ITEM, pBonusTime);
			}
		}
	}
}

const TCHAR* CCreateManager::SentenceList()
{
	if (m_pCookie == L"BraveCookie")
	{
		int iRandom = Sentence(gen);

		if (iRandom == 1)
		{
			return L"난 그냥 쿠키가 아니야!";
		}
		else if (iRandom == 2)
		{
			return L"달리기 하면 나지!";
		}
		else if (iRandom == 3)
		{
			return L"돌아보지 말고 뛰어!";
		}
		else if (iRandom == 4)
		{
			return L"마녀는 보이지 않는군... 이때다!";
		}
		else if (iRandom == 5)
		{
			return L"안 돼! 난 여기서 빠져나가야겠어!";
		}
		else if (iRandom == 6)
		{
			return L"우리는 해낼 수 있어!";
		}
		else if (iRandom == 7)
		{
			return L"이곳을 꼭 나가야겠어!";
		}
		else if (iRandom == 8)
		{
			return L"이대로 먹힐 수는 없어!";
		}
		else if (iRandom == 9)
		{
			return L"지금이야! 가자!";
		}
		else if (iRandom == 10)
		{
			return L"탈출해야 해!";
		}
	}
	else if (m_pCookie == L"BlackBerryCookie")
	{
		int iRandom = Sentence(gen);

		if (iRandom == 1)
		{
			return L"또 어디로 사라지신 걸까...";
		}
		else if (iRandom == 2)
		{
			return L"또 어디로 사라지신 걸까...";
		}
		else if (iRandom == 3)
		{
			return L"맡기실 일이 있으신가요?";
		}
		else if (iRandom == 4)
		{
			return L"아직 할 일이 많이 남아있습니다.";
		}
		else if (iRandom == 5)
		{
			return L"이곳은 정리가 좀 필요할 것 같군요.";
		}
		else if (iRandom == 6)
		{
			return L"이번에는 어디로 가신걸까...";
		}
		else if (iRandom == 7)
		{
			return L"이제 집에 가실 시간입니다.";
		}
		else if (iRandom == 8)
		{
			return L"저 혼자서도 충분합니다.";
		}
		else if (iRandom >= 9)
		{
			return L"항상 품위를 유지하셔야 합니다.";
		}
	}
	else if (m_pCookie == L"CherryCookie")
	{
		int iRandom = Sentence(gen);

		if (iRandom == 1)
		{
			return L"같이 놀자! 내가 피크닉 가기 좋은 곳을 알아!";
		}
		else if (iRandom == 2)
		{
			return L"기분이 샤랄라~ 날아갈 것 같은 느낌!";
		}
		else if (iRandom == 3)
		{
			return L"나랑 피크닉 갈래?";
		}
		else if (iRandom == 4)
		{
			return L"바람이 우리를 좋은 곳으로 데려다줄거야!";
		}
		else if (iRandom == 5)
		{
			return L"벚꽃놀이 좋아해?";
		}
		else if (iRandom == 6)
		{
			return L"샌드위치? 좀 먹어봐~! 맛있지?";
		}
		else if (iRandom == 7)
		{
			return L"어떤 차 좋아해? 홍차?";
		}
		else if (iRandom == 8)
		{
			return L"피크닉 가기 좋은 날씨야!";
		}
		else if (iRandom >= 9)
		{
			return L"훨~ 훨~ 날아라~ 훨~";
		}

	}
	else if (m_pCookie == L"ChessChocoCookie")
	{
		int iRandom = Sentence(gen);

		if (iRandom == 1)
		{
			return L"우리 체스할래?";
		}
		else if (iRandom == 2)
		{
			return L"우린 둘만 있으면 돼";
		}
		else if (iRandom == 3)
		{
			return L"우린 둘이 합쳐 하나야!";
		}
		else if (iRandom == 4)
		{
			return L"우리는 언제나 하나야";
		}
		else if (iRandom == 5)
		{
			return L"우리가 이겼어!";
		}
		else if (iRandom == 6)
		{
			return L"이겼으니 뭘 줄래? ";
		}
		else if (iRandom == 7)
		{
			return L"비숍으로 공격할게~ 체스블랙 쿠키!";
		}
		else if (iRandom == 8)
		{
			return L"체스화이트 쿠키, 나이트로 방어할게.";
		}
		else if (iRandom == 9)
		{
			return L"체스블랙 쿠키, 이번엔 내가 먼저 시작할래~";
		}
		else if (iRandom == 10)
		{
			return L"이번엔 내가 끝낼게, 체스화이트 쿠키.";
		}
		else if (iRandom == 11)
		{
			return L"체크메이트야!";
		}
	}
	else if (m_pCookie == L"MoonlightCookie")
	{
		int iRandom = Sentence(gen);

		if (iRandom == 1)
		{
			return L"겨울 같은 시간 속에서 봄 같은 꿈을 꾸어요";
		}
		else if (iRandom == 2)
		{
			return L"꿈 속에서 저는 별들과 춤을 추어요";
		}
		else if (iRandom == 3)
		{
			return L"꿈을 꾸는 기분이에요";
		}
		else if (iRandom == 4)
		{
			return L"꿈이 흐르고 별빛이 춤을 춰요";
		}
		else if (iRandom == 5)
		{
			return L"당신은 지금 깨어있나요?";
		}
		else if (iRandom == 6)
		{
			return L"별들이 우리를 지켜줄 거예요";
		}
		else if (iRandom == 7)
		{
			return L"시간은 멎어있고, 고요만 남아있네요";
		}
		else if (iRandom == 8)
		{
			return L"언젠간 그분들을 다시 만날 날이 오겠죠?";
		}
		else if (iRandom >= 9)
		{
			return L"오늘도 달빛이 아름답네요";
		}
	}
}

void CCreateManager::ReCreateOnlyGround()
{
	// 실험용 바닥 재생
	if (CGameObjectManager::Get_Instance()->Get_Ground()->size() < 7)
	{
		float fX = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_Info().fX;

		CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 372, 650);
		if (nullptr == pGroundObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);
	}
}