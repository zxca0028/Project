#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum final : public CComponent
{
private:

	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFrustum() = default;

public:

	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;

public:

	void Update_Frustum();
	void Transform_ToLocalSpace(_matrix WorldMatrix);
	void Transform_ToWorldSpace();

	bool isIn(_float3 vPosition, _float fRange = 0.f);

private:

	_float3			m_vOriginal_Point[8];
	_float3			m_vWorldPoint[8];
	D3DXPLANE		m_Plane[6];

public:

	static  CFrustum* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END