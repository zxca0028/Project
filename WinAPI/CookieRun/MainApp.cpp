#include "pch.h"
#include "MainApp.h"
#include "Controller.h"

#include "SceneManager.h"
#include "GameObjectManager.h"
#include "BitmapManager.h"
#include "KeyManager.h"
#include "CreateManager.h"
#include "InventoryManager.h"
#include "SoundManager.h"

#include "GameObject.h"
#include "Player.h"

CMainApp::CMainApp()
	: m_hDC()
	, m_tFPS_Time(0)
	, m_szFPS(L"")
	, m_iFPS(0)
	, m_bIs_FPS_View(false)
{}

CMainApp::~CMainApp()
{
	Release_MainApp();
}

int CMainApp::Ready_MainApp()
{
	CSoundManager::Get_Instance()->Initialize();
	m_hDC = GetDC(g_hWnd);
	m_tFPS_Time = GetTickCount64();

	CBitmapManager::Get_Instance()->Load_Bitmap();

	if (SEQUENCE == 0)
	{
		CSceneManager::Get_Instance()->Scene_Change(SCENE::LOADING);
	}
	else if (SEQUENCE == 1)
	{
		//렌더 테스트
		CSceneManager::Get_Instance()->Scene_Change(SCENE::RENDERTEST);
	}
	else if (SEQUENCE == 2)
	{
		// 스테이지 바로 진입
		CSceneManager::Get_Instance()->Scene_Change(SCENE::STAGE);
	}
	else if (SEQUENCE == 3)
	{
		// 스테이지 로딩
		CSceneManager::Get_Instance()->Scene_Change(SCENE::STAGELOADING);
	}
	else if (SEQUENCE == 4)
	{
		// 결과창
		CSceneManager::Get_Instance()->Scene_Change(SCENE::RESULT);
	}
	else if (SEQUENCE == 5)
	{
		// 로비
		CSceneManager::Get_Instance()->Scene_Change(SCENE::LOBBY);
	}

	CInventoryManager::Get_Instance()->Set_Cookie();
	

	return READY_OK;
}

void CMainApp::Update_MainApp()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_F))
	{
		if (m_bIs_FPS_View == true)
		{
			m_bIs_FPS_View = false;
		}
		else if (m_bIs_FPS_View == false)
		{
			m_bIs_FPS_View = true;
		}
	}

	CKeyManager::Get_Instance()->Key_Update();
	CSceneManager::Get_Instance()->Update_Scene_Manager();
}

void CMainApp::Render_MainApp()
{
	static HDC MemDC, tempDC;
	static HBITMAP  BackBit, oldBackBit;

	MemDC = CreateCompatibleDC(m_hDC);
	BackBit = CreateCompatibleBitmap(m_hDC, CONSTANT::RES_X, CONSTANT::RES_Y);
	oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
	PatBlt(MemDC, 0, 0, CONSTANT::RES_X, CONSTANT::RES_Y, SRCCOPY);

	tempDC = m_hDC;
	m_hDC = MemDC;
	MemDC = tempDC;

	CSceneManager::Get_Instance()->Render_Scene_Manager(m_hDC);

	++m_iFPS;
	if (m_tFPS_Time + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		m_iFPS = 0;
		m_tFPS_Time = GetTickCount64();
	}
	HFONT	hFont, OldFont;

	hFont = CreateFont
	(
		50,
		0, 0, 0, 0, 0, 0, 0,
		HANGEUL_CHARSET,
		0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN,
		TEXT("CookieRun Black")
	);
	OldFont = (HFONT)SelectObject(m_hDC, hFont);

	SetTextColor(m_hDC, RGB(255, 255, 255));
	SetBkMode(m_hDC, TRANSPARENT);

	if (m_bIs_FPS_View == true)
	{
		TextOut(m_hDC, 95, 100, m_szFPS, lstrlen(m_szFPS));
	}
	
	SelectObject(m_hDC, OldFont);
	DeleteObject(hFont);

	tempDC = m_hDC;
	m_hDC = MemDC;
	MemDC = tempDC;

	BitBlt(m_hDC, 0, 0, CONSTANT::RES_X, CONSTANT::RES_Y, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, oldBackBit);
	DeleteObject(BackBit);
	DeleteDC(MemDC);
}

void CMainApp::Release_MainApp()
{
	ReleaseDC(g_hWnd, m_hDC);
	CKeyManager::Destroy_Instance();
	CGameObjectManager::Destroy_Instance();
	CSceneManager::Destroy_Instance();
	CCreateManager::Destroy_Instance();
}