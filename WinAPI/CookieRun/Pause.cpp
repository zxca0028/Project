#include "pch.h"
#include "Pause.h"

#include "GameObjectManager.h"
#include "SceneManager.h"

CPause::CPause()
{}

CPause::~CPause()
{
    Release_Scene();
}

int CPause::Ready_Scene()
{
    return READY_OK;
}

void CPause::Update_Scene()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_ESC))
	{
		CSceneManager::Get_Instance()->Scene_Change(SCENE::TYPE::STAGE);
	}
}

void CPause::Render_Scene(HDC hDC)
{
	TCHAR szText[32] = {};

	swprintf_s(szText, L"일시정지 화면");

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

	TextOut(hDC, CONSTANT::CT_X - 50, 120, szText, lstrlen(szText));

	SelectObject(hDC, OldFont);
	DeleteObject(hFont);
}

void CPause::Release_Scene()
{}

CScene* CPause::Create()
{
    CScene* pInstance = new CPause;

    if (0 > pInstance->Ready_Scene()) { Safe_Delete(pInstance); }

    return pInstance;
}
