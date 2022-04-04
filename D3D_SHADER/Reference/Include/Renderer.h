#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:

	enum class RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_SHADOW, RENDER_OUTLINE, RENDER_END };

private:

	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:

	HRESULT Add_RenderGroup(RENDERGROUP eGroup, class CGameObject* pGameObject);
	HRESULT Add_RenderGroup(class CComponent*  pComponent);

public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT Render()					override;

private:

	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();
	HRESULT Render_Shadow();
	HRESULT Render_Component();

private:

	HRESULT Render_Outline();
	HRESULT Render_LightAcc();
	HRESULT Render_Blend();

private:

	list<class CGameObject*>	m_RenderObjects[(_uint)RENDERGROUP::RENDER_END];
	list<class CComponent*>     m_RenderComponent;

private:

	class CShader*		   m_pShader		 = nullptr;
	class CTarget_Manager* m_pTarget_Manager = nullptr;

	class CVIBuffer_Viewport* m_pVIBuffer = nullptr;

private:

	LPDIRECT3DSURFACE9 m_pOriginalSurface = nullptr;
	LPDIRECT3DSURFACE9 m_pShadowSurface   = nullptr;

private:

	HRESULT Set_RenderTarget(D3DVIEWPORT9 ViewPort);	

public:

	static  CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END