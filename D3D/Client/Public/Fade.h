#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CFade final : public CGameObject
{
private:

	explicit CFade(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFade(const CFade& rhs);
	virtual ~CFade() = default;

public:

	virtual HRESULT		NativeConstruct_Prototype()   override;
	virtual HRESULT		NativeConstruct(void* pArg)	  override;
	virtual _int		Tick(_double DeltaTime)		  override;
	virtual _int		Late_Tick(_double DeltaTime)  override;
	virtual HRESULT		Render()					  override;

private:

	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

private:

	_float m_fFadeIn  = 0.f;
	_float m_fFadeOut = 1.f;

private:

	HRESULT			Add_Components();

public:

	static CFade* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free() override;

};

END