#include "pch.h"
#include "RenderTest.h"
#include "SceneManager.h"
#include "BitmapManager.h"

CRenderTest::CRenderTest()
{}

CRenderTest::~CRenderTest()
{
	Release_Scene();
}

int CRenderTest::Ready_Scene()
{
	m_iHiScore = 123456789;
	return READY_OK;
}

void CRenderTest::Update_Scene()
{

}

void CRenderTest::Render_Scene(HDC hDC)
{
	HFONT	hFont, OldFont;

	hFont = CreateFont(
		50,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		HANGEUL_CHARSET,
		0,
		0,
		0,
		VARIABLE_PITCH | FF_ROMAN,
		TEXT("CookieRun Black")
	);
	OldFont = (HFONT)SelectObject(hDC, hFont);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	TextOut(hDC, 30, 15, L"Render Test", 11);

	SelectObject(hDC, OldFont);
	DeleteObject(hFont);

	HDC hMemDC_BG_1 = CBitmapManager::Get_Instance()->Find_Image(L"BG_1");
	if (nullptr == hMemDC_BG_1) { return; }
	BitBlt(hDC, 0, 0, 1280, 720, hMemDC_BG_1, 0, 0, SRCCOPY);

	HDC hMemDC_BG_2 = CBitmapManager::Get_Instance()->Find_Image(L"BG_2");
	if (nullptr == hMemDC_BG_2) { return; }
	GdiTransparentBlt(hDC, 0, 0, 1280, 720, hMemDC_BG_2, 0, 0, 1280, 720, RGB(255, 0, 255));

	HDC hMemDC_Pause = CBitmapManager::Get_Instance()->Find_Image(L"Pause");
	if (nullptr == hMemDC_Pause) { return; }
	GdiTransparentBlt(hDC, 1230, 10, 43, 43, hMemDC_Pause, 0, 0, 43, 43, RGB(255, 0, 255));


	HDC hMemDC_BonusTime = CBitmapManager::Get_Instance()->Find_Image(L"BONUSTIME");
	if (nullptr == hMemDC_BonusTime) { return; }
	GdiTransparentBlt(hDC, 5, 20, 248, 35, hMemDC_BonusTime, 0, 0, 248, 35, RGB(255, 0, 255));

	BLENDFUNCTION	tBlend = {};
	tBlend.SourceConstantAlpha = 100; // 0 투명 ~ 255 불투명

	HDC hMemDC_GaugeUnder = CBitmapManager::Get_Instance()->Find_Image(L"Gauge_Under");
	if (nullptr == hMemDC_GaugeUnder) { return; }
	AlphaBlend(hDC, 295, 19, 915, 30, hMemDC_GaugeUnder, 0, 0, 915, 30, tBlend);

	HDC hMemDC_Gauge = CBitmapManager::Get_Instance()->Find_Image(L"Gauge");
	if (nullptr == hMemDC_Gauge) { return; }
	GdiTransparentBlt(hDC, 305, 14, 915, 40, hMemDC_Gauge, 0, 0, 915, 40, RGB(255, 0, 255));

	HDC hMemDC_GaugeIcon = CBitmapManager::Get_Instance()->Find_Image(L"GaugeIcon");
	if (nullptr == hMemDC_GaugeIcon) { return; }
	GdiTransparentBlt(hDC, 260, 3, 54, 64, hMemDC_GaugeIcon, 0, 0, 54, 64, RGB(255, 0, 255));

	for (int i = 0; i < 4; ++i)
	{
		HDC hMemDC_Ground = CBitmapManager::Get_Instance()->Find_Image(L"Ground");
		if (nullptr == hMemDC_Ground) { return; }
		GdiTransparentBlt(hDC, 0 + 372 * i, 580, 372, 140, hMemDC_Ground, 0, 0, 372, 124, RGB(255, 0, 255));
	}

	HDC hMemDC_Player = CBitmapManager::Get_Instance()->Find_Image(L"BlackBerryCookie");
	if (nullptr == hMemDC_Player) { return; }
	GdiTransparentBlt(hDC, 0, 220, 360, 360, hMemDC_Player, 0,0, 360, 360, RGB(255, 0, 255));

	HDC hMemDC_ScoreJelly = CBitmapManager::Get_Instance()->Find_Image(L"ScoreJelly");
	if (nullptr == hMemDC_ScoreJelly) { return; }
	GdiTransparentBlt(hDC, 500, 60, 30, 40, hMemDC_ScoreJelly, 0, 0, 30, 40, RGB(255, 0, 255));

	BLENDFUNCTION	tBlend_BG = {};

	HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"Score_BG");
	if (nullptr == hMemDC_BG) { return; }
	for (int i = 0; i < 300; ++i)
	{
		tBlend_BG.SourceConstantAlpha = i / 3;
		AlphaBlend(hDC, 980 + i, 100, 1, 80, hMemDC_BG, 0, 0, 1, 80, tBlend_BG);
	}

	HDC hMemDC_BEST = CBitmapManager::Get_Instance()->Find_Image(L"BEST");
	if (nullptr == hMemDC_BEST) { return; }
	GdiTransparentBlt(hDC, 400, 60, 100, 40, hMemDC_BEST, 0, 0, 100, 40, RGB(255, 0, 255));

	HDC hMemDC_NewRecord = CBitmapManager::Get_Instance()->Find_Image(L"NewRecord");
	if (nullptr == hMemDC_NewRecord) { return; }
	GdiTransparentBlt(hDC, 1090, 88, 180, 100, hMemDC_NewRecord, 0, 0, 180, 100, RGB(255, 0, 255));

	//MyBestScore(hDC);

	HDC test = CBitmapManager::Get_Instance()->Find_Image(L"Progress");
	GdiTransparentBlt(hDC, 430, 60, 485, 16, test, 0, 0, 485, 16, RGB(255, 0, 255));

	HDC test1 = CBitmapManager::Get_Instance()->Find_Image(L"Progress_Icon");
	GdiTransparentBlt(hDC, 427, 54, 24, 26, test1, 0, 0, 24, 26, RGB(255, 0, 255));
}

void CRenderTest::Release_Scene()
{}

CScene* CRenderTest::Create()
{
	CScene* pInstance = new CRenderTest;

	if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

	return pInstance;
}

void CRenderTest::MyBestScore(HDC hDC)
{
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

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	if (m_iHiScore < 10)
	{
		swprintf_s(m_szScore, L"%d", m_iHiScore);
		TextOut(hDC, 1250, 135, m_szScore, lstrlen(m_szScore));
	}
	else if (m_iHiScore < 100)
	{
		swprintf_s(m_szScore, L"%d", m_iHiScore);
		TextOut(hDC, 1235, 135, m_szScore, lstrlen(m_szScore));
	}
	else if (m_iHiScore < 1000)
	{
		swprintf_s(m_szScore, L"%d", m_iHiScore);
		TextOut(hDC, 1220, 135, m_szScore, lstrlen(m_szScore));
	}
	else if (m_iHiScore < 10000)
	{
		int iTemp_Front = m_iHiScore / 1000;
		int iTemp_Back = m_iHiScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d", iTemp_Front, iTemp_Back);
		TextOut(hDC, 1195, 135, m_szScore, lstrlen(m_szScore));
	}
	else if (m_iHiScore < 100000)
	{
		int iTemp_Front = m_iHiScore / 1000;
		int iTemp_Back = m_iHiScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d", iTemp_Front, iTemp_Back);
		TextOut(hDC, 1175, 135, m_szScore, lstrlen(m_szScore));
	}
	else if (m_iHiScore < 1000000)
	{
		int iTemp_Front = m_iHiScore / 1000;
		int iTemp_Back = m_iHiScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d", iTemp_Front, iTemp_Back);
		TextOut(hDC, 1155, 135, m_szScore, lstrlen(m_szScore));
	}
	else if (m_iHiScore < 10000000)
	{
		int iTemp_Front = m_iHiScore / 1000000;
		int iTemp_Mid = (m_iHiScore % 1000000) / 1000;
		int iTemp_Back = m_iHiScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
		TextOut(hDC, 1135, 135, m_szScore, lstrlen(m_szScore));
	}
	else if (m_iHiScore < 100000000)
	{
		int iTemp_Front = m_iHiScore / 1000000;
		int iTemp_Mid = (m_iHiScore % 1000000) / 1000;
		int iTemp_Back = m_iHiScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
		TextOut(hDC, 1115, 135, m_szScore, lstrlen(m_szScore));
	}
	else if (m_iHiScore < 1000000000)
	{
		int iTemp_Front = m_iHiScore / 1000000;
		int iTemp_Mid = (m_iHiScore % 1000000) / 1000;
		int iTemp_Back = m_iHiScore % 1000;

		swprintf_s(m_szScore, L"%d,%03d,%03d", iTemp_Front, iTemp_Mid, iTemp_Back);
		TextOut(hDC, 1098, 135, m_szScore, lstrlen(m_szScore));
	}


	SelectObject(hDC, OldScoreFont);
	DeleteObject(hScoreFont);
}