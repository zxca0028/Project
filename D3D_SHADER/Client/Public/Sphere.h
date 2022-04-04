#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CPicking;
class CRenderer;
class CCollider;
class CTransform;
END

BEGIN(Client)

class CSphere final : public CGameObject
{
private:

	explicit CSphere(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSphere(const CSphere& rhs);
	virtual ~CSphere() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()	      override;
	virtual HRESULT NativeConstruct(void* pArg)	      override;
	virtual _int    Tick(_double DeltaTime)      override;
	virtual _int    Late_Tick(_double DeltaTime)      override;
	virtual HRESULT Render(TYPE eType = TYPE::NORMAL) override;

private:

	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Set_ConstTable()		   override;

private:

	CPicking*	m_pPicking   = nullptr;
	LPD3DXMESH	m_pMesh      = nullptr;
	CShader*    m_pShader    = nullptr;
	CRenderer*  m_pRenderer  = nullptr;
	CCollider*  m_pCollider  = nullptr;
	CTransform* m_pTransform = nullptr;

public:

	static  CSphere* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END