#pragma once

#include "Component.h"

BEGIN(Client)

class CCollider final : public CComponent
{
public:

	typedef struct tagColliderDesc
	{
		const _matrix* pParentWorldMatrix = nullptr;

		_float3 vInitPos    = _float3(0.f, 0.f, 0.f);
		_float3 vInitScale  = _float3(1.f, 1.f, 0.f);

		_bool bCustomizable = false;

	}COLLIDERDESC;

private:

	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollider() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int    Tick     (_double DeltaTime) override;
	virtual HRESULT Render()				     override;

public:

	_bool m_bDebug = false;
	void Set_State(_float3 vPos, _float3 vScale) { m_bDebug = true; m_vPos = vPos; m_vScale = vScale; }
	
private:

	_float3 m_vPos;
	_float3 m_vScale;

public:

	_bool OnCollision(CCollider* pCollider);

private:

	_float3 m_vMin = _float3(-0.5f, -0.5f, 0.f);
	_float3 m_vMax = _float3( 0.5f,  0.5f, 0.f);

	bool m_bIsCollision = false;

private:

	HRESULT Add_Components();
	HRESULT Set_ConstTable();

private:

	class CShader*		  m_pShader    = nullptr;
	class CTransform*     m_pTransform = nullptr;
	class CVIBuffer_Rect* m_pVIBuffer  = nullptr;

private:

	_matrix		 m_IdentityMatrix;
	_matrix      m_ColliderWorldMatrix;
	COLLIDERDESC m_ColliderDesc;

public:

	static  CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual void Free() override;

};

END