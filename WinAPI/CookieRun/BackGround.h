#pragma once

#include "GameObject.h"

class CBackGround final : public CGameObject
{
private:

	explicit CBackGround();

public:

	virtual ~CBackGround();

public:

	// CGameObject을(를) 통해 상속됨
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject()		override;

public:

	void Set_BG_Type(BACKGROUND::TYPE eType) { m_eType = eType; }

public:

	static CGameObject* Create(BACKGROUND::TYPE eType, PHASE::TYPE ePhase, float fX, float fY);

	void     Set_Phase(PHASE::TYPE ePhase) { m_ePhase = ePhase; }

private:

	PHASE::TYPE		m_ePhase;
	BACKGROUND::TYPE	m_eType;

};

