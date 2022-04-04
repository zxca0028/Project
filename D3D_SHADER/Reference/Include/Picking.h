#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CPicking final : public CComponent
{
private:

	explicit CPicking(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPicking() = default;

public:

	virtual HRESULT NativeConstruct_Prototype();

public:

	HRESULT Compute_MousePos(_float2 MousePos, _float2 TextureSize);
	
public:

	_bool Intersect_Ray(class CCollider* pCollider);

private:

	_float3 m_vMousePivot;
	_float3 m_vMouseRay;

public:

	static  CPicking* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END