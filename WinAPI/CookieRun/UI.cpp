#include "pch.h"
#include "UI.h"
#include "GameObjectManager.h"
#include "BitmapManager.h"
#include "Controller.h"
#include "SceneManager.h"
#include "Stage.h"
#include "CreateManager.h"

#include "Player.h"
#include "Ground.h"
#include "BackGround.h"
#include "Obstacles.h"
#include "Jelly.h"
#include "Item.h"

CUI::CUI()
	: m_tLifeTime(0)
	, m_iGauge(0)
	, m_tBonusTime({ false })
	, m_iAlpha(0)
	, m_bBonusTimeGauge(false)
	, m_fBonusTimeGaugeDecrease(248)
	, m_bBonusTimeGaugeDecrease(false)
{}

CUI::~CUI()
{
	Release_GameObject();
}

int CUI::Ready_GameObject()
{
	m_tLifeTime = GetTickCount64();
	m_iGauge = 915;
	m_bGaugeStop = false;

	m_bIsActive = true;

	m_bAlphaPlus_IN = false;
	m_bAlphaMinus_IN = false;
	m_bAlphaPlus_OUT = false;
	m_bAlphaMinus_OUT = false;
	m_iAlpha = 0;

	m_bTemp = false;
	m_bTemp2 = false;
	m_bTemp3 = false;
	m_bTemp4 = false;

	m_tEffectInfo.iCX = 900;
	m_tEffectInfo.iCY = 900;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;

	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount64();

	m_bExhaustLock = false;

	m_fProgressX = 0.f;

	m_tProgress = GetTickCount64();

	return READY_OK;
}

int CUI::Update_GameObject()
{
	if (CCreateManager::Get_Instance()->Get_Cookie() == L"BraveCookie")
	{
		m_iHP = 25;
	}
	else if (CCreateManager::Get_Instance()->Get_Cookie() == L"CherryCookie")
	{
		m_iHP = 20;
	}
	else if (CCreateManager::Get_Instance()->Get_Cookie() == L"BlackBerryCookie")
	{
		m_iHP = 15;
	}
	else if (CCreateManager::Get_Instance()->Get_Cookie() == L"ChessChocoCookie")
	{
		m_iHP = 4;
	}
	else if (CCreateManager::Get_Instance()->Get_Cookie() == L"MoonlightCookie")
	{
		m_iHP = 4;
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_LB))
	{
		m_iGauge -= 250;
	}

	if (m_tProgress + 1000 < GetTickCount64())
	{
		m_fProgressX += 2.8f;
		m_tProgress = GetTickCount64();
	}

	Check_BonusTime();

	if (m_bGaugeStop == false && m_tLifeTime + 1000 < GetTickCount64())
	{
		m_iGauge -= m_iHP;
		m_tLifeTime = GetTickCount64();
	}
	if (m_iGauge <= 0 && m_bExhaustLock == false)
	{
		static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Set_Exhaust();
		m_bExhaustLock = true;
	}

	if (m_iAlpha < 250 && m_bAlphaPlus_IN == true)
	{
		m_iAlpha += 7;
	}

	if (m_iAlpha > 10 && m_bAlphaMinus_IN == true)
	{
		m_iAlpha -= 7;
	}

	if (m_iAlpha < 250 && m_bAlphaPlus_OUT == true)
	{
		m_iAlpha += 12;
	}

	if (m_iAlpha > 10 && m_bAlphaMinus_OUT == true)
	{
		m_iAlpha -= 5;
	}

	m_tEffectInfo.fX = CGameObjectManager::Get_Instance()->Get_Player()->Get_Info().fX;
	m_tEffectInfo.fY = CGameObjectManager::Get_Instance()->Get_Player()->Get_Info().fY + 110;

	m_tEffectRect.left = m_tEffectInfo.fX - (m_tEffectInfo.iCX / 2);
	m_tEffectRect.top = m_tEffectInfo.fY - (m_tEffectInfo.iCY / 2);
	m_tEffectRect.right = m_tEffectInfo.fX + (m_tEffectInfo.iCX / 2);
	m_tEffectRect.bottom = m_tEffectInfo.fY + (m_tEffectInfo.iCY / 2);

	return OBJ_NO_EVENT;
}

void CUI::Late_Update_GameObject()
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

	if (m_bBonusTimeGaugeDecrease == true)
	{
		m_fBonusTimeGaugeDecrease -= 0.2713f;
	}

	if (m_tBonusTimeStart + 1000 < GetTickCount64() && m_bTemp2 == true)
	{
		for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Ground()))
		{
			pObject->Passive();
		}

		for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_BackGround()))
		{
			pObject->Passive();
		}

		for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_BT_Ground()))
		{
			pObject->Active();
		}

		for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_BT_BackGround()))
		{
			pObject->Active();
		}

		if (m_tBonusTimeStart + 1500 < GetTickCount64())
		{
			m_bAlphaPlus_IN = false;
			m_bAlphaMinus_IN = true;
		}

		if (m_tBonusTimeStart + 2000 < GetTickCount64())
		{
			static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Set_StageType(STAGE::TYPE::BONUSTIME);

			m_bBonusTimeGaugeDecrease = true;
			m_bTemp2 = false;
			m_bAlphaMinus_IN = false;
			m_iAlpha = 0;
			m_tBonusTimeStart = GetTickCount64();
		}

		for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Obstacle()))
		{
			static_cast<CObstacles*>(pObject)->Set_IsObstacleType(OBSTACLE::TYPE::END);
		}

		for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Jelly()))
		{
			static_cast<CJelly*>(pObject)->Set_JellyType(JELLY::TYPE::END);
		}
	}

	if (m_tBonusTimeEnd + 16000 < GetTickCount64() && m_bTemp == true)
	{
		if (m_bTemp4 == true)
		{
			m_bAlphaPlus_OUT = true;

			for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Jelly()))
			{
				static_cast<CJelly*>(pObject)->Set_JellyType(JELLY::TYPE::END);
			}

			for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Ground()))
			{
				static_cast<CGround*>(pObject)->Set_GroundType(GROUND::TYPE::NORMAL);
			}

			for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Item()))
			{
				static_cast<CItem*>(pObject)->Set_ItemType(ITEM::TYPE::BLAST_EFFECT);
			}

			static_cast<CStage*>(CSceneManager::Get_Instance()->Get_Scene())->Set_BonusTime();
			static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Set_StageType(STAGE::TYPE::BT_OUT_1);
			m_bTemp4 = false;
		}

		if (m_tBonusTimeEnd + 17500 < GetTickCount64())
		{
			

			for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Ground()))
			{
				pObject->Active();
			}

			for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_BackGround()))
			{
				pObject->Active();
			}

			for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_BT_Ground()))
			{
				pObject->Passive();
			}

			for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_BT_BackGround()))
			{
				pObject->Passive();
			}

			m_bAlphaPlus_OUT = false;
			m_bAlphaMinus_OUT = true;
			static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Set_StageType(STAGE::TYPE::BT_OUT_2);
			for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Item()))
			{
				static_cast<CItem*>(pObject)->Set_ItemType(ITEM::TYPE::BLAST_EFFECT);
			}
		}

		if (m_tBonusTimeEnd + 18500 < GetTickCount64())
		{
			CSoundManager::Get_Instance()->StopSound(CSoundManager::CHANNELID::BGM);

			if (CCreateManager::Get_Instance()->Get_Phase() == PHASE::TYPE::_1ST)
			{
				CSoundManager::Get_Instance()->PlayBGM(L"PHASE_1.mp3");
			}
			else if (CCreateManager::Get_Instance()->Get_Phase() == PHASE::TYPE::_2ND)
			{
				CSoundManager::Get_Instance()->PlayBGM(L"PHASE_2.mp3");
			}

			m_bAlphaMinus_OUT = false;
			m_bTemp = false;
			m_bBonusTimeGaugeDecrease = false;
			m_fBonusTimeGaugeDecrease = 248;
			m_bBonusTimeGauge = false;
			m_bGaugeStop = false;
			m_tBonusTimeEnd = GetTickCount64();
		}
	}
}

void CUI::Render_GameObject(HDC hDC)
{
	if (m_bAlphaPlus_IN == true || m_bAlphaMinus_IN == true)
	{
		BLENDFUNCTION	tBlend = {};
		tBlend.SourceConstantAlpha = m_iAlpha;

		HDC hMemDC_Under = CBitmapManager::Get_Instance()->Find_Image(L"BonusTime_In");
		if (nullptr == hMemDC_Under) { return; }
		AlphaBlend(hDC, 0, 0, 1280, 720, hMemDC_Under, 0, 0, 1280, 720, tBlend);

		m_tEffectRect.left = m_tEffectInfo.fX - (m_tEffectInfo.iCX / 2);
		m_tEffectRect.top = m_tEffectInfo.fY - (m_tEffectInfo.iCY / 2);
		m_tEffectRect.right = m_tEffectInfo.fX + (m_tEffectInfo.iCX / 2);
		m_tEffectRect.bottom = m_tEffectInfo.fY + (m_tEffectInfo.iCY / 2);

		HDC hMemDC_Effect = CBitmapManager::Get_Instance()->Find_Image(L"Effect");
		if (nullptr == hMemDC_Effect) { return; }
		GdiTransparentBlt
		(
			hDC,
			m_tEffectRect.left,
			m_tEffectRect.top,
			m_tEffectInfo.iCX,
			m_tEffectInfo.iCY,
			hMemDC_Effect,
			m_tFrame.iFrameStart * m_tEffectInfo.iCX,
			0,
			m_tEffectInfo.iCX,
			m_tEffectInfo.iCY,
			RGB(255, 0, 255)
		);
		//AlphaBlend(hDC, m_tEffectRect.left, m_tEffectRect.top, m_tEffectInfo.iCX, m_tEffectInfo.iCY, hMemDC_Effect, m_tFrame.iFrameStart * m_tEffectInfo.iCX, 0, m_tEffectInfo.iCX, m_tEffectInfo.iCY, tBlend);
	}

	if (m_bAlphaPlus_OUT == true || m_bAlphaMinus_OUT == true)
	{
		BLENDFUNCTION	tBlend = {};
		tBlend.SourceConstantAlpha = m_iAlpha;

		HDC hMemDC_Under = CBitmapManager::Get_Instance()->Find_Image(L"BonusTime_Out");
		if (nullptr == hMemDC_Under) { return; }
		AlphaBlend(hDC, 0, 0, 1280, 720, hMemDC_Under, 0, 0, 1280, 720, tBlend);
	}

	HDC hMemDC_BonusTime = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME");
	if (nullptr == hMemDC_BonusTime) { return; }
	GdiTransparentBlt(hDC, 5, 20, 248, 35, hMemDC_BonusTime, 0, 0, 248, 35, RGB(255, 0, 255));

	if (m_bBonusTimeGauge == true)
	{
		HDC hMemDC_BonusTime = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC_BonusTime) { return; }
		GdiTransparentBlt(hDC, 5, 20, m_fBonusTimeGaugeDecrease, 35, hMemDC_BonusTime, 0, 0, m_fBonusTimeGaugeDecrease, 35, RGB(255, 0, 255));
	}

	BLENDFUNCTION	tBlend = {};
	tBlend.SourceConstantAlpha = 100; // 0 투명 ~ 255 불투명

	HDC hMemDC_Under = CBitmapManager::Get_Instance()->Find_Image(L"Gauge_Under");
	if (nullptr == hMemDC_Under) { return; }
	AlphaBlend(hDC, 295, 19, 915, 30, hMemDC_Under, 0, 0, 915, 30, tBlend);

	HDC hMemDC1 = CBitmapManager::Get_Instance()->Find_Image(L"Gauge");
	if (nullptr == hMemDC1) { return; }
	GdiTransparentBlt(hDC, 305, 14, m_iGauge, 40, hMemDC1, 0, 0, m_iGauge, 40, RGB(255, 0, 255));


	if (m_tBonusTime.B == true)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, 5, 20, 28, 35, hMemDC, 0, 0, 28, 35, RGB(255, 0, 255));
	}
	if (m_tBonusTime.O == true)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, 33, 20, 28, 35, hMemDC, 28, 0, 28, 35, RGB(255, 0, 255));
	}
	if (m_tBonusTime.N == true)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, 61, 20, 31, 35, hMemDC, 56, 0, 31, 35, RGB(255, 0, 255));
	}
	if (m_tBonusTime.U == true)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, 92, 20, 30, 35, hMemDC, 87, 0, 30, 35, RGB(255, 0, 255));
	}
	if (m_tBonusTime.S == true)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, 122, 20, 27, 35, hMemDC, 117, 0, 27, 35, RGB(255, 0, 255));
	}
	if (m_tBonusTime.T == true)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, 150, 20, 25, 35, hMemDC, 144, 0, 27, 35, RGB(255, 0, 255));
	}
	if (m_tBonusTime.I == true)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, 176, 20, 17, 35, hMemDC, 171, 0, 17, 35, RGB(255, 0, 255));
	}
	if (m_tBonusTime.M == true)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, 193, 20, 34, 35, hMemDC, 188, 0, 34, 35, RGB(255, 0, 255));
	}
	if (m_tBonusTime.E == true)
	{
		HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME_Color");
		if (nullptr == hMemDC) { return; }
		GdiTransparentBlt(hDC, 227, 20, 26, 35, hMemDC, 222, 0, 26, 35, RGB(255, 0, 255));
	}

	HDC hMemDC_GaugeIcon = CBitmapManager::Get_Instance()->Find_Image(L"GaugeIcon");
	if (nullptr == hMemDC_GaugeIcon) { return; }
	GdiTransparentBlt(hDC, 260, 3, 54, 64, hMemDC_GaugeIcon, 0, 0, 54, 64, RGB(255, 0, 255));


	HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Pause");
	if (nullptr == hMemDC) { return; }
	GdiTransparentBlt(hDC, 1230, 10, 43, 43, hMemDC, 0, 0, 43, 43, RGB(255, 0, 255));

	HDC hMemDC_Progress = CBitmapManager::Get_Instance()->Find_Image(L"Progress");
	GdiTransparentBlt(hDC, 430, 60, 485, 16, hMemDC_Progress, 0, 0, 485, 16, RGB(255, 0, 255));

	HDC hMemDC_ProgressIcon = CBitmapManager::Get_Instance()->Find_Image(L"Progress_Icon");
	GdiTransparentBlt(hDC, 427 + (int)m_fProgressX, 54, 24, 26, hMemDC_ProgressIcon, 0, 0, 24, 26, RGB(255, 0, 255));

}

void CUI::Release_GameObject()
{}

void CUI::Check_BonusTime()
{
	if (m_tBonusTime.B == true && m_tBonusTime.O == true && m_tBonusTime.N == true && m_tBonusTime.U == true && m_tBonusTime.S == true &&
		m_tBonusTime.T == true && m_tBonusTime.I == true && m_tBonusTime.M == true && m_tBonusTime.E == true)
	{
		CSoundManager::Get_Instance()->StopSound(CSoundManager::CHANNELID::BGM);
		CSoundManager::Get_Instance()->PlayBGM(L"BonusTime.mp3");
		m_bGaugeStop = true;
		m_bAlphaPlus_IN = true;
		m_bBonusTimeGauge = true;

		m_bTemp = true;
		m_bTemp2 = true;
		m_bTemp3 = true;
		m_bTemp4 = true;

		static_cast<CStage*>(CSceneManager::Get_Instance()->Get_Scene())->Set_BonusTime();
		static_cast<CPlayer*>(CGameObjectManager::Get_Instance()->Get_Player())->Set_StageType(STAGE::TYPE::BT_IN);

		for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Obstacle()))
		{
			static_cast<CObstacles*>(pObject)->Set_IsObstacleType(OBSTACLE::TYPE::END);
		}

		for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Jelly()))
		{
			static_cast<CJelly*>(pObject)->Set_JellyType(JELLY::TYPE::END);
		}
		for (auto pObject : *(CGameObjectManager::Get_Instance()->Get_Item()))
		{
			static_cast<CItem*>(pObject)->Set_ItemType(ITEM::TYPE::BLAST_EFFECT);
		}

		m_tBonusTime.B = false;
		m_tBonusTime.O = false;
		m_tBonusTime.N = false;
		m_tBonusTime.U = false;
		m_tBonusTime.S = false;
		m_tBonusTime.T = false;
		m_tBonusTime.I = false;
		m_tBonusTime.M = false;
		m_tBonusTime.E = false;

		m_tBonusTimeStart = GetTickCount64();
		m_tBonusTimeEnd = GetTickCount64();
	}
}

CGameObject* CUI::Create()
{
	CGameObject* pInstance = new CUI;

	if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

	return pInstance;
}