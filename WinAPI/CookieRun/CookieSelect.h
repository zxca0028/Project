#pragma once

#include "Scene.h"

class CCookieSelect final : public CScene
{
private:

	explicit CCookieSelect();

public:

	virtual ~CCookieSelect();

public:

	// CScene을(를) 통해 상속됨
	virtual  int Ready_Scene()			override;
	virtual void Update_Scene()			override;
	virtual void Render_Scene(HDC hDC)  override;
	virtual void Release_Scene()		override;

private:

	void Exit_Button();
	void Sort_Button();
	void Name_Sort_Button();
	void Level_Sort_Button();
	void Cookie_Select();
	void Click();

public:

	static CScene* Create();

private:

	// X 버튼 변수
	ULONGLONG				m_tLobbyDelay;
	bool					m_bExitButton;
	bool					m_bLobbyScene;

	// 정렬 버튼 변수
	bool					m_bSortButton;
	bool					m_bNameSort;
	bool					m_bLevelSort;

	struct tagSlot
	{
		RECT tSlot_01 = { 40,   342,  240, 542 };
		RECT tSlot_02 = { 290,  342,  490, 542 };
		RECT tSlot_03 = { 540,  342,  740, 542 };
		RECT tSlot_04 = { 790,  342,  990, 542 };
		RECT tSlot_05 = { 1040, 342, 1240, 542 };
	}SLOT;
};

