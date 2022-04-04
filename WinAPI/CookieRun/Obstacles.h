#pragma once

#include "GameObject.h"

class CObstacles final : public CGameObject
{
private:

	explicit CObstacles();

public:

	virtual ~CObstacles();

public:

	// CGameObject을(를) 통해 상속됨
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject()		override;

	void OnCollision();

	OBSTACLE::TYPE	Get_ObstacleType() { return m_eType; }
	void			Set_IsObstacleType(OBSTACLE::TYPE eType) { m_eType = eType; }

public:

	static CGameObject* Create(OBSTACLE::TYPE eType, PHASE::TYPE ePhase, float fX, float fY);
	void     Set_Phase(PHASE::TYPE ePhase) { m_ePhase = ePhase; }

private:

	PHASE::TYPE		    m_ePhase;
	OBSTACLE::TYPE		m_eType;

	ULONGLONG			m_tCollisionTime;

	int					m_iState;

	bool				m_bCrash = false;
};

