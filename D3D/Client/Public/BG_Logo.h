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

class CBG_Logo final : public CGameObject
{
private:

	explicit CBG_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBG_Logo(const CBG_Logo& rhs);
	virtual ~CBG_Logo() = default;

public:

	virtual HRESULT		NativeConstruct_Prototype()   override;
	virtual HRESULT		NativeConstruct(void* pArg)	  override;
	virtual _int		Tick(_double DeltaTime)		  override;
	virtual _int		Late_Tick(_double DeltaTime)  override;
	virtual HRESULT		Render()					  override;

private:

	CShader*	    m_pShaderCom    = nullptr;
	CRenderer*	    m_pRendererCom  = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom  = nullptr;
	CTexture*		m_pTextureCom   = nullptr;

private:

	HRESULT			Add_Components();

public:

	static CBG_Logo*  Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free() override;

};

END