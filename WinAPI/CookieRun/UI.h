#pragma once

#include "GameObject.h"
#include "Controller.h"

class CUI final : public CGameObject
{
private:

	explicit CUI();

public:

	virtual ~CUI();

public:

	// CGameObject을(를) 통해 상속됨
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject()		override;

	BONUSTIME*  Get_BonusTime() { return &m_tBonusTime; }
	void		Check_BonusTime();

	void		Set_PlusHP(ITEM::TYPE eType)
	{
		if (eType == ITEM::TYPE::ENERGY_JELLY)
		{
			if (m_iGauge < 885)
			{
				m_iGauge += PLUSHP_SMALL;
			}
		}
		
		if (eType == ITEM::TYPE::BIG_ENERGY_JELLY)
		{
			if (m_iGauge < 825)
			{
				m_iGauge += PLUSHP_BIG;
			}
		}
	}
	void		Set_MinusHP()
	{
		m_iGauge -= MINUSHP;
	}

public:

	static CGameObject* Create();

private:

	ULONGLONG	m_tLifeTime;
	int			m_iGauge;
	bool		m_bGaugeStop;

	ULONGLONG	m_tBonusTimeStart;
	ULONGLONG	m_tBonusTimeEnd;
	BONUSTIME	m_tBonusTime;

	bool		m_bAlphaPlus_IN;
	bool		m_bAlphaMinus_IN;
	bool		m_bAlphaPlus_OUT;
	bool		m_bAlphaMinus_OUT;
	int			m_iAlpha;

	bool		m_bTemp;
	bool		m_bTemp2;
	bool		m_bTemp3;
	bool		m_bTemp4;

	FRAME		m_tFrame;
	RECT		m_tEffectRect;
	INFO		m_tEffectInfo;

	bool		m_bExhaustLock;

	bool		m_bBonusTimeGauge;
	bool		m_bBonusTimeGaugeDecrease;
	float		m_fBonusTimeGaugeDecrease;

	ULONGLONG   m_tProgress;
	float		m_fProgressX = 0.f;

	int			m_iHP = 0;
};

