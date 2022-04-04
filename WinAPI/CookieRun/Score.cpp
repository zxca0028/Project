#include "pch.h"
#include "Score.h"
#include "BitmapManager.h"
#include "CreateManager.h"
#include "Result.h"

int CScore::m_iCurrentScore = 0;
int CScore::m_iHiScore = 0;
bool CScore::m_bFirstCycle = false;
int CScore::m_iHiScoreBuffer = 0;
int CScore::m_iGetCoin = 0;

CScore::CScore()
	:m_iJellyX(0)	
{}

CScore::~CScore()
{
    Release_GameObject();
}

int CScore::Ready_GameObject()
{
	m_iMoveX = -600;
	m_iMoveY = -55;

	m_iCurrentScore = 0;

    return READY_OK;
}

int CScore::Update_GameObject()
{
	if (m_bFirstCycle == false)
	{
		m_iHiScore = m_iCurrentScore;
	}
	else if (m_bFirstCycle == true)
	{
		m_iHiScore = CScore::m_iHiScoreBuffer;
	}

    return OBJ_NO_EVENT;
}

void CScore::Late_Update_GameObject()
{
}

void CScore::Render_GameObject(HDC hDC)
{
	HDC hMemDC_SilverCoin = CBitmapManager::Get_Instance()->Find_Image(L"Silver_Coin");
	if (nullptr == hMemDC_SilverCoin) { return; }
	GdiTransparentBlt(hDC, 10, 60, 40, 40, hMemDC_SilverCoin, 0, 0, 50, 50, RGB(255, 0, 255));

	TCHAR szGetCoin[32] = {};

	HFONT	hCoinFont, OldCoinFont;
	swprintf_s(szGetCoin, L"%d", m_iGetCoin);
	hCoinFont = CreateFont
	(
		40,
		0, 0, 0, 0, 0, 0, 0,
		HANGEUL_CHARSET,
		0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN,
		TEXT("CookieRun Black")
	);
	OldCoinFont = (HFONT)SelectObject(hDC, hCoinFont);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	TextOut(hDC, 60, 60, szGetCoin, lstrlen(szGetCoin));

	SelectObject(hDC, OldCoinFont);
	DeleteObject(hCoinFont);

	if (m_bFirstCycle == true && m_iCurrentScore >= m_iHiScore)
	{
		BLENDFUNCTION	tBlend = {};

		HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"Score_BG");
		if (nullptr == hMemDC_BG) { return; }
		for (int i = 0; i < 300; ++i)
		{
			tBlend.SourceConstantAlpha = i / 3;
			AlphaBlend(hDC, 980 + i, 100, 1, 80, hMemDC_BG, 0, 0, 1, 80, tBlend);
		}

		HDC hMemDC_NewRecord = CBitmapManager::Get_Instance()->Find_Image(L"NewRecord");
		if (nullptr == hMemDC_NewRecord) { return; }
		GdiTransparentBlt(hDC, 1080, 90, 180, 100, hMemDC_NewRecord, 0, 0, 180, 100, RGB(255, 0, 255));
	}
	else
	{
		BLENDFUNCTION	tBlend = {};

		HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"Score_BG");
		if (nullptr == hMemDC_BG) { return; }
		for (int i = 0; i < 300; ++i)
		{
			tBlend.SourceConstantAlpha = i / 3;
			AlphaBlend(hDC, 980 + i, 100, 1, 80, hMemDC_BG, 0, 0, 1, 80, tBlend);
		}

		TCHAR m_szTitle[32];

		HFONT	hTitleFont, OldTitleFont;
		swprintf_s(m_szTitle, L"내 최고점수");
		hTitleFont = CreateFont
		(
			25,
			0, 0, 0, 0, 0, 0, 0,
			HANGEUL_CHARSET,
			0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN,
			TEXT("CookieRun Black")
		);
		OldTitleFont = (HFONT)SelectObject(hDC, hTitleFont);

		SetTextColor(hDC, RGB(244, 210, 72));
		SetBkMode(hDC, TRANSPARENT);

		TextOut(hDC, 1185, 113, m_szTitle, lstrlen(m_szTitle));

		SelectObject(hDC, OldTitleFont);
		DeleteObject(hTitleFont);


		// 여기서는 실시간 점수가 아닌 최고점수를 표시 ( MyBestScore )
		// 실시간 점수는 체력바 중앙 아래쪽에 따로 표시
		TCHAR m_szHighScore[32];

		HFONT	hHighScoreFont, OldHighScoreFont;

		hHighScoreFont = CreateFont
		(
			40,
			0, 0, 0, 0, 0, 0, 0,
			HANGEUL_CHARSET,
			0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN,
			TEXT("CookieRun Black")
		);
		OldHighScoreFont = (HFONT)SelectObject(hDC, hHighScoreFont);

		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);

		if (m_iHiScore < 10)
		{
			swprintf_s(m_szHighScore, L"%d", m_iHiScore);
			TextOut(hDC, 1250, 135, m_szHighScore, lstrlen(m_szHighScore));
		}
		else if (m_iHiScore < 100)
		{
			swprintf_s(m_szHighScore, L"%d", m_iHiScore);
			TextOut(hDC, 1235, 135, m_szHighScore, lstrlen(m_szHighScore));
		}
		else if (m_iHiScore < 1000)
		{
			swprintf_s(m_szHighScore, L"%d", m_iHiScore);
			TextOut(hDC, 1220, 135, m_szHighScore, lstrlen(m_szHighScore));
		}
		else if (m_iHiScore < 10000)
		{
			int iTemp_Front = m_iHiScore / 1000;
			int iTemp_Back = m_iHiScore % 1000;

			swprintf_s(m_szHighScore, L"%d,%03d", iTemp_Front, iTemp_Back);
			TextOut(hDC, 1195, 135, m_szHighScore, lstrlen(m_szHighScore));
		}
		else if (m_iHiScore < 100000)
		{
			int iTemp_Front = m_iHiScore / 1000;
			int iTemp_Back = m_iHiScore % 1000;

			swprintf_s(m_szHighScore, L"%d,%03d", iTemp_Front, iTemp_Back);
			TextOut(hDC, 1175, 135, m_szHighScore, lstrlen(m_szHighScore));
		}
		else if (m_iHiScore < 1000000)
		{
			int iTemp_Front = m_iHiScore / 1000;
			int iTemp_Back = m_iHiScore % 1000;

			swprintf_s(m_szHighScore, L"%d,%03d", iTemp_Front, iTemp_Back);
			TextOut(hDC, 1155, 135, m_szHighScore, lstrlen(m_szHighScore));
		}
		else if (m_iHiScore < 10000000)
		{
			int iTemp_Front = m_iHiScore / 1000000;
			int iTemp_Mid = (m_iHiScore % 1000000) / 1000;
			int iTemp_Back = m_iHiScore % 1000;

			swprintf_s(m_szHighScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
			TextOut(hDC, 1135, 135, m_szHighScore, lstrlen(m_szHighScore));
		}
		else if (m_iHiScore < 100000000)
		{
			int iTemp_Front = m_iHiScore / 1000000;
			int iTemp_Mid = (m_iHiScore % 1000000) / 1000;
			int iTemp_Back = m_iHiScore % 1000;

			swprintf_s(m_szHighScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
			TextOut(hDC, 1115, 135, m_szHighScore, lstrlen(m_szHighScore));
		}
		else if (m_iHiScore < 1000000000)
		{
			int iTemp_Front = m_iHiScore / 1000000;
			int iTemp_Mid = (m_iHiScore % 1000000) / 1000;
			int iTemp_Back = m_iHiScore % 1000;

			swprintf_s(m_szHighScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
			TextOut(hDC, 1098, 135, m_szHighScore, lstrlen(m_szHighScore));
		}

		SelectObject(hDC, OldHighScoreFont);
		DeleteObject(hHighScoreFont);
	}	


	// 스테이지 점수

	TCHAR m_szScore[32];

	HFONT	hScoreFont, OldScoreFont;

	hScoreFont = CreateFont
	(
		40,
		0, 0, 0, 0, 0, 0, 0,
		HANGEUL_CHARSET,
		0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN,
		TEXT("CookieRun Black")
	);
	OldScoreFont = (HFONT)SelectObject(hDC, hScoreFont);

	if (m_bFirstCycle == true && m_iCurrentScore >= m_iHiScore)
	{
		HDC hMemDC_Best = CBitmapManager::Get_Instance()->Find_Image(L"BEST");
		if (nullptr == hMemDC_Best) { return; }
		GdiTransparentBlt(hDC, 520 - m_iJellyX, 80, 100, 40, hMemDC_Best, 0, 0, 100, 40, RGB(255, 0, 255));
	}

	HDC hMemDC_Jelly = CBitmapManager::Get_Instance()->Find_Image(L"ScoreJelly");
	if (nullptr == hMemDC_Jelly) { return; }
	GdiTransparentBlt(hDC, 620 - m_iJellyX, 80, 30, 40, hMemDC_Jelly, 0, 0, 30, 40, RGB(255, 0, 255));
	
	if (m_bFirstCycle == true && m_iCurrentScore >= m_iHiScore)
	{
		SetTextColor(hDC, RGB(255, 255, 0));
		SetBkMode(hDC, TRANSPARENT);
	}
	else
	{
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);
	}
	

	if (m_iCurrentScore < 10)
	{
		swprintf_s(m_szScore, L"%d", m_iCurrentScore);
		TextOut(hDC, 1250 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		m_iJellyX = 10;
	}
	else if (m_iCurrentScore < 100)
	{
		swprintf_s(m_szScore, L"%d", m_iCurrentScore);
		TextOut(hDC, 1240 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		m_iJellyX = 20;
	}
	else if (m_iCurrentScore < 1000)
	{
		swprintf_s(m_szScore, L"%d", m_iCurrentScore);
		TextOut(hDC, 1230 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		m_iJellyX = 30;
	}
	else if (m_iCurrentScore < 10000)
	{
		int iTemp_Front = m_iCurrentScore / 1000;
		int iTemp_Back  = m_iCurrentScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d", iTemp_Front, iTemp_Back);
		TextOut(hDC, 1220 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		m_iJellyX = 40;
	}
	else if (m_iCurrentScore < 100000)
	{
		int iTemp_Front = m_iCurrentScore / 1000;
		int iTemp_Back = m_iCurrentScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d", iTemp_Front, iTemp_Back);
		TextOut(hDC, 1210 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		m_iJellyX = 50;
	}
	else if (m_iCurrentScore < 1000000)
	{
		int iTemp_Front = m_iCurrentScore / 1000;
		int iTemp_Back = m_iCurrentScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d", iTemp_Front, iTemp_Back);
		TextOut(hDC, 1200 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		m_iJellyX = 60;
	}
	else if (m_iCurrentScore < 10000000)
	{
		int iTemp_Front = m_iCurrentScore / 1000000;
		int iTemp_Mid = (m_iCurrentScore % 1000000) / 1000;
		int iTemp_Back = m_iCurrentScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
		TextOut(hDC, 1190 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		m_iJellyX = 70;
	}
	else if (m_iCurrentScore < 100000000)
	{
		int iTemp_Front = m_iCurrentScore / 1000000;
		int iTemp_Mid = (m_iCurrentScore % 1000000) / 1000;
		int iTemp_Back = m_iCurrentScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
		TextOut(hDC, 1180 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		m_iJellyX = 80;
	}
	else if (m_iCurrentScore < 1000000000)
	{
		int iTemp_Front = m_iCurrentScore / 1000000;
		int iTemp_Mid = (m_iCurrentScore % 1000000) / 1000;
		int iTemp_Back = m_iCurrentScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
		TextOut(hDC, 1170 + m_iMoveX, 135 + m_iMoveY, m_szScore, lstrlen(m_szScore));
		m_iJellyX = 90;
	}
	SelectObject(hDC, OldScoreFont);
	DeleteObject(hScoreFont);
}

void CScore::Release_GameObject()
{
	m_iCurrentScore = 0;
}

CGameObject* CScore::Create()
{
    CGameObject* pInstance = new CScore;

    if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

    return pInstance;
}
