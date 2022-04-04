#include "pch.h"
#include "StageLoading.h"
#include "SceneManager.h"
#include "BitmapManager.h"
#include "CreateManager.h"
#include "Score.h"

CStageLoading::CStageLoading()
	: m_tTime(0)
	, m_bTemp(false)
	, m_bSentenceLock(false)
{}

CStageLoading::~CStageLoading()
{
	Release_Scene();
}

int CStageLoading::Ready_Scene()
{
	m_tTime = GetTickCount64();
	m_bSentenceLock = false;

	return READY_OK;
}

void CStageLoading::Update_Scene()
{
	if (m_tTime + 2000 < GetTickCount64())
	{
		CSceneManager::Get_Instance()->Scene_Change(SCENE::STAGE);
	}
}

void CStageLoading::Render_Scene(HDC hDC)
{
	HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"StageLoading12");
	if (nullptr == hMemDC_BG) { return; }
	BitBlt(hDC, 0, 0, 1280, 720, hMemDC_BG, 0, 0, SRCCOPY);

	HDC hMemDC_Cloud = CBitmapManager::Get_Instance()->Find_Image(L"Cloud");
	if (nullptr == hMemDC_Cloud) { return; }
	GdiTransparentBlt(hDC, CONSTANT::CT_X - 200, CONSTANT::CT_Y - 250, 900, 300, hMemDC_Cloud, 0, 0, 900, 300, RGB(0, 0, 0));

	HDC hMemDC_Stand = CBitmapManager::Get_Instance()->Find_Image(CCreateManager::Get_Instance()->Get_Cookie());
	if (nullptr == hMemDC_Stand) { return; }
	GdiTransparentBlt(hDC, CONSTANT::CT_X - 124, CONSTANT::CT_Y, 248, 248, hMemDC_Stand, 0, 3960, 248, 248, RGB(255, 0, 255));

	HFONT	hFont, OldFont;

	hFont = CreateFont(45, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("CookieRun Black"));
	OldFont = (HFONT)SelectObject(hDC, hFont);

	SetBkMode(hDC, TRANSPARENT);

	if (m_bSentenceLock == false)
	{
		m_pSentence = CCreateManager::Get_Instance()->SentenceList();
		m_bSentenceLock = true;
	}

	TextOut(hDC, CONSTANT::CT_X + 30, CONSTANT::CT_Y - 140, m_pSentence, lstrlen(m_pSentence));

	SelectObject(hDC, OldFont);
	DeleteObject(hFont);	
}

void CStageLoading::Release_Scene()
{
	CScore::m_iCurrentScore = 0;

	srand(unsigned(time(NULL)));

	/*int iPhase = rand() % 2;*/
	int iPhase = 1;

	if (iPhase == 0)
	{
		CCreateManager::Get_Instance()->Set_Phase(PHASE::TYPE::_1ST);
	}
	else if (iPhase == 1)
	{
		CCreateManager::Get_Instance()->Set_Phase(PHASE::TYPE::_2ND);
	}

	CSoundManager::Get_Instance()->StopSound(CSoundManager::CHANNELID::BGM);
	
}

CScene* CStageLoading::Create()
{
	CScene* pInstance = new CStageLoading;

	if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

	return pInstance;
}
