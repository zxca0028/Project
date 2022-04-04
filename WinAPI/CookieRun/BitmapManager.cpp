#include "pch.h"
#include "BitmapManager.h"
#include "MyBitmap.h"

CBitmapManager* CBitmapManager::m_pInstance = nullptr;

CBitmapManager::CBitmapManager()
{}

CBitmapManager::~CBitmapManager()
{
	Release_BitMap();
}

CBitmapManager* CBitmapManager::Get_Instance()
{
	if (nullptr == m_pInstance) { m_pInstance = new CBitmapManager; }

	return m_pInstance;
}

void CBitmapManager::Destroy_Instance()
{
	if (nullptr != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

HDC CBitmapManager::Find_Image(const TCHAR* pImageKey)
{
	auto iter_find = m_mapBmp.find(pImageKey);

	if (iter_find == m_mapBmp.end()) { return nullptr; }

	return iter_find->second->Get_MemDC();
}

void CBitmapManager::Insert_Bitmap(const TCHAR* pPath, const TCHAR* pImageKey)
{
	auto iter_find = m_mapBmp.find(pImageKey);

	if (iter_find != m_mapBmp.end()) { return; }

	CMyBitmap* pBitMap = new CMyBitmap;
	pBitMap->Insert_Bitmap(pPath);

	m_mapBmp.emplace(pImageKey, pBitMap);
}

void CBitmapManager::Load_Bitmap()
{
	Insert_Bitmap(L"../Resource/test.bmp", L"test");

	// 로딩 ////////////////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/Loading/Loading.bmp", L"Loading");
	Insert_Bitmap(L"../Resource/StageLoading/StageLoading12.bmp", L"StageLoading12");
	Insert_Bitmap(L"../Resource/StageLoading/Cloud1.bmp", L"Cloud");

	// 로비 ////////////////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/Lobby/Lobby123.bmp", L"Lobby");
	Insert_Bitmap(L"../Resource/Lobby/PlayButton.bmp", L"PlayButton");
	Insert_Bitmap(L"../Resource/Lobby/MyCookiesButton.bmp", L"MyCookiesButton");
	Insert_Bitmap(L"../Resource/Lobby/Coin_BG.bmp", L"Coin_BG");
	Insert_Bitmap(L"../Resource/Lobby/Lobby_Stand_BlackBerry.bmp", L"Lobby_Stand_BlackBerry");
	Insert_Bitmap(L"../Resource/Lobby/Lobby_Stand_Cream.bmp", L"Lobby_Stand_Cream");

	// 쿠키선택 페이지 /////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/CookieSelect/BlackBerry_Profile.bmp", L"BlackBerry_Profile");
	Insert_Bitmap(L"../Resource/CookieSelect/ChessChoco_Profile.bmp",  L"ChessChoco_Profile");
	Insert_Bitmap(L"../Resource/CookieSelect/CherryBlossom_Profile.bmp", L"CherryBlossom_Profile");
	Insert_Bitmap(L"../Resource/CookieSelect/Brave_Profile.bmp", L"Brave_Profile");
	Insert_Bitmap(L"../Resource/CookieSelect/Moonlight_Profile.bmp", L"Moonlight_Profile");

	
	Insert_Bitmap(L"../Resource/CookieSelect/CookieSelect_BG.bmp", L"CookieSelect_BG");
	Insert_Bitmap(L"../Resource/CookieSelect/CookieSelect_Frame.bmp", L"CookieSelect_Frame");
	Insert_Bitmap(L"../Resource/CookieSelect/CookieSelect_Sort.bmp", L"CookieSelect_Sort");
	Insert_Bitmap(L"../Resource/CookieSelect/CookieSelect_Exit.bmp", L"CookieSelect_Exit");
	Insert_Bitmap(L"../Resource/CookieSelect/CookieSelect_Option.bmp", L"CookieSelect_Option");
	Insert_Bitmap(L"../Resource/CookieSelect/BorderLine.bmp", L"BorderLine");
	Insert_Bitmap(L"../Resource/CookieSelect/Selected.bmp", L"Selected");


	// 스테이지 ////////////////////////////////////////////////////////////////////////////////////
	// PHASE 1
	Insert_Bitmap(L"../Resource/Stage/PHASE_1/BG_1.bmp", L"BG_1");
	Insert_Bitmap(L"../Resource/Stage/PHASE_1/BG_2.bmp", L"BG_2");
	Insert_Bitmap(L"../Resource/Stage/PHASE_1/Ground2.bmp", L"Ground");

	// PHASE 2
	Insert_Bitmap(L"../Resource/Stage/PHASE_2/BG_1.bmp", L"BG2_1");
	Insert_Bitmap(L"../Resource/Stage/PHASE_2/BG_2.bmp", L"BG2_2");
	Insert_Bitmap(L"../Resource/Stage/PHASE_2/Ground2.bmp", L"Ground2");


	// 보너스타임 ////////////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/BonusTime/BonusTime_In.bmp", L"BonusTime_In");
	Insert_Bitmap(L"../Resource/BonusTime/BonusTime_Out.bmp", L"BonusTime_Out");
	Insert_Bitmap(L"../Resource/BonusTime/BonusTime_BG.bmp", L"BonusTime_BG");
	Insert_Bitmap(L"../Resource/BonusTime/BonusTime_Ground.bmp", L"BonusTime_Ground");
	Insert_Bitmap(L"../Resource/BonusTime/Effect.bmp", L"Effect");

	// 캐릭터, 펫 ///////////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/Cookies/BlackBerryCookie.bmp", L"BlackBerryCookie");
	Insert_Bitmap(L"../Resource/Cookies/ChessChocoCookie.bmp", L"ChessChocoCookie");
	Insert_Bitmap(L"../Resource/Cookies/CherryCookie.bmp", L"CherryCookie");
	Insert_Bitmap(L"../Resource/Cookies/BraveCookie.bmp", L"BraveCookie");
	Insert_Bitmap(L"../Resource/Cookies/MoonlightCookie.bmp", L"MoonlightCookie");

	Insert_Bitmap(L"../Resource/Pet/Pets.bmp", L"Pets");

	// 아이템  /////////////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/Item/Items.bmp", L"Items");
	Insert_Bitmap(L"../Resource/Item/Big_HP.bmp", L"Big_HP");
	Insert_Bitmap(L"../Resource/Item/Text.bmp", L"Text");
	Insert_Bitmap(L"../Resource/Item/Heart.bmp", L"Heart");
	Insert_Bitmap(L"../Resource/Item/BonusTime_Item.bmp", L"BonusTime_Item");

	// 젤리 //////////////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/Jelly/Jellies.bmp", L"Jellies");

	// 장애물 //////////////////////////////////////////////////////////////////////////////////////
	// PHASE_1
	Insert_Bitmap(L"../Resource/Obstacle/Obstacles_1.bmp", L"Obstacles_1");
	Insert_Bitmap(L"../Resource/Obstacle/Obstacles_2.bmp", L"Obstacles_2");

	// UI //////////////////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/UI/BONUSTIME.bmp", L"BONUSTIME");
	Insert_Bitmap(L"../Resource/UI/BONUSTIME_Color.bmp", L"BONUSTIME_Color");
	Insert_Bitmap(L"../Resource/UI/Pause.bmp", L"Pause");
	Insert_Bitmap(L"../Resource/UI/GaugeIcon.bmp", L"GaugeIcon");
	Insert_Bitmap(L"../Resource/UI/Gauge.bmp", L"Gauge");
	Insert_Bitmap(L"../Resource/UI/Gauge_Under.bmp", L"Gauge_Under");
	Insert_Bitmap(L"../Resource/UI/Score_BG.bmp", L"Score_BG");
	Insert_Bitmap(L"../Resource/UI/BEST.bmp", L"BEST");
	Insert_Bitmap(L"../Resource/UI/NewRecord.bmp", L"NewRecord");
	Insert_Bitmap(L"../Resource/UI/ScoreJelly.bmp", L"ScoreJelly");
	Insert_Bitmap(L"../Resource/UI/Silver_Coin.bmp", L"Silver_Coin");
	Insert_Bitmap(L"../Resource/UI/Progress.bmp", L"Progress");
	Insert_Bitmap(L"../Resource/UI/Progress_Icon.bmp", L"Progress_Icon");

	// 결과창 ///////////////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/Result/Result_BG.bmp", L"Result_BG");
	Insert_Bitmap(L"../Resource/Result/Result_KR.bmp", L"Result_KR");
	Insert_Bitmap(L"../Resource/Result/Result_HiScore.bmp", L"Result_HiScore");
	Insert_Bitmap(L"../Resource/Result/Result_Button.bmp", L"Result_Button");
	Insert_Bitmap(L"../Resource/Result/Result_Coin.bmp", L"Result_Coin");
	Insert_Bitmap(L"../Resource/Result/Result_CoinImage_test.bmp", L"Result_CoinImage_test");


	// 이펙트 //////////////////////////////////////////////////////////////////////////////////////
	Insert_Bitmap(L"../Resource/Effect/BlastEffect.bmp", L"BlastEffect");
	Insert_Bitmap(L"../Resource/Effect/JellyEffect.bmp", L"JellyEffect");

	// 깃발
	Insert_Bitmap(L"../Resource/Stage/Flag.bmp", L"Flag");

}

void CBitmapManager::Release_BitMap()
{
	for (auto& rPair : m_mapBmp) { Safe_Delete(rPair.second); }

	m_mapBmp.clear();
}