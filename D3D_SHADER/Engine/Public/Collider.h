#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	
	typedef struct tagColliderDesc
	{
		const _matrix* pParentMatrix;

		_float3 vScale   = _float3(1.f, 1.f, 1.f);
		_float3 vInitPos = _float3(0.f, 0.f, 0.f);

	}COLLIDERDESC;

private:

	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render() override;

public:

	void Update_Collider();

public:

	PICKINGDESC Get_PickingDesc();

private:

	class CShader*    m_pShader	   = nullptr;
	class CTransform* m_pTransform = nullptr;
	LPD3DXMESH		  m_pMesh	   = nullptr;

private:

	COLLIDERDESC m_ColliderDesc;
	PICKINGDESC  m_PickingDesc;

	_matrix m_OriginMatrix;
	_matrix m_ParentMatrix;

	_float3 m_vOriginPos;

public:

	static  CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END