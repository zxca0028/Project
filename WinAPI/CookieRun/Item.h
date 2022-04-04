#pragma once

#include "GameObject.h"

class CItem final : public CGameObject
{
private:

	explicit CItem();

public:

	virtual ~CItem();

public:

	// CGameObject��(��) ���� ��ӵ�
	virtual  int Ready_GameObject()			override;
	virtual  int Update_GameObject()		override;
	virtual void Late_Update_GameObject()	override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject()		override;

public:

	void MoveFrame();
	void SizingObject();
	bool OnCollision();

	void Set_ItemType(ITEM::TYPE eType) { m_eType = eType; }

public:

	static CGameObject* Create(ITEM::TYPE eType, float fX, float fY);

private:

	ITEM::TYPE		m_eType;

	FRAME			m_tFrame;
	int				m_iFrameStart;

	ULONGLONG		m_tBonusTimeIntro;

};

