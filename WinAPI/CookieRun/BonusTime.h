#pragma once

#include "Scene.h"

class CBonusTime final : public CScene
{
private:

	explicit CBonusTime();

public:

	virtual ~CBonusTime();

public:

	// CScene��(��) ���� ��ӵ�
	virtual int Ready_Scene()			override;
	virtual void Update_Scene()			override;
	virtual void Render_Scene(HDC hDC)  override;
	virtual void Release_Scene()		override;

public:

	static CScene* Create();
};

