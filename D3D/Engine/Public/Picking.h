#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CPicking final : public CComponent
{
private:

	explicit CPicking(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPicking() = default;

public:

	virtual HRESULT NativeConstruct_Prototype() override;

public:

	HRESULT Compute_MousePosInWorld(HWND hWnd);
	_float3* Compute_PickingPoint(class CVIBuffer* pVIBuffer, _matrix _WorldMatrix);
	_float3* Compute_PickingPoint(class CNavigation* pNavigation, _float3 vPlayerPos);

private:

	_float3 m_vMouseRay   = {};
	_float3 m_vMousePivot = {};
	_float3 m_vResultPos  = {};

private:

	_uint m_iCount = 0;

public:

	static  CPicking*   Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void        Free() override;

};

END