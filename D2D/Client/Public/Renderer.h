#pragma once

#include "Base.h"

BEGIN(Client)

class CRenderer final : public CBase
{
	DECLARE_SINGLETON(CRenderer);

public:

	enum RENDERGROUP { RENDER_BACKGROUND, RENDER_OBJECT, RENDER_PLAYER, RENDER_UI, RENDER_SCORE, RENDER_END };

private:

	explicit CRenderer();
	virtual ~CRenderer() = default;

public:

	HRESULT NativeConstruct();
	HRESULT Render();

public:

	HRESULT Add_RenderObject(RENDERGROUP eGroup, class CGameObject* pGameObject);

private:

	HRESULT Render_BackGround();
	HRESULT Render_Object();
	HRESULT Render_Player();
	HRESULT Render_UI();
	HRESULT Render_Score();

private:

	vector<class CGameObject*> m_RenderObjects[RENDER_END];


#ifdef _DEBUG

public:

	HRESULT Add_RenderComponent(class CComponent* pComponent);

private:

	HRESULT Render_Collider();

private:

	vector<class CComponent*> m_RenderComponent;

#endif


public:

	virtual void Free() override;

};

END