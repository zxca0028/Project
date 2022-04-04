#include "pch.h"
#include "Lobby.h"
#include "SceneManager.h"
#include "BitmapManager.h"
#include "CreateManager.h"

int CLobby::m_iHaveCoin = 0;

CLobby::CLobby()
	: m_bPlayButton(false)
	, m_bPlayScene(false)
{}

CLobby::~CLobby()
{
	Release_Scene();
}

int CLobby::Ready_Scene()
{
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;

	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount64();

	m_bPlayButton = false;
	m_bPlayScene = false;

	m_bCookiesButton = false;
	m_bCookiesScene = false;

	m_fX = 0;

	return READY_OK;
}

void CLobby::Update_Scene()
{
	Click();	

	if (CKeyManager::Get_Instance()->Key_Down(KEY_F1))
	{
		m_iHaveCoin += 999;
	}

	if (m_bPlayScene==true)
	{
		if (m_tPlayDelay + 100 < GetTickCount64())
		{
			CSceneManager::Get_Instance()->Scene_Change(SCENE::STAGELOADING);
		}
	}

	if (m_bCookiesScene == true)
	{
		if (m_tCookiesDelay + 100 < GetTickCount64())
		{
			CSceneManager::Get_Instance()->Scene_Change(SCENE::COOKIESELECT);
		}
	}

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

void CLobby::Render_Scene(HDC hDC)
{
	HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"Lobby");
	if (nullptr == hMemDC_BG) { return; }
	BitBlt(hDC, 0, 0, 1280, 720, hMemDC_BG, 0, 0, SRCCOPY);

	HDC hMemDC_PlayButton = CBitmapManager::Get_Instance()->Find_Image(L"PlayButton");
	if (nullptr == hMemDC_PlayButton) { return; }
	if (m_bPlayButton == false)
	{
		GdiTransparentBlt(hDC, 850, 560, 406, 133, hMemDC_PlayButton, 0, 0, 406, 133, RGB(255, 0, 255));
	}
	else
	{
		GdiTransparentBlt(hDC, 850, 560, 406, 133, hMemDC_PlayButton, 0, 133, 406, 133, RGB(255, 0, 255));
	}

	HDC hMemDC_CookiesButton = CBitmapManager::Get_Instance()->Find_Image(L"MyCookiesButton");
	if (nullptr == hMemDC_CookiesButton) { return; }
	if (m_bCookiesButton == false)
	{
		GdiTransparentBlt(hDC, 20, 600, 210, 98, hMemDC_CookiesButton, 0, 0, 210, 98, RGB(255, 0, 255));
	}
	else
	{
		GdiTransparentBlt(hDC, 20, 600, 210, 98, hMemDC_CookiesButton, 0, 98, 210, 98, RGB(255, 0, 255));
	}


	HDC hMemDC_Cookie_Stand = CBitmapManager::Get_Instance()->Find_Image(CCreateManager::Get_Instance()->Get_Cookie());
	if (nullptr == hMemDC_Cookie_Stand) { return; }
	GdiTransparentBlt
	(
		hDC,
		515,
		300,
		248,
		248,
		hMemDC_Cookie_Stand,
		m_tFrame.iFrameStart * 248,
		3960,
		248,
		248,
		RGB(255, 0, 255)
	);

	BLENDFUNCTION tBlend_CoinBG = {};
	tBlend_CoinBG.SourceConstantAlpha = 50;
	HDC hMemDC_SilverCoinBG = CBitmapManager::Get_Instance()->Find_Image(L"Coin_BG");
	if (nullptr == hMemDC_SilverCoinBG) { return; }
	GdiTransparentBlt(hDC, 31+ m_fX, 28, 174, 31, hMemDC_SilverCoinBG, 0, 0, 174, 37, RGB(255, 0, 255));
	
	HDC hMemDC_SilverCoin = CBitmapManager::Get_Instance()->Find_Image(L"Silver_Coin");
	if (nullptr == hMemDC_SilverCoin) { return; }
	GdiTransparentBlt(hDC, 15+ m_fX, 20, 50, 50, hMemDC_SilverCoin, 0, 0, 50, 50, RGB(255, 0, 255));
	
	TCHAR szGetCoin[32] = {};
	
	HFONT	hCoinFont, OldCoinFont;
	hCoinFont = CreateFont
	(
		38,
		0, 0, 0, 0, 0, 0, 0,
		HANGEUL_CHARSET,
		0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN,
		TEXT("CookieRun Black")
	);
	OldCoinFont = (HFONT)SelectObject(hDC, hCoinFont);
	
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	
	if (m_iHaveCoin < 10)
	{
		swprintf_s(szGetCoin, L"%d", m_iHaveCoin);
		TextOut(hDC, 170+ m_fX, 24, szGetCoin, lstrlen(szGetCoin));
	}
	else if (m_iHaveCoin < 100)
	{
		swprintf_s(szGetCoin, L"%d", m_iHaveCoin);
		TextOut(hDC, 155+ m_fX, 24, szGetCoin, lstrlen(szGetCoin));
	}
	else if (m_iHaveCoin < 1000)
	{
		swprintf_s(szGetCoin, L"%d", m_iHaveCoin);
		TextOut(hDC, 140+ m_fX, 24, szGetCoin, lstrlen(szGetCoin));
	}
	else if (m_iHaveCoin < 10000)
	{
		int iTemp_Front = m_iHaveCoin / 1000;
		int iTemp_Back  = m_iHaveCoin % 1000;
	
		swprintf_s(szGetCoin, L"%d,%03d", iTemp_Front, iTemp_Back);
		TextOut(hDC, 115+ m_fX, 24, szGetCoin, lstrlen(szGetCoin));
	}
	else if (m_iHaveCoin < 100000)
	{
		int iTemp_Front = m_iHaveCoin / 1000;
		int iTemp_Back = m_iHaveCoin % 1000;
	
		swprintf_s(szGetCoin, L"%d,%03d", iTemp_Front, iTemp_Back);
		TextOut(hDC, 100+ m_fX, 24, szGetCoin, lstrlen(szGetCoin));
	}
	else if (m_iHaveCoin < 1000000)
	{
		int iTemp_Front = m_iHaveCoin / 1000;
		int iTemp_Back = m_iHaveCoin % 1000;
	
		swprintf_s(szGetCoin, L"%d,%03d", iTemp_Front, iTemp_Back);
		TextOut(hDC, 85+ m_fX, 24, szGetCoin, lstrlen(szGetCoin));
	}
	
	SelectObject(hDC, OldCoinFont);
	DeleteObject(hCoinFont);
}

void CLobby::Release_Scene()
{}

CScene* CLobby::Create()
{
	CScene* pInstance = new CLobby;

	if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

	return pInstance;

}

void CLobby::Click()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_LB))
	{
		POINT	pt_Cookies = {};
		RECT	tCookiesButton = { 20, 600, 230, 693 };

		GetCursorPos(&pt_Cookies);
		ScreenToClient(g_hWnd, &pt_Cookies);

		if (PtInRect(&tCookiesButton, pt_Cookies))
		{
			m_bCookiesButton = true;
			m_bCookiesScene = true;

			m_tCookiesDelay = GetTickCount64();
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
		}
		else
		{
			m_bCookiesScene = false;
			m_bCookiesButton = false;
		}

		POINT	pt_Play = {};
		RECT	tPlayButton = { 850,560,1256,693 };

		GetCursorPos(&pt_Play);
		ScreenToClient(g_hWnd, &pt_Play);

		if (PtInRect(&tPlayButton, pt_Play))
		{
			m_bPlayButton = true;
			m_bPlayScene = true;

			m_tPlayDelay = GetTickCount64();
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
		}
		else
		{
			m_bPlayScene = false;
			m_bPlayButton = false;
		}
	}
}
