#include "pch.h"
#include "BonusStage.h"
#include "SceneManager.h"

#include "BitmapManager.h"
#include "GameObjectManager.h"
#include "Player.h"
#include "Ground.h"
#include "BackGround.h"
#include "Obstacles.h"

#include "Item.h"
#include "UI.h"

CBonusStage::CBonusStage()
{}

CBonusStage::~CBonusStage()
{
	Release_Scene();
}

int CBonusStage::Ready_Scene()
{
	// Player 积己
	CGameObject* pPlayerObj = CPlayer::Create();
	if (nullptr == pPlayerObj) { return READY_FAILED; }
	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::ID::PLAYER, pPlayerObj);

	//// BackGround 积己
	//for (int i = 0; i < 2; ++i)
	//{
	//	CGameObject* pBackGroundObj = CBackGround::Create(BG::TYPE::BONUS_BG, 640 + (float)(1280 * i), 360);
	//	if (nullptr == pBackGroundObj) { return READY_FAILED; }
	//	CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::ID::BONUS_BACKGROUND, pBackGroundObj);
	//}

    return READY_OK;
}

void CBonusStage::Update_Scene()
{
	// BackGround 犁积 == GameObjectManager俊辑

	CGameObjectManager::Get_Instance()->Update_GameObjectManager();
}

void CBonusStage::Render_Scene(HDC hDC)
{
	CGameObjectManager::Get_Instance()->Render_GameObjectManager(hDC);
	CUI::Render_UI(hDC, UI::TYPE::BONUSTIME);
	CUI::Render_UI(hDC, UI::TYPE::GAUGE);
	CUI::Render_UI(hDC, UI::TYPE::GAUGEICON);
	CUI::Render_UI(hDC, UI::TYPE::PAUSE);
}

void CBonusStage::Release_Scene()
{
}

CScene* CBonusStage::Create()
{
	CScene* pInstance = new CBonusStage;

	if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

	return pInstance;
}
