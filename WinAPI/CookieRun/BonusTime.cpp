#include "pch.h"
#include "BonusTime.h"

CBonusTime::CBonusTime()
{
}

CBonusTime::~CBonusTime()
{
}

int CBonusTime::Ready_Scene()
{
    return READY_OK;
}

void CBonusTime::Update_Scene()
{
}

void CBonusTime::Render_Scene(HDC hDC)
{
}

void CBonusTime::Release_Scene()
{
}

CScene* CBonusTime::Create()
{
	CScene* pInstance = new CBonusTime;

	if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

	return pInstance;
}

