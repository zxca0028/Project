#pragma once

#include "Scene.h"

class CLoading final : public CScene
{
private:

	explicit CLoading();

public:

	virtual ~CLoading();

public:

	// CScene��(��) ���� ��ӵ�
	virtual  int Ready_Scene()			override;
	virtual void Update_Scene()			override;
	virtual void Render_Scene(HDC hDC)  override;
	virtual void Release_Scene()		override;

public:

	static CScene* Create();

};

