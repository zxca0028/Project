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

	// 쿠키가방에서 표시될 번호
	int  iIndex;

	// 쿠키가방에서 표시될 위치
	RECT tRect;

	// 정렬을 위한 키값
	const TCHAR* szCookieName;

	// 프로필 이미지 키값
	const TCHAR* szKeyValue;

	// 쿠키 스프라이트 이미지 키값
	const TCHAR* szSpriteKeyValue;

};

