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

// ��ֹ� �������� ���� : 1,2 = 1�� ��ֹ� | 3,4 = 2�� ��ֹ� | 5,6 = õ�� ��ֹ�
uniform_int_distribution<int> Obstacles(1, 6);

// ũ�Ⱑ 1 �Ǵ� 3�� �� �ٴ� ���� ����
uniform_int_distribution<int> Ground_VOID(1, 10);
uniform_int_distribution<int> Ground_VOID2(1, 10);

// ��� ���� ���
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
	// �÷��̾� ����
	CGameObject* pPlayerObj = CPlayer::Create(m_pCookie);
	if (nullptr == pPlayerObj) { return READY_FAILED; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::PLAYER, pPlayerObj);

	// �� ����
	CGameObject* pPetObj = CPet::Create();
	if (nullptr == pPetObj) { return READY_FAILED; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::PET, pPetObj);

	// �ٴ� ����
	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, 248 * (float)(2 * i), 650);
		if (nullptr == pGroundObj) { return READY_FAILED; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);
	}
	// ��� ����
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

	// ���ʽ�Ÿ�� �ٴ� ����
	for (int i = 0; i < 12; ++i)
	{
		CGameObject* pBTGround = CGround_BT::Create(812 + (float)(1878 * i), 620);
		if (nullptr == pBTGround) { return READY_FAILED; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BT_GROUND, pBTGround);
	}
	// ���ʽ�Ÿ�� ��� ����
	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pBTObj = CBackGround_BT::Create(939 + (float)(1878 * i), 360);
		if (nullptr == pBTObj) { return READY_FAILED; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::BT_BACKGROUND, pBTObj);
	}

	// UI ����
	CGameObject* pUIObj_BT = CUI::Create();
	if (nullptr == pUIObj_BT) { return READY_FAILED; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::UI, pUIObj_BT);

	if (m_bCreateScoreLock == false)
	{
		// ���ھ�� ����
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
		// ����� �ٴ� ���
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
		// ������ �ٴ� ���	
		if (CGameObjectManager::Get_Instance()->Get_Ground()->size() < 7)
		{
			// ������ �ٴ� "������ �ٴ��� ����" �ϱ� ���� ������ �ٴ� ������ �����´� ( ������ �ٴ��� X��ǥ�� �ٴ� Ÿ�� )
			GROUND::TYPE eType = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_GroundType();
			float fX = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_Info().fX;
			bool bIsJ2 = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_IsJ2();
			bool bIsObstacleSeries = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_IsObstacle();

			// �� �ٴ��� �����Ұǵ�
			if (Ground_VOID(gen) > 3)
			{
				// ��� ũ���� ��ٴ��� �����Ҳ��ĸ�
				// ũ�� 2�� �� �ٴ��� �����Ҳ���
				if (Ground_VOID2(gen) > 6)
				{
					// ������ ��ġ ���� �ٴ��� �Ϲ� �ٴ��� ������, �� ũ��2 ��ֹ��� ���� �ٴ��� ������ ũ��2 ��ٴ��� �����Ҳ�
					if (eType == GROUND::TYPE::NORMAL /*&& false == bIsJ2*/)
					{
						CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::_VOID2, m_ePhase, fX + 496, 650);
						if (nullptr == pGroundObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

						// ũ��2 �� �ٴ� ����
						static_cast<CGround*>(pGroundObj)->Set_IsVoid2();
					}
					// �� �ٴ��� ũ��1 ��ٴ��̸� �Ϲ� �ٴ��� �����Ҳ�
					else if (eType == GROUND::TYPE::_VOID)
					{
						CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 372, 650);
						if (nullptr == pGroundObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

						//CreateObstaclesWithoutJ2();
						CreateObstacles();
					}
					// �� �ٴ��� ũ��2 ��ٴ��̸� �Ϲ� �ٴ��� �����Ҳ�
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
				// ũ�� 1�� �� �ٴ��� �����Ҳ���
				else
				{
					// ������ ��ġ �� �ٴ��� �Ϲ� �ٴ��̿��ٸ� �� �ٴ��� �����Ұ�
					if (eType == GROUND::TYPE::NORMAL /*&& false == bIsJ2*/)
					{
						CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::_VOID, m_ePhase, fX + 372, 650);
						if (nullptr == pGroundObj) { return; }
						CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);

						// ũ��1 �� �ٴ� ����
						static_cast<CGround*>(pGroundObj)->Set_IsVoid();
						
					}
					// �׷��� �� �ٴ��� �� �ٴ��̿����� �� �ٴ� ���� �Ϲ� �ٴ��� �����Ұ� ( ���� ��ٴ� ���� )
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
					// �� �ٴ��� ũ��2 �� �ٴ��϶����� �Ϲ� �ٴ��� �����Ҳ�
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
			// �Ϲ� �ٴ��� �����Ұǵ�
			else
			{
				// �Ϲ� �ٴ��� ���� �ٴ��� ���� ������� ���� �� ����
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

			// ���� �ٴڿ� ���� ������ �����Ұǵ�, �Ϲ� �ٴ��� ���� 3��, ��ٴ��� ������, ��ֹ��� ��ġ�� �ٴ��� ������ ���� ���Ҳ���
			// ������ ������ �ٴ��� ����?
			// ũ��1 �� �ٴ��̾�
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
			// ũ��2 �� �ٴ��̾�
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
			// �Ϲ� �ٴ��ε� ��ֹ��� ���� �ٴ��̾�
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

	// ��� ���
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
	// ����� �ٴ� ���
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

	// ��� ���
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

	// ���� ���� ����
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
			return L"�� �׳� ��Ű�� �ƴϾ�!";
		}
		else if (iRandom == 2)
		{
			return L"�޸��� �ϸ� ����!";
		}
		else if (iRandom == 3)
		{
			return L"���ƺ��� ���� �پ�!";
		}
		else if (iRandom == 4)
		{
			return L"����� ������ �ʴ±�... �̶���!";
		}
		else if (iRandom == 5)
		{
			return L"�� ��! �� ���⼭ ���������߰ھ�!";
		}
		else if (iRandom == 6)
		{
			return L"�츮�� �س� �� �־�!";
		}
		else if (iRandom == 7)
		{
			return L"�̰��� �� �����߰ھ�!";
		}
		else if (iRandom == 8)
		{
			return L"�̴�� ���� ���� ����!";
		}
		else if (iRandom == 9)
		{
			return L"�����̾�! ����!";
		}
		else if (iRandom == 10)
		{
			return L"Ż���ؾ� ��!";
		}
	}
	else if (m_pCookie == L"BlackBerryCookie")
	{
		int iRandom = Sentence(gen);

		if (iRandom == 1)
		{
			return L"�� ���� ������� �ɱ�...";
		}
		else if (iRandom == 2)
		{
			return L"�� ���� ������� �ɱ�...";
		}
		else if (iRandom == 3)
		{
			return L"�ñ�� ���� �����Ű���?";
		}
		else if (iRandom == 4)
		{
			return L"���� �� ���� ���� �����ֽ��ϴ�.";
		}
		else if (iRandom == 5)
		{
			return L"�̰��� ������ �� �ʿ��� �� ������.";
		}
		else if (iRandom == 6)
		{
			return L"�̹����� ���� ���Űɱ�...";
		}
		else if (iRandom == 7)
		{
			return L"���� ���� ���� �ð��Դϴ�.";
		}
		else if (iRandom == 8)
		{
			return L"�� ȥ�ڼ��� ����մϴ�.";
		}
		else if (iRandom >= 9)
		{
			return L"�׻� ǰ���� �����ϼž� �մϴ�.";
		}
	}
	else if (m_pCookie == L"CherryCookie")
	{
		int iRandom = Sentence(gen);

		if (iRandom == 1)
		{
			return L"���� ����! ���� ��ũ�� ���� ���� ���� �˾�!";
		}
		else if (iRandom == 2)
		{
			return L"����� ������~ ���ư� �� ���� ����!";
		}
		else if (iRandom == 3)
		{
			return L"���� ��ũ�� ����?";
		}
		else if (iRandom == 4)
		{
			return L"�ٶ��� �츮�� ���� ������ �������ٰž�!";
		}
		else if (iRandom == 5)
		{
			return L"���ɳ��� ������?";
		}
		else if (iRandom == 6)
		{
			return L"������ġ? �� �Ծ��~! ������?";
		}
		else if (iRandom == 7)
		{
			return L"� �� ������? ȫ��?";
		}
		else if (iRandom == 8)
		{
			return L"��ũ�� ���� ���� ������!";
		}
		else if (iRandom >= 9)
		{
			return L"��~ ��~ ���ƶ�~ ��~";
		}

	}
	else if (m_pCookie == L"ChessChocoCookie")
	{
		int iRandom = Sentence(gen);

		if (iRandom == 1)
		{
			return L"�츮 ü���ҷ�?";
		}
		else if (iRandom == 2)
		{
			return L"�츰 �Ѹ� ������ ��";
		}
		else if (iRandom == 3)
		{
			return L"�츰 ���� ���� �ϳ���!";
		}
		else if (iRandom == 4)
		{
			return L"�츮�� ������ �ϳ���";
		}
		else if (iRandom == 5)
		{
			return L"�츮�� �̰��!";
		}
		else if (iRandom == 6)
		{
			return L"�̰����� �� �ٷ�? ";
		}
		else if (iRandom == 7)
		{
			return L"������� �����Ұ�~ ü���� ��Ű!";
		}
		else if (iRandom == 8)
		{
			return L"ü��ȭ��Ʈ ��Ű, ����Ʈ�� ����Ұ�.";
		}
		else if (iRandom == 9)
		{
			return L"ü���� ��Ű, �̹��� ���� ���� �����ҷ�~";
		}
		else if (iRandom == 10)
		{
			return L"�̹��� ���� ������, ü��ȭ��Ʈ ��Ű.";
		}
		else if (iRandom == 11)
		{
			return L"üũ����Ʈ��!";
		}
	}
	else if (m_pCookie == L"MoonlightCookie")
	{
		int iRandom = Sentence(gen);

		if (iRandom == 1)
		{
			return L"�ܿ� ���� �ð� �ӿ��� �� ���� ���� �پ��";
		}
		else if (iRandom == 2)
		{
			return L"�� �ӿ��� ���� ����� ���� �߾��";
		}
		else if (iRandom == 3)
		{
			return L"���� �ٴ� ����̿���";
		}
		else if (iRandom == 4)
		{
			return L"���� �帣�� ������ ���� ���";
		}
		else if (iRandom == 5)
		{
			return L"����� ���� �����ֳ���?";
		}
		else if (iRandom == 6)
		{
			return L"������ �츮�� ������ �ſ���";
		}
		else if (iRandom == 7)
		{
			return L"�ð��� �ܾ��ְ�, ��丸 �����ֳ׿�";
		}
		else if (iRandom == 8)
		{
			return L"������ �׺е��� �ٽ� ���� ���� ������?";
		}
		else if (iRandom >= 9)
		{
			return L"���õ� �޺��� �Ƹ���׿�";
		}
	}
}

void CCreateManager::ReCreateOnlyGround()
{
	// ����� �ٴ� ���
	if (CGameObjectManager::Get_Instance()->Get_Ground()->size() < 7)
	{
		float fX = static_cast<CGround*>(CGameObjectManager::Get_Instance()->Get_Ground()->back())->Get_Info().fX;

		CGameObject* pGroundObj = CGround::Create(GROUND::TYPE::NORMAL, m_ePhase, fX + 372, 650);
		if (nullptr == pGroundObj) { return; }
		CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::GROUND, pGroundObj);
	}
}