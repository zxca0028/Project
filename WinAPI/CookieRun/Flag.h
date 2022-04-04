#pragma once

#include "GameObject.h"

class CFlag final : public CGameObject
{
private:

	explicit CFlag();

public:

	virtual ~CFlag();

public:

	// CGameObject을(를) 통해 상속됨
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject()		override;

public:

	static CGameObject* Create(float fX, float fY);

private:

	FRAME   m_tFrame	  = {};

};

