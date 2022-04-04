#pragma once

class CInventoryManager
{
public:

	CInventoryManager();

private:

	~CInventoryManager();

public:

	static CInventoryManager* Get_Instance();
	static void Destroy_Instance();
	void Register_Cookie(COOKIEINFO* pObject, const TCHAR* pKeyValue, const TCHAR* pCookieSprite, const TCHAR* pCookieName, int Grade);

	void Set_Cookie();

	list<COOKIEINFO*>* Get_CookieList() { return &m_listCookie; }
	void Render_Profile(HDC hDC);

	void Name_Ascend_Sort();
	void Name_Descend_Sort();
	void Rarity_Ascend_Sort();

	int Cookie_Price(const TCHAR* pCookieName);
	void Cookie_Unlock();


private:

	static CInventoryManager* m_pInstance;

	list<COOKIEINFO*> m_listCookie;

};