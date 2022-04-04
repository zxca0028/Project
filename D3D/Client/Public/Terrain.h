#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CFrustum;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
private:

	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double TimeDelta)		 override;
	virtual _int    Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render()					 override;

private:

	CShader*		   m_pShader	= nullptr;
	CTexture*		   m_pTexture = nullptr;
	CTexture*		   m_pTexture_N   = nullptr;
	CFrustum*		   m_pFrustum   = nullptr;
	CRenderer*		   m_pRenderer  = nullptr;
	CTransform*		   m_pTransform = nullptr;
	CVIBuffer_Terrain* m_pVIBuffer  = nullptr;

private:

	HRESULT Add_Components();

public:

	static  CTerrain*    Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void		 Free() override;

};

END