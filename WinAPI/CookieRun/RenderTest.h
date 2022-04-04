#pragma once

#include "Scene.h"

class CRenderTest final : public CScene
{
private:

	explicit CRenderTest();

public:

	virtual ~CRenderTest();

public:

	// CScene을(를) 통해 상속됨
	virtual  int Ready_Scene()			override;
	virtual void Update_Scene()			override;
	virtual void Render_Scene(HDC hDC)  override;
	virtual void Release_Scene()		override;

public:

	static CScene* Create();

	void MyBestScore(HDC hDC);

private:

	INFO		m_tCollisionInfo;
	RECT		m_tCollisionRect;
	bool		m_bCollisionView;

	INFO		m_tInfo;
	RECT		m_tRect;

	int			m_iHiScore;
};

