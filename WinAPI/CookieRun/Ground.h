#pragma once

#include "GameObject.h"

class CGround final : public CGameObject
{
private:

	explicit CGround();

public:

	virtual ~CGround();

public:

	// CGameObject을(를) 통해 상속됨
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject()		override;

public:

	GROUND::TYPE Get_GroundType() { return m_eType; }
	void		 Set_GroundType(GROUND::TYPE eType) { m_eType = eType; }

	bool		 Get_IsObstacle() { return m_bSet_IsObstacle; }
	void		 Set_IsObstacle() { m_bSet_IsObstacle = true; }

	bool		 Get_IsJelly() { return m_bSet_IsJelly; }
	void		 Set_IsJelly() { m_bSet_IsJelly = true; }

	bool		 Get_IsVoid() { return m_bSet_IsVoid; }
	void		 Set_IsVoid() { m_bSet_IsVoid = true; }

	bool		 Get_IsVoid2() { return m_bSet_IsVoid2; }
	void		 Set_IsVoid2() { m_bSet_IsVoid2 = true; }

	bool		 Get_IsJ2() { return m_bSetJ2; }
	void		 Set_IsJ2() { m_bSetJ2 = true; }

public:

	static CGameObject* Create(GROUND::TYPE eType, PHASE::TYPE ePhase, float fX, float fY);

	bool IsCreateJellyLock() { return m_bCreateJellyLock; }
	void CreateJellyLock() { m_bCreateJellyLock = true; }
	void     Set_Phase(PHASE::TYPE ePhase) { m_ePhase = ePhase; }

private:

	PHASE::TYPE		m_ePhase;
	GROUND::TYPE	m_eType;

	enum INDEX
	{
		NORMAL = 0,
		_VOID = 140,
		_VOID2 = 280
	};

	int				m_iType;

	bool			m_bSet_IsObstacle;
	bool			m_bSet_IsJelly;
	bool			m_bSet_IsVoid;
	bool			m_bSet_IsVoid2;

	bool		    m_bSetJ2;

	bool		    m_bCreateJellyLock;

};