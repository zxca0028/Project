#pragma once

#include "Scene.h"

class CStage final : public CScene
{
private:

	explicit CStage();

public:

	virtual ~CStage();

public:

	// CScene을(를) 통해 상속됨
	virtual  int Ready_Scene()			override;
	virtual void Update_Scene()			override;
	virtual void Render_Scene(HDC hDC)  override;
	virtual void Release_Scene()		override;

private:

	void Cheat();

public:

	void IsPlayerDead() { m_bIsPlayerDead = true; }
	void Set_BonusTime()
	{ 
		if (m_bIsBonusTime == false)
		{
			m_bIsBonusTime = true;
		}
		else if (m_bIsBonusTime == true)
		{
			m_bIsBonusTime = false;
		}
		m_tBonusTime = GetTickCount64();
	}

public:

	static CScene* Create();

public:

	bool			m_bIsPlayerDead;
	bool			m_bIsBonusTime;
	ULONGLONG		m_tBonusTime;
	int				m_iDistance;

	ULONGLONG		m_tFeverTimeStart;
	ULONGLONG		m_tFeverTimeEnd;
	bool			m_bFeverTime;
	bool			m_bLock;

	int				m_iFeverCount = 0;
	bool			m_bFlagLock = false;

};

