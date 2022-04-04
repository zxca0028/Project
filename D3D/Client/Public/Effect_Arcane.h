#pragma once

#include "Blender.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEffect_Arcane final : public CBlender
{
private:

	explicit CEffect_Arcane(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_Arcane(const CEffect_Arcane& rhs);
	virtual ~CEffect_Arcane() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double TimeDelta)		 override;
	virtual _int    Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render()					 override;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components();

private:

	CShader* m_pShader = nullptr;
	CTexture* m_pTexture = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTransform* m_pTransform = nullptr;
	CVIBuffer_Rect* m_pVIBuffer = nullptr;

private:

	_float m_fFrame = 0.f;

	_float m_fScale = 0.f;

public:

	static  CEffect_Arcane* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free() override;

};

END
