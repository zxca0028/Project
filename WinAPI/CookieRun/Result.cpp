#include "pch.h"
#include "Result.h"
#include "SceneManager.h"
#include "BitmapManager.h"
#include "Score.h"
#include "Lobby.h"

CResult::CResult()
	: m_tInfo({})
	, m_tRect({})
	, m_bDownLock(false)
	, m_bUpLock(false)
	, m_bScoreRender(false)
	, m_bPlusLock(false)
	, m_iTemp(0)
	, m_iMoveX(0)
	, m_iMoveY(0)
	, m_bHiScoreRender(false)
	, m_bCoinRender(false)
	, m_iAlpha(0)
	, m_bLobbyScene(false)
	, m_bOKButton(false)
	, m_iCoinTemp(0)
	, m_bCoinTemp(false)
{}

CResult::~CResult()
{
	Release_Scene();
}

int CResult::Ready_Scene()
{
	m_tInfo.fX = 640.f;
	m_tInfo.fY = -100.f;

	m_tInfo.iCX = 330;
	m_tInfo.iCY = 110;

	m_iMoveX = -670;
	m_iMoveY = 150;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;

	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount64();
	//CSoundManager::Get_Instance()->PlaySound(L"Finish.mp3", CSoundManager::CHANNELID::FINISH);

	return READY_OK;
}

void CResult::Update_Scene()
{
	m_tRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX >> 1));
	m_tRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY >> 1));
	m_tRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX >> 1));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY >> 1));

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount64())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount64();
	}

	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
	{
		m_tFrame.iFrameStart = 0;
	}

	POINT	pt_OK = {};
	RECT	tPlayButton = { 488, 595, 794, 700 };

	GetCursorPos(&pt_OK);
	ScreenToClient(g_hWnd, &pt_OK);

	if (CKeyManager::Get_Instance()->Key_Pressing(KEY_LB))
	{
		if (PtInRect(&tPlayButton, pt_OK))
		{
			m_bOKButton = true;
			m_bLobbyScene = true;

			m_bLobbyDelay = GetTickCount64();
			
		}
		else
		{
			m_bLobbyScene = false;
		}
	}
	else
	{
		m_bOKButton = false;
	}

	if (m_bLobbyScene == true)
	{
		if (m_bLobbyDelay + 100 < GetTickCount64())
		{
			CSceneManager::Get_Instance()->Scene_Change(SCENE::TYPE::LOBBY);
		}
	}

	// 서순 : 게임 결과가 재생되고 나서 점수가 출력된 후 ( 최고점수라면 최고점수 표시 )
	// 획득 코인이 뜨고 표시가 완료되면 확인 버튼

	// 게임결과 재생
	if (m_tInfo.fY < 150 && m_bDownLock == false)
	{
		m_tInfo.fY += 10;

		if (m_tInfo.fY >= 150)
		{
			m_bDownLock = true;
		}
	}
	if (m_tInfo.fY > 120 && m_bUpLock == false && m_bDownLock == true )
	{
		m_tInfo.fY -= 5;

		if (m_tInfo.fY <= 120)
		{
			m_bUpLock = true;
			m_bScoreRender = true;
		}
	}

	// 점수 재생
	if (m_bScoreRender == true && m_bPlusLock == false)
	{
		m_iTemp += 799999;

		if (m_iTemp > CScore::m_iCurrentScore)
		{
			m_iTemp = CScore::m_iCurrentScore;
			m_bPlusLock = true;
			m_bHiScoreRender = true;
			m_bCoinRenderDelay = GetTickCount64();
		}
	}

	if (m_bCoinRenderDelay + 750 < GetTickCount64())
	{
		m_bCoinRender = true;
	}

	if (m_bCoinRender == true)
	{
		m_iAlpha += 5;

		if (m_iAlpha > 255)
		{
			m_iAlpha = 255;
		}

		if (m_iCoinTemp <= CScore::m_iGetCoin && m_bCoinTemp == false)
		{
			m_iCoinTemp += 24;
		}
		else if (m_iCoinTemp >= CScore::m_iGetCoin)
		{
			m_iCoinTemp = CScore::m_iGetCoin;
			m_bCoinTemp = true;
		}
	}
}

void CResult::Render_Scene(HDC hDC)
{
	m_tRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX >> 1));
	m_tRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY >> 1));
	m_tRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX >> 1));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY >> 1));

	HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"Result_BG");
	if (nullptr == hMemDC_BG) { return; }
	BitBlt(hDC, 0, 0, 1280, 720, hMemDC_BG, 0, 0, SRCCOPY);

	HDC hMemDC_KR = CBitmapManager::Get_Instance()->Find_Image(L"Result_KR");
	if (nullptr == hMemDC_KR) { return; }
	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC_KR, 0, 0, m_tInfo.iCX, m_tInfo.iCY, RGB(255, 0, 255));

	if (m_bScoreRender == true)
	{
		TCHAR m_szScore[32];

		HFONT	hScoreFont, OldScoreFont;

		hScoreFont = CreateFont
		(
			100,
			0, 0, 0, 0, 0, 0, 0,
			HANGEUL_CHARSET,
			0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN,
			TEXT("CookieRun Black")
		);
		OldScoreFont = (HFONT)SelectObject(hDC, hScoreFont);

		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);

		if (m_iTemp < 10)
		{
			swprintf_s(m_szScore, L"%d", m_iTemp);
			TextOut(hDC, 1250 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		}
		else if (m_iTemp < 100)
		{
			swprintf_s(m_szScore, L"%d", m_iTemp);
			TextOut(hDC, 1235 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		}
		else if (m_iTemp < 1000)
		{
			swprintf_s(m_szScore, L"%d", m_iTemp);
			TextOut(hDC, 1220 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		}
		else if (m_iTemp < 10000)
		{
			int iTemp_Front = m_iTemp / 1000;
			int iTemp_Back = m_iTemp % 1000;

			swprintf_s(m_szScore, L"%d,%03d", iTemp_Front, iTemp_Back);
			TextOut(hDC, 1195 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		}
		else if (m_iTemp < 100000)
		{
			int iTemp_Front = m_iTemp / 1000;
			int iTemp_Back = m_iTemp % 1000;

			swprintf_s(m_szScore, L"%d,%03d", iTemp_Front, iTemp_Back);
			TextOut(hDC, 1175 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		}
		else if (m_iTemp < 1000000)
		{
			int iTemp_Front = m_iTemp / 1000;
			int iTemp_Back = m_iTemp % 1000;

			swprintf_s(m_szScore, L"%d,%03d", iTemp_Front, iTemp_Back);
			TextOut(hDC, 1155 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		}
		else if (m_iTemp < 10000000)
		{
			int iTemp_Front = m_iTemp / 1000000;
			int iTemp_Mid = (m_iTemp % 1000000) / 1000;
			int iTemp_Back = m_iTemp % 1000;

			swprintf_s(m_szScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
			TextOut(hDC, 1135 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		}
		else if (m_iTemp < 100000000)
		{
			int iTemp_Front = m_iTemp / 1000000;
			int iTemp_Mid = (m_iTemp % 1000000) / 1000;
			int iTemp_Back = m_iTemp % 1000;

			swprintf_s(m_szScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
			TextOut(hDC, 1115 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		}
		else if (m_iTemp < 1000000000)
		{
			int iTemp_Front = m_iTemp / 1000000;
			int iTemp_Mid = (m_iTemp % 1000000) / 1000;
			int iTemp_Back = m_iTemp % 1000;

			swprintf_s(m_szScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
			TextOut(hDC, 1098 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		}

		SelectObject(hDC, OldScoreFont);
		DeleteObject(hScoreFont);
	}

	if (m_bHiScoreRender == true && CScore::m_iHiScoreBuffer <= CScore::m_iCurrentScore)
	{
		BLENDFUNCTION	tBlend_HiScore = {};
		HDC hMemDC_HiScore = CBitmapManager::Get_Instance()->Find_Image(L"Result_HiScore");
		if (nullptr == hMemDC_HiScore) { return; }
		tBlend_HiScore.SourceConstantAlpha = m_iAlpha;
		AlphaBlend(hDC, 185, 190, 210, 200, hMemDC_HiScore, 0, 0, 210, 200, tBlend_HiScore);
	}

	if (m_bCoinRender == true)
	{
		BLENDFUNCTION	tBlend_Coin = {};
		HDC hMemDC_Coin = CBitmapManager::Get_Instance()->Find_Image(L"Result_Coin");
		if (nullptr == hMemDC_Coin) { return; }
		tBlend_Coin.SourceConstantAlpha = m_iAlpha;
		AlphaBlend(hDC, 526, 455, 230, 30, hMemDC_Coin, 0, 0, 230, 30, tBlend_Coin);

		TCHAR szGetCoin[32] = {};

		HFONT	hCoinFont, OldCoinFont;
		
		hCoinFont = CreateFont
		(
			50,
			0, 0, 0, 0, 0, 0, 0,
			HANGEUL_CHARSET,
			0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN,
			TEXT("CookieRun Black")
		);
		OldCoinFont = (HFONT)SelectObject(hDC, hCoinFont);

		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);

		if (m_iCoinTemp < 10)
		{
			swprintf_s(szGetCoin, L"%d", m_iCoinTemp);
			TextOut(hDC, 635, 497, szGetCoin, lstrlen(szGetCoin));
		}
		else if (m_iCoinTemp < 100)
		{
			swprintf_s(szGetCoin, L"%d", m_iCoinTemp);
			TextOut(hDC, 625, 497, szGetCoin, lstrlen(szGetCoin));
		}
		else if (m_iCoinTemp < 1000)
		{
			swprintf_s(szGetCoin, L"%d", m_iCoinTemp);
			TextOut(hDC, 615, 497, szGetCoin, lstrlen(szGetCoin));
		}
		else if (m_iCoinTemp < 10000)
		{
			int iTemp_Front = m_iCoinTemp / 1000;
			int iTemp_Back = m_iCoinTemp % 1000;

			swprintf_s(szGetCoin, L"%d,%03d", iTemp_Front, iTemp_Back);
			TextOut(hDC, 605, 497, szGetCoin, lstrlen(szGetCoin));
		}
		else if (m_iCoinTemp < 100000)
		{
			int iTemp_Front = m_iCoinTemp / 1000;
			int iTemp_Back = m_iCoinTemp % 1000;

			swprintf_s(szGetCoin, L"%d,%03d", iTemp_Front, iTemp_Back);
			TextOut(hDC, 595, 497, szGetCoin, lstrlen(szGetCoin));
		}
		else if (m_iCoinTemp < 1000000)
		{
			int iTemp_Front = m_iCoinTemp / 1000;
			int iTemp_Back = m_iCoinTemp % 1000;

			swprintf_s(szGetCoin, L"%d,%03d", iTemp_Front, iTemp_Back);
			TextOut(hDC, 585, 497, szGetCoin, lstrlen(szGetCoin));
		}

		SelectObject(hDC, OldCoinFont);
		DeleteObject(hCoinFont);

		BLENDFUNCTION	tBlend_OK = {};
		HDC hMemDC_Button = CBitmapManager::Get_Instance()->Find_Image(L"Result_Button");
		if (nullptr == hMemDC_Button) { return; }
		tBlend_OK.SourceConstantAlpha = m_iAlpha;
		if (m_bOKButton == false)
		{
			AlphaBlend(hDC, 493, 595, 304, 105, hMemDC_Button, 0, 0, 304, 105, tBlend_OK);
		}
		else
		{
			AlphaBlend(hDC, 493, 595, 304, 105, hMemDC_Button, 0, 105, 304, 105, tBlend_OK);
		}
		

		BLENDFUNCTION	tBlend_CoinImage = {};
		HDC hMemDC_CoinImage = CBitmapManager::Get_Instance()->Find_Image(L"Result_CoinImage_test");
		if (nullptr == hMemDC_CoinImage) { return; }
		tBlend_CoinImage.SourceConstantAlpha = m_iAlpha;
		AlphaBlend(hDC, 533, 493, 60, 60, hMemDC_CoinImage, m_tFrame.iFrameStart * 60, 0, 60, 60, tBlend_CoinImage);
	}


}

void CResult::Release_Scene()
{
	CSoundManager::Get_Instance()->PlayBGM(L"Lobby.mp3");
	CLobby::m_iHaveCoin += CScore::m_iGetCoin;
	CScore::m_iGetCoin = 0;
}

CScene* CResult::Create()
{
	CScene* pInstance = new CResult;

	if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

	return pInstance;

}
