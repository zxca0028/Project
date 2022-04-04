#pragma once

#include "Scene.h"

class CStageLoading final : public CScene
{
private:

	explicit CStageLoading();

public:

	virtual ~CStageLoading();

public:

	// CScene을(를) 통해 상속됨
	virtual  int Ready_Scene()			override;
	virtual void Update_Scene()			override;
	virtual void Render_Scene(HDC hDC)  override;
	virtual void Release_Scene()		override;

public:

	static CScene* Create();

private:

	ULONGLONG	m_tTime;

	bool m_bTemp;

	const TCHAR* m_pSentence;
	bool		 m_bSentenceLock;

};

