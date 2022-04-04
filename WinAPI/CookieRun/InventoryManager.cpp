#include "pch.h"
#include "InventoryManager.h"
#include "BitmapManager.h"
#include "Lobby.h"

CInventoryManager* CInventoryManager::m_pInstance = nullptr;

CInventoryManager::CInventoryManager()
{
}

CInventoryManager::~CInventoryManager()
{
}

CInventoryManager* CInventoryManager::Get_Instance()
{
	if (nullptr == m_pInstance) { m_pInstance = new CInventoryManager; }
	return m_pInstance;
}

void CInventoryManager::Destroy_Instance()
{
	if (nullptr != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void CInventoryManager::Register_Cookie(COOKIEINFO* pObject, const TCHAR* pKeyValue, const TCHAR* pCookieSprite, const TCHAR* pCookieName, int Grade)
{
	pObject->szKeyValue = pKeyValue;
	pObject->szSpriteKeyValue = pCookieSprite;
	pObject->szCookieName = pCookieName;
	pObject->iGrade = Grade;

	m_listCookie.emplace_back(pObject);

	int i = 1;

	for (auto pIndex : m_listCookie)
	{
		pIndex->iIndex = i;
		++i;
	}
}

void CInventoryManager::Set_Cookie()
{
	// µÎ¹øÂ° : ÇÁ·ÎÇÊ »çÁø, ¼¼¹øÂ° : ÄíÅ° ½ºÇÁ¶óÀÌÆ®

	COOKIEINFO* pBrave = new COOKIEINFO;
	CInventoryManager::Get_Instance()->Register_Cookie(pBrave, L"Brave_Profile", L"BraveCookie", L"¿ë°¨ÇÑ_ÄíÅ°",3);

	COOKIEINFO* pMoonlight = new COOKIEINFO;
	CInventoryManager::Get_Instance()->Register_Cookie(pMoonlight, L"Moonlight_Profile", L"MoonlightCookie", L"´Þºû¼ú»ç_ÄíÅ°",1);

	COOKIEINFO* pCherry = new COOKIEINFO;
	CInventoryManager::Get_Instance()->Register_Cookie(pCherry, L"CherryBlossom_Profile", L"CherryCookie", L"º¢²É¸À_ÄíÅ°",2);

	COOKIEINFO* pBlackBerry = new COOKIEINFO;
	CInventoryManager::Get_Instance()->Register_Cookie(pBlackBerry, L"BlackBerry_Profile", L"BlackBerryCookie", L"ºí·¢º£¸®¸À_ÄíÅ°",2);

	COOKIEINFO* pCream = new COOKIEINFO;
	CInventoryManager::Get_Instance()->Register_Cookie(pCream, L"ChessChoco_Profile", L"ChessChocoCookie", L"Ã¼½ºÃÊÄÚ_ÄíÅ°",2);

	m_listCookie.front()->IsSelected = true;
	m_listCookie.front()->IsAvailable = true;

	//for (auto Temp : m_listCookie)
	//{
	//	Temp->IsAvailable = true;
	//}

}

void CInventoryManager::Render_Profile(HDC hDC)
{
	int i = 0;

	for (auto pCookie : m_listCookie)
	{
		if (pCookie->IsAvailable == true)
		{
			HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(pCookie->szKeyValue);
			if (nullptr == hMemDC) { return; }
			GdiTransparentBlt(hDC, 40 + 250 * i, 342, 200, 200, hMemDC, 0, 0, 200, 200, RGB(255, 0, 255));
		}
		else
		{
			HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(pCookie->szKeyValue);
			if (nullptr == hMemDC) { return; }
			GdiTransparentBlt(hDC, 40 + 250 * i, 342, 200, 200, hMemDC, 200, 0, 200, 200, RGB(255, 0, 255));
		}

		++i;
	}

	for (auto pCookieSelected : m_listCookie)
	{
		if (pCookieSelected->IsAvailable == true && pCookieSelected->IsSelected == true)
		{
			HDC hMemDC_Border = CBitmapManager::Get_Instance()->Find_Image(L"BorderLine");
			if (nullptr == hMemDC_Border) { return; }
			GdiTransparentBlt(hDC, 40 + 250 * (pCookieSelected->iIndex - 1) - 19, 342 - 19, 238, 238, hMemDC_Border, 0, 0, 238, 238, RGB(255, 0, 255));

			HDC hMemDC_Pont = CBitmapManager::Get_Instance()->Find_Image(L"Selected");
			if (nullptr == hMemDC_Pont) { return; }
			GdiTransparentBlt(hDC, 40 + 250 * (pCookieSelected->iIndex - 1) + 75, 342 + 7, 54, 22, hMemDC_Pont, 0, 0, 54, 22, RGB(255, 0, 255));
		}
	}

	Cookie_Unlock();

	for (auto pCookie : m_listCookie)
	{
		if (pCookie->IsAvailable == false)
		{
			HDC hMemDC_SilverCoinBG = CBitmapManager::Get_Instance()->Find_Image(L"Coin_BG");
			if (nullptr == hMemDC_SilverCoinBG) { return; }
			GdiTransparentBlt(hDC, 40 + 250 * (pCookie->iIndex - 1) + 15, 590, 174, 31, hMemDC_SilverCoinBG, 0, 0, 174, 37, RGB(255, 0, 255));

			HDC hMemDC_SilverCoin = CBitmapManager::Get_Instance()->Find_Image(L"Silver_Coin");
			if (nullptr == hMemDC_SilverCoin) { return; }
			GdiTransparentBlt(hDC, 40 + 250 * (pCookie->iIndex - 1) + 6, 582, 50, 50, hMemDC_SilverCoin, 0, 0, 50, 50, RGB(255, 0, 255));

			TCHAR szGetCoin[32] = {};

			HFONT	hCoinFont, OldCoinFont;
			hCoinFont = CreateFont(38, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("CookieRun Black"));
			OldCoinFont = (HFONT)SelectObject(hDC, hCoinFont);

			SetTextColor(hDC, RGB(255, 255, 255));
			SetBkMode(hDC, TRANSPARENT);

			if (Cookie_Price(pCookie->szCookieName) < 10)
			{
				swprintf_s(szGetCoin, L"%d", Cookie_Price(pCookie->szCookieName));
				TextOut(hDC, 40 + 250 * (pCookie->iIndex - 1) + 15+130, 586, szGetCoin, lstrlen(szGetCoin));
			}
			else if (Cookie_Price(pCookie->szCookieName) < 100)
			{
				swprintf_s(szGetCoin, L"%d", Cookie_Price(pCookie->szCookieName));
				TextOut(hDC, 40 + 250 * (pCookie->iIndex - 1) + 130, 586, szGetCoin, lstrlen(szGetCoin));
			}
			else if (Cookie_Price(pCookie->szCookieName) < 1000)
			{
				swprintf_s(szGetCoin, L"%d", Cookie_Price(pCookie->szCookieName));
				TextOut(hDC, 40 + 250 * (pCookie->iIndex - 1) - 15 + 130, 586, szGetCoin, lstrlen(szGetCoin));
			}
			else if (Cookie_Price(pCookie->szCookieName) < 10000)
			{
				int iTemp_Front = Cookie_Price(pCookie->szCookieName) / 1000;
				int iTemp_Back = Cookie_Price(pCookie->szCookieName) % 1000;

				swprintf_s(szGetCoin, L"%d,%03d", iTemp_Front, iTemp_Back);
				TextOut(hDC, 40 + 250 * (pCookie->iIndex - 1) - 30 + 130, 586, szGetCoin, lstrlen(szGetCoin));
			}
			else if (Cookie_Price(pCookie->szCookieName) < 100000)
			{
				int iTemp_Front = Cookie_Price(pCookie->szCookieName) / 1000;
				int iTemp_Back = Cookie_Price(pCookie->szCookieName) % 1000;

				swprintf_s(szGetCoin, L"%d,%03d", iTemp_Front, iTemp_Back);
				TextOut(hDC, 40 + 250 * (pCookie->iIndex - 1) - 45 + 130, 586, szGetCoin, lstrlen(szGetCoin));
			}

			SelectObject(hDC, OldCoinFont);
			DeleteObject(hCoinFont);
		}
	}
}

void CInventoryManager::Name_Ascend_Sort()
{
	m_listCookie.sort([](COOKIEINFO* pCmp1, COOKIEINFO* pCmp2) { return lstrcmp(pCmp1->szCookieName, pCmp2->szCookieName) < 0; });
	int i = 1;
	for (auto pIndex : m_listCookie)
	{
		pIndex->iIndex = i;
		++i;
	}
}

void CInventoryManager::Name_Descend_Sort()
{
	m_listCookie.sort([](COOKIEINFO* pCmp1, COOKIEINFO* pCmp2) { return lstrcmp(pCmp1->szCookieName, pCmp2->szCookieName) > 0; });
	int i = 1;
	for (auto pIndex : m_listCookie)
	{
		pIndex->iIndex = i;
		++i;
	}
}

void CInventoryManager::Rarity_Ascend_Sort()
{
	m_listCookie.sort([](COOKIEINFO* pCmp1, COOKIEINFO* pCmp2) {return pCmp1->iGrade < pCmp2->iGrade; });
	int i = 1;
	for (auto pIndex : m_listCookie)
	{
		pIndex->iIndex = i;
		++i;
	}
}

int CInventoryManager::Cookie_Price(const TCHAR* pCookieName)
{
	if (pCookieName == L"º¢²É¸À_ÄíÅ°")
	{
		return 1000;
	}
	else if (pCookieName == L"ºí·¢º£¸®¸À_ÄíÅ°")
	{
		return 5000;
	}
	else if (pCookieName == L"Ã¼½ºÃÊÄÚ_ÄíÅ°")
	{
		return 12000;
	}
	else if (pCookieName == L"´Þºû¼ú»ç_ÄíÅ°")
	{
		return 27000;
	}
}

void CInventoryManager::Cookie_Unlock()
{
	for (auto pCookie : m_listCookie)
	{
		if (pCookie->IsAvailable == false)
		{
			if (pCookie->szCookieName == L"º¢²É¸À_ÄíÅ°" && CLobby::m_iHaveCoin >= 400)
			{
				pCookie->IsAvailable = true;
				CLobby::m_iHaveCoin -= 1000;
			}
			else if (pCookie->szCookieName == L"ºí·¢º£¸®¸À_ÄíÅ°" && CLobby::m_iHaveCoin >= 3300)
			{
				pCookie->IsAvailable = true;
				CLobby::m_iHaveCoin -= 5000;
			}
			else if (pCookie->szCookieName == L"Ã¼½ºÃÊÄÚ_ÄíÅ°" && CLobby::m_iHaveCoin >= 12000)
			{
				pCookie->IsAvailable = true;
				CLobby::m_iHaveCoin -= 12000;
			}
			else if (pCookie->szCookieName == L"´Þºû¼ú»ç_ÄíÅ°" && CLobby::m_iHaveCoin >= 27000)
			{
				pCookie->IsAvailable = true;
				CLobby::m_iHaveCoin -= 27000;
			}
		}
	}
}
