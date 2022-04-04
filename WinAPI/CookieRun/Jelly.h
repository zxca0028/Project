#pragma once

#include "GameObject.h"

class CJelly final : public CGameObject
{
private:

	explicit CJelly();

public:

	virtual ~CJelly();

public:

	// CGameObject을(를) 통해 상속됨
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject()		override;	

	bool OnCollision();
	void Set_JellyType(JELLY::TYPE eType) { m_eType = eType; }

	void SizingObject();
	void IsRise() { m_bRise = true; }

public:

	static CGameObject* Create(JELLY::TYPE eType, float fX, float fY);

private:

	FRAME			m_tFrame;
	JELLY::TYPE		m_eType;
	bool		    m_bRise;

	enum INDEX
	{
		BIG_GOLD_BEAR	= 0,
		BIG_PINK_BEAR	= 140,
		RAINBOW_BEAR	= 280,
		BIG_SILVER_COIN = 410,
		BIG_GOLD_COIN	= 570,
		SILVER_COIN		= 730,
		GOLD_COIN		= 790,
		GOLD_BEAR		= 850,
		PINK_BEAR		= 910,
		NORMAL			= 970
	};

	enum SCORE
	{
		SCORE_NORMAL_JELLY	= 9444,

		SCORE_GOLD_BEAR		= 20555,
		SCORE_BIG_GOLD_BEAR	= 66666,

		SCORE_PINK_BEAR		= 29888,
		SCORE_BIG_PINK_BEAR	= 77777,

		SCORE_RAINBOW_BEAR	= 135555,
		
		SCORE_SILVER_COIN		= 2333,
		SCORE_BIG_SILVER_COIN = 13333,
		
		SCORE_GOLD_COIN		= 3222,
		SCORE_BIG_GOLD_COIN	= 26666
	};

	float m_fScale = 1.f;
};

