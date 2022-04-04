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

class CBG_Loading final : public CGameObject
{
private:

	explicit CBG_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBG_Loading(const CBG_Loading& rhs);
	virtual ~CBG_Loading() = default;

public:
	
	virtual HRESULT     NativeConstruct_Prototype()	 override;
	virtual HRESULT     NativeConstruct(void* pArg)	 override;
	virtual _int	    Tick(_double DeltaTime)		 override;
	virtual _int	    Late_Tick(_double DeltaTime) override;
	virtual HRESULT     Render()					 override;

public:

	static CBG_Loading*		 Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void         Free() override;

private:

	HRESULT			Add_Component();

	CShader*		m_pShaderCom	= nullptr;
	CTexture*		m_pTextureCom	= nullptr;
	CRenderer*      m_pRendererCom  = nullptr;
	CTransform*	    m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom  = nullptr;

};

END