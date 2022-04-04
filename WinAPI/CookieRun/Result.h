#pragma once

#include "Scene.h"

class CResult final : public CScene
{
private:

	explicit CResult();

public:

	virtual ~CResult();

public:

	// CScene을(를) 통해 상속됨
	virtual  int Ready_Scene()			override;
	virtual void Update_Scene()			override;
	virtual void Render_Scene(HDC hDC)  override;
	virtual void Release_Scene()		override;

public:

	static CScene* Create();

public:

	INFO		m_tInfo;
	RECT		m_tRect;

	bool		m_bDownLock;
	bool		m_bUpLock;

	bool		m_bScoreRender;
	bool		m_bPlusLock;
	int			m_iTemp;

	int			m_iMoveX;
	int			m_iMoveY;

	bool		m_bHiScoreRender;

	ULONGLONG   m_bCoinRenderDelay;
	bool		m_bCoinRender;

	int			m_iAlpha;

	ULONGLONG	m_bLobbyDelay;
	bool		m_bLobbyScene;
	bool		m_bOKButton;

	FRAME		m_tFrame;

	int			m_iCoinTemp;
	bool		m_bCoinTemp;
};

