#pragma once

#include "GameObject.h"

class CEffect final : public CGameObject
{
private:

	explicit CEffect();

public:

	virtual ~CEffect();

public:	

	// CGameObject을(를) 통해 상속됨
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject()		override;
	
public:

	void Set_ItemType(ITEM::TYPE eType)
	{ 
		m_eType = eType;
		m_tEffectTime = GetTickCount64();
	}

	void Set_HeartInfo(float fX, float fY)
	{ 
		m_tHeartInfo.fX = fX; 
		m_tHeartInfo.fY = fY;
		m_bHeartRender = true;
		m_tHeartRender = GetTickCount64();
	}

public:

	static CGameObject* Create(ITEM::TYPE eType, float fX, float fY);

private:

	FRAME			m_tFrame;

	ITEM::TYPE		m_eType;


	ULONGLONG		m_tEffectTime;

	bool			m_bTemp;
	int				m_iAlpha;

	INFO			m_tHeartInfo;
	RECT			m_tHeartRect;
	bool			m_bHeartEffect;

	ULONGLONG		m_tHeartRender;
	bool			m_bHeartRender;

	RECT			m_tFireRect;
	bool			m_bFireEffect;

	bool			m_bTwinkleEffect;

};

