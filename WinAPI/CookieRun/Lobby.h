#pragma once

#include "Scene.h"

class CLobby final : public CScene
{
private:

	explicit CLobby();

public:

	virtual ~CLobby();

public:

	// CScene을(를) 통해 상속됨
	virtual  int Ready_Scene()			override;
	virtual void Update_Scene()			override;
	virtual void Render_Scene(HDC hDC)  override;
	virtual void Release_Scene()		override;

public:

	static CScene* Create();

	static int  m_iHaveCoin;

private:

	void Click();

private:

	FRAME		 m_tFrame;
				 
	ULONGLONG	 m_tPlayDelay;
	bool		 m_bPlayButton;
	bool		 m_bPlayScene;
				 
	ULONGLONG	 m_tCookiesDelay;
	bool		 m_bCookiesButton;
	bool		 m_bCookiesScene;

	int			 m_fX;
};

