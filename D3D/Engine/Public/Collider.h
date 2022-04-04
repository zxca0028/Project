#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent 
{
public:

	enum COLLIDER { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:

	typedef struct tagColliderDesc
	{
		const _matrix* pParentMatrix;
		_float3        vScale   = _float3(1.f, 1.f, 1.f);
		_float3		   vRadians = _float3(0.f, 0.f, 0.f);
		_float3		   vInitPos = _float3(0.f, 0.f, 0.f);

	}COLLIDERDESC;

	typedef struct tagOBBDesc
	{
		_float3 vCenter;
		_float3 vAlignAxis[3];
		_float3 vCenterAxis[3];

	}OBBDESC;

private:

	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	HRESULT NativeConstruct_Prototype(COLLIDER eType);
	HRESULT NativeConstruct(void* pArg);
	void    Update_Collider();
	void    Set_Collider(_float3 vScale, _float3 vPos);

public:
	bool Collision_AABB  (CCollider* pTargetCollider);
	bool Collision_OBB   (CCollider* pTargetCollider);
	bool Collision_SPHERE(CCollider* pTargetCollider);

public:

	void Set_CollisionState(bool isCollision)
	{
		m_isCollision = isCollision;
	}
	
private:

	OBBDESC Compute_OBBDesc(_float3* pPoints);

private:

	COLLIDER			m_eType		  = TYPE_END;
	LPD3DXMESH			m_pMesh		  = nullptr;
	bool				m_isCollision = false;
	class CTransform*   m_pTransform  = nullptr;

	COLLIDERDESC		m_ColliderDesc;
	_matrix				m_OriginalMatrix;
	_matrix				m_ParentMatrix;
	_float3				m_vMin;
	_float3			    m_vMax;
	_float3				m_vPoints[8];

	_float3				vOriginPos;

	_float				m_fRadius = 0.5f;

#ifdef _DEBUG
public:

	HRESULT Render_Debug();
	HRESULT Render();

private:

	class CShader* m_pShader = nullptr;

	HRESULT Compile_ShaderDebug();

#endif

public:

	static  CCollider*  Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER eType);
	virtual CComponent* Clone(void* pArg);
	virtual void		Free() override;

};

END