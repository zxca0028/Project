#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
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

	virtual HRESULT NativeConstruct_Prototype()	      override;
	virtual HRESULT NativeConstruct(void* pArg)	      override;
	virtual _int    Tick     (_double DeltaTime)      override;
	virtual _int    Late_Tick(_double DeltaTime)      override;
	virtual HRESULT Render(TYPE eType = TYPE::NORMAL) override;

public:

	virtual HRESULT Add_Components(void* pArg = nullptr) override;
	virtual HRESULT Set_ConstTable()		    		 override;

private:

	CShader*		   m_pShader    = nullptr;
	CTexture*		   m_pTexture_D = nullptr;
	CTexture*		   m_pTexture_N = nullptr;
	CRenderer*		   m_pRenderer  = nullptr;
	CTransform*		   m_pTransform = nullptr;
	CVIBuffer_Terrain* m_pVIBuffer  = nullptr;

public:

	static  CTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END