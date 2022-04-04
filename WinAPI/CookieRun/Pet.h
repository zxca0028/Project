#pragma once

#include "GameObject.h"

class CPet final : public CGameObject
{
private:

	explicit CPet();

public:

	virtual ~CPet();

public:

	// CGameObject을(를) 통해 상속됨
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject()		override;

public:

	void MoveFrame();

	bool Get_MagneticForce() { return m_bMagneticForce; }
	void Set_MagneticForce()
	{ 
		m_bMagneticForce = true;
		m_tMagneticTime = GetTickCount64();
	}

public:

	static CGameObject* Create();

private:

	ULONGLONG	m_tMagneticTime;
	bool		m_bMagneticForce;

	bool		m_bPetOnOff;

	FRAME		m_tFrame;

};

