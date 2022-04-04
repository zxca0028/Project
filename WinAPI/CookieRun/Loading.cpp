#include "pch.h"
#include "Loading.h"
#include "SceneManager.h"
#include "BitmapManager.h"

CLoading::CLoading()
{}

CLoading::~CLoading()
{
	Release_Scene();
}

int CLoading::Ready_Scene()
{
	CSoundManager::Get_Instance()->PlayBGM(L"Lobby.mp3");
	return READY_OK;
}

void CLoading::Update_Scene()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_ENTER))
	{
		CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
		CSceneManager::Get_Instance()->Scene_Change(SCENE::LOBBY);
	}
}

void CLoading::Render_Scene(HDC hDC)
{
	HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(L"Loading");
	if (nullptr == hMemDC) { return; }

	BitBlt(hDC, 0, 0, 1280, 720, hMemDC, 0, 0, SRCCOPY);
}

void CLoading::Release_Scene()
{}

CScene* CLoading::Create()
{
	CScene* pInstance = new CLoading;

	if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

	return pInstance;
}
