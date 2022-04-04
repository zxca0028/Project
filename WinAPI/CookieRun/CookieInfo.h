#pragma once

class CCookieInfo
{
public:

	 CCookieInfo();
	~CCookieInfo();

private:

	void Sentence();
	void Set_CookieInfo();

public:

	static CCookieInfo* Create(const TCHAR* pKeyValue, const TCHAR* pCookieSprite, const TCHAR* pCookieName);

private:

	// ��Ű���濡�� ǥ�õ� ��ȣ
	int  iIndex;

	// ��Ű���濡�� ǥ�õ� ��ġ
	RECT tRect;

	// ������ ���� Ű��
	const TCHAR* szCookieName;

	// ������ �̹��� Ű��
	const TCHAR* szKeyValue;

	// ��Ű ��������Ʈ �̹��� Ű��
	const TCHAR* szSpriteKeyValue;

};

