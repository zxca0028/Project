#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:

	enum RENDER_GROUP { RENDER_PRIORITY, RENDER_SHADOWDEPTH ,RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

private:

	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:

	virtual HRESULT NativeConstruct_Prototype() override;

public:

	HRESULT Add_RenderGroup(RENDER_GROUP eGroup, class CGameObject* pGameObject);
	HRESULT Add_RenderDebug(class CComponent* pGameObject);
	HRESULT Render_GameObject(_bool bRenderDebug = true);

private:

	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();

	HRESULT Set_RenderTaget(D3DVIEWPORT9 Viewport);
	HRESULT Render_LightAcc();
	HRESULT Render_Blend();
	HRESULT Render_ShadowDepth();

private:

	list<class CGameObject*> m_RenderObjects[RENDER_END];

#ifdef _DEBUG

private:

	list<class CComponent*> m_DebugObject;

#endif

private:

	class CTarget_Manager*		   m_pTarget_Manager = nullptr;
	class CShader*				   m_pShader		 = nullptr;
	class CVIBuffer_Rect_Viewport* m_pVIBuffer	     = nullptr;

	LPDIRECT3DSURFACE9	m_pOriginal_DS_Surface = nullptr;
	LPDIRECT3DSURFACE9	m_pShadow_DS_Surface   = nullptr;

public:

	static CRenderer*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void		Free() override;

};

END