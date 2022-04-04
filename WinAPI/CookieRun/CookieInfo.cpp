#include "pch.h"
#include "CookieInfo.h"

CCookieInfo::CCookieInfo()
{
}

CCookieInfo::~CCookieInfo()
{
}

void CCookieInfo::Sentence()
{
}

void CCookieInfo::Set_CookieInfo()
{
}

CCookieInfo* CCookieInfo::Create(const TCHAR* pKeyValue, const TCHAR* pCookieSprite, const TCHAR* pCookieName)
{
	CCookieInfo* pInstance = new CCookieInfo;
	if (nullptr == pInstance) { Safe_Delete(pInstance); }

	pInstance->szKeyValue = pKeyValue;
	pInstance->szSpriteKeyValue = pCookieSprite;
	pInstance->szCookieName = pCookieName;

	return pInstance;
}
