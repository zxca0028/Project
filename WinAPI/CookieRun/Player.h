#pragma once

#include "GameObject.h"

class CPlayer final : public CGameObject
{
private:

	explicit CPlayer();

public:

	virtual ~CPlayer();

public:

	// CObject을(를) 통해 상속됨
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC)	override;
	virtual void Release_GameObject()		override;

public:

	void MoveFrame();
	bool Get_Sliding(){ return m_bSliding; }

	bool Get_Blasting() { return m_bBlast; }
	void Set_Blasting()
	{
		if (m_eType != STAGE::TYPE::BT_OUT_1)
		{
			m_bBlast = true;
			m_tBlastTime = GetTickCount64();
		}
		else
		{
			m_bBlast = false;
		}
	}

	bool Get_Biggest() { return m_bBiggest; }
	void Set_Biggest()
	{
		m_bBiggest = true;
		m_tBiggestTime = GetTickCount64();
	}
	void Set_Collision()				  
	{ 
		if (m_bExhaust == false )
		{
			if (m_bInvincible == false)
			{
				if (m_bBlast == false && m_bBiggest == false)
				{
					m_bInvincible = true;
					m_bCollision = true;
					m_tFrame.iFrameState = PLAYER::STATE::CRASH;
					m_tFrame.iFrameStart = 0;
					m_tFrame.iFrameEnd = 3;
					m_tCollisionTime = GetTickCount64();
					m_tJumpingTime = GetTickCount64();
					m_tInvincibleTime = GetTickCount64();
				}
			}					
		}		
	}
	void Set_StageType(STAGE::TYPE eType)
	{ 
		m_eType = eType; 
		m_tIntroTime = GetTickCount64();
		m_tOutroTime = GetTickCount64();
		if (eType == STAGE::TYPE::BT_OUT_2)
		{
			m_bSetPosition = true;
		}
	}
	void Set_Exhaust() 
	{ 
		m_bExhaust = true;
		m_bExhaustTime = GetTickCount64();
		m_tJumpingTime = GetTickCount64();
		m_tFrame.iFrameState = PLAYER::STATE::EXHAUST;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 4;
	}

	void Set_Cookie(const TCHAR* pCookieName) { m_pCookieName = pCookieName; }

	bool Get_Invincible() { return m_bInvincible; }

public:

	static CGameObject* Create(const TCHAR* pCookieName);

private:

	const TCHAR*	m_pCookieName = L"BlackBerryCookie";

	FRAME			m_tFrame;

private:

	bool			m_bRunning;
	bool			m_bSliding;
	bool			m_bJumping;
	bool			m_bBlast;
	bool			m_bDive;
	bool			m_bBiggest;
	bool			m_bCollision;
	bool			m_bExhaust;
	bool			m_bInvincible;	

	ULONGLONG		m_tJumpingTime;
	float			m_fJumpSpeed;
	int				m_iJumpCount;
	int				m_iSpinCount;

	ULONGLONG		m_tBlastTime;

	ULONGLONG		m_tBiggestTime;

	ULONGLONG		m_tCollisionTime;

	STAGE::TYPE		m_eType;
	ULONGLONG		m_tIntroTime;
	ULONGLONG		m_tOutroTime;

	bool			m_bSetPosition;
	ULONGLONG		m_bExhaustTime;

	ULONGLONG		m_bDiveTime;

	ULONGLONG		m_tInvincibleTime;

	ULONGLONG		m_tFireDelay;

	static int		m_iSlideCount;
	bool			m_bExhaustSound = false;
	bool			m_bDiveSound = false;
};

