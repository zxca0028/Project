#include "pch.h"
#include "CookieSelect.h"
#include "SceneManager.h"
#include "BitmapManager.h"
#include "CreateManager.h"
#include "InventoryManager.h"
#include "GameObjectManager.h"
#include "Player.h"

CCookieSelect::CCookieSelect()
	: m_bLobbyScene(false)
	, m_bExitButton(false)
	, m_bSortButton(false)
	, m_bNameSort(true)
	, m_bLevelSort(false)
{}

CCookieSelect::~CCookieSelect()
{
	Release_Scene();
}

int CCookieSelect::Ready_Scene()
{
	return READY_OK;
}

void CCookieSelect::Update_Scene()
{
	//Exit_Button();
	//Sort_Button();
	//Name_Sort_Button();
	//Level_Sort_Button();
	//Cookie_Select();

	Click();

	if (m_bLobbyScene == true)
	{
		if (m_tLobbyDelay + 100 < GetTickCount64())
		{
			CSceneManager::Get_Instance()->Scene_Change(SCENE::LOBBY);
		}
	}
}

void CCookieSelect::Render_Scene(HDC hDC)
{
	HDC hMemDC_BG = CBitmapManager::Get_Instance()->Find_Image(L"CookieSelect_BG");
	if (nullptr == hMemDC_BG) { return; }
	BLENDFUNCTION tBlend = {};
	tBlend.SourceConstantAlpha = 100;
	AlphaBlend(hDC, 0, 0, 1280, 720, hMemDC_BG, 0, 0, 1280, 720, tBlend);

	HDC hMemDC_Frame = CBitmapManager::Get_Instance()->Find_Image(L"CookieSelect_Frame");
	if (nullptr == hMemDC_Frame) { return; }
	GdiTransparentBlt(hDC, 0, 21, 1280, 144, hMemDC_Frame, 0, 0, 1280, 144, RGB(255, 0, 255));

	HDC hMemDC_Exit = CBitmapManager::Get_Instance()->Find_Image(L"CookieSelect_Exit");
	if (nullptr == hMemDC_Exit) { return; }
	if (m_bExitButton == false)
	{
		GdiTransparentBlt(hDC, 1188, 5, 88, 88, hMemDC_Exit, 0, 0, 88, 88, RGB(255, 0, 255));
	}
	else
	{
		GdiTransparentBlt(hDC, 1188, 5, 88, 88, hMemDC_Exit, 88, 0, 88, 88, RGB(255, 0, 255));
	}

	int iHaveCookie = 0;
	TCHAR szHaveCookie[32] = {};

	for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
	{
		if (pCookieList->IsAvailable == true)
		{
			++iHaveCookie;
		}
	}

	swprintf_s(szHaveCookie, L"%d", iHaveCookie);


	HFONT	hFont, OldFont;

	hFont = CreateFont
	(
		30,
		0, 0, 0, 0, 0, 0, 0,
		HANGEUL_CHARSET,
		0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN,
		TEXT("CookieRun Black")
	);
	OldFont = (HFONT)SelectObject(hDC, hFont);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	TextOut(hDC, CONSTANT::CT_X - 70, 123, L"모은  쿠키  :      /  5", 19);
	TextOut(hDC, CONSTANT::CT_X + 47, 123, szHaveCookie, lstrlen(szHaveCookie));

	SelectObject(hDC, OldFont);
	DeleteObject(hFont);

	if (m_bSortButton == true && m_bNameSort == true)
	{
		HDC hMemDC_Option = CBitmapManager::Get_Instance()->Find_Image(L"CookieSelect_Option");
		if (nullptr == hMemDC_Option) { return; }
		GdiTransparentBlt(hDC, 1065, 116, 164, 158, hMemDC_Option, 0, 0, 164, 158, RGB(255, 0, 255));
	}
	else if (m_bSortButton == true && m_bLevelSort == true)
	{
		HDC hMemDC_Option = CBitmapManager::Get_Instance()->Find_Image(L"CookieSelect_Option");
		if (nullptr == hMemDC_Option) { return; }
		GdiTransparentBlt(hDC, 1065, 116, 164, 158, hMemDC_Option, 164, 0, 164, 158, RGB(255, 0, 255));
	}

	if (m_bNameSort == true)
	{
		HDC hMemDC_Sort = CBitmapManager::Get_Instance()->Find_Image(L"CookieSelect_Sort");
		if (nullptr == hMemDC_Sort) { return; }
		GdiTransparentBlt(hDC, 1065, 116, 164, 47, hMemDC_Sort, 0, 0, 164, 47, RGB(255, 0, 255));
	}
	else if (m_bLevelSort == true)
	{
		HDC hMemDC_Sort = CBitmapManager::Get_Instance()->Find_Image(L"CookieSelect_Sort");
		if (nullptr == hMemDC_Sort) { return; }
		GdiTransparentBlt(hDC, 1065, 116, 164, 47, hMemDC_Sort, 0, 47, 164, 47, RGB(255, 0, 255));
	}

	CInventoryManager::Get_Instance()->Render_Profile(hDC);

}

void CCookieSelect::Release_Scene()
{}

void CCookieSelect::Exit_Button()
{
	POINT	pt_Play = {};
	RECT	tPlayButton = { 1188,5,1276,93 };

	GetCursorPos(&pt_Play);
	ScreenToClient(g_hWnd, &pt_Play);

	if (CKeyManager::Get_Instance()->Key_Pressing(KEY_LB))
	{
		if (PtInRect(&tPlayButton, pt_Play))
		{
			m_bExitButton = true;
			m_bLobbyScene = true;

			CSoundManager::Get_Instance()->PlaySound(L"Close.mp3", CSoundManager::CHANNELID::CLOSE);
			m_tLobbyDelay = GetTickCount64();
		}
		else
		{
			m_bLobbyScene = false;
		}
	}
	else
	{
		m_bExitButton = false;
	}

	if (m_bLobbyScene == true)
	{
		if (m_tLobbyDelay + 100 < GetTickCount64())
		{
			CSceneManager::Get_Instance()->Scene_Change(SCENE::LOBBY);
		}
	}
}

void CCookieSelect::Sort_Button()
{
	POINT	pt_Sort = {};
	RECT	tSortButton = { 1065,116,1229,163 };

	GetCursorPos(&pt_Sort);
	ScreenToClient(g_hWnd, &pt_Sort);

	if (CKeyManager::Get_Instance()->Key_Down(KEY_LB))
	{
		if (PtInRect(&tSortButton, pt_Sort))
		{
			if (m_bSortButton == true)
			{
				m_bSortButton = false;
			}
			else if (m_bSortButton == false)
			{
				m_bSortButton = true;
				CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			}
		}
		else
		{
			m_bSortButton = false;
		}
	}
}

void CCookieSelect::Name_Sort_Button()
{
	POINT	pt_NameSort = {};
	RECT	tNameSort = { 1092, 173, 1202, 207 };

	GetCursorPos(&pt_NameSort);
	ScreenToClient(g_hWnd, &pt_NameSort);

	if (CKeyManager::Get_Instance()->Key_Pressing(KEY_LB))
	{
		if (PtInRect(&tNameSort, pt_NameSort))
		{
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			CInventoryManager::Get_Instance()->Name_Ascend_Sort();
			m_bNameSort = true;
			m_bLevelSort = false;
		}
	}
}

void CCookieSelect::Level_Sort_Button()
{
	POINT	pt_LevelSort = {};
	RECT	tLevelSort = { 1092, 227, 1202, 261 };

	GetCursorPos(&pt_LevelSort);
	ScreenToClient(g_hWnd, &pt_LevelSort);

	if (CKeyManager::Get_Instance()->Key_Pressing(KEY_LB))
	{
		if (PtInRect(&tLevelSort, pt_LevelSort))
		{
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			CInventoryManager::Get_Instance()->Rarity_Ascend_Sort();
			m_bLevelSort = true;
			m_bNameSort = false;
		}
	}
}

void CCookieSelect::Cookie_Select()
{
	POINT pt_CookieSelect = {};
	GetCursorPos(&pt_CookieSelect);
	ScreenToClient(g_hWnd, &pt_CookieSelect);

	if (CKeyManager::Get_Instance()->Key_Pressing(KEY_LB))
	{
		CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);

		if (PtInRect(&SLOT.tSlot_01, pt_CookieSelect))
		{
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 1)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 1)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}
		else if (PtInRect(&SLOT.tSlot_02, pt_CookieSelect))
		{
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 2)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 2)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}
		else if (PtInRect(&SLOT.tSlot_03, pt_CookieSelect))
		{
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 3)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 3)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}
		else if (PtInRect(&SLOT.tSlot_04, pt_CookieSelect))
		{
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 4)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 4)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}
		else if (PtInRect(&SLOT.tSlot_05, pt_CookieSelect))
		{
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 5)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 5)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}
	}
}

void CCookieSelect::Click()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_LB))
	{
		// 쿠키 선택
		POINT pt_CookieSelect = {};
		GetCursorPos(&pt_CookieSelect);
		ScreenToClient(g_hWnd, &pt_CookieSelect);

		if (PtInRect(&SLOT.tSlot_01, pt_CookieSelect))
		{
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 1)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 1)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}
		else if (PtInRect(&SLOT.tSlot_02, pt_CookieSelect))
		{
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 2)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 2)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}
		else if (PtInRect(&SLOT.tSlot_03, pt_CookieSelect))
		{
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 3)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 3)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}
		else if (PtInRect(&SLOT.tSlot_04, pt_CookieSelect))
		{
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 4)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 4)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}
		else if (PtInRect(&SLOT.tSlot_05, pt_CookieSelect))
		{
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
			{
				if (pCookieList->iIndex == 5)
				{
					if (pCookieList->IsAvailable == true)
					{
						CCreateManager::Get_Instance()->Set_Cookie(pCookieList->szSpriteKeyValue);
						pCookieList->IsSelected = true;

						for (auto pCookieList : *(CInventoryManager::Get_Instance()->Get_CookieList()))
						{
							if (pCookieList->iIndex != 5)
							{
								pCookieList->IsSelected = false;
							}
						}
					}
				}
			}
		}

		// 쿠키 레벨순 정렬
		POINT	pt_LevelSort = {};
		RECT	tLevelSort = { 1092, 227, 1202, 261 };

		GetCursorPos(&pt_LevelSort);
		ScreenToClient(g_hWnd, &pt_LevelSort);

		if (PtInRect(&tLevelSort, pt_LevelSort))
		{
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			CInventoryManager::Get_Instance()->Rarity_Ascend_Sort();
			m_bLevelSort = true;
			m_bNameSort = false;
		}

		// 쿠키 이름순 정렬
		POINT	pt_NameSort = {};
		RECT	tNameSort = { 1092, 173, 1202, 207 };

		GetCursorPos(&pt_NameSort);
		ScreenToClient(g_hWnd, &pt_NameSort);

		if (PtInRect(&tNameSort, pt_NameSort))
		{
			CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			CInventoryManager::Get_Instance()->Name_Ascend_Sort();
			m_bNameSort = true;
			m_bLevelSort = false;
		}

		// 정렬 버튼
		POINT	pt_Sort = {};
		RECT	tSortButton = { 1065,116,1229,163 };

		GetCursorPos(&pt_Sort);
		ScreenToClient(g_hWnd, &pt_Sort);

		if (PtInRect(&tSortButton, pt_Sort))
		{
			if (m_bSortButton == true)
			{
				m_bSortButton = false;
			}
			else if (m_bSortButton == false)
			{
				m_bSortButton = true;
				CSoundManager::Get_Instance()->PlaySound(L"Open.mp3", CSoundManager::CHANNELID::OPEN);
			}
		}
		else
		{
			m_bSortButton = false;
		}

		// 나가기 버튼
		POINT	pt_Play = {};
		RECT	tPlayButton = { 1188,5,1276,93 };

		GetCursorPos(&pt_Play);
		ScreenToClient(g_hWnd, &pt_Play);

		if (CKeyManager::Get_Instance()->Key_Pressing(KEY_LB))
		{
			if (PtInRect(&tPlayButton, pt_Play))
			{
				m_bExitButton = true;
				m_bLobbyScene = true;

				CSoundManager::Get_Instance()->PlaySound(L"Close.mp3", CSoundManager::CHANNELID::CLOSE);
				m_tLobbyDelay = GetTickCount64();
			}
			else
			{
				m_bLobbyScene = false;
			}
		}
		else
		{
			m_bExitButton = false;
		}
	}
}


CScene* CCookieSelect::Create()
{
	CScene* pInstance = new CCookieSelect;

	if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

	return pInstance;
}