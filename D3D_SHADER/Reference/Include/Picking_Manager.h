#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPicking_Manager final : public CBase
{
	DECLARE_SINGLETON(CPicking_Manager)

private:

	explicit CPicking_Manager();
	virtual ~CPicking_Manager() = default;

public:

	HRESULT NativeConstruct_Prototype(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd);
	_int    Tick(_float2 TextureSize);
	_int    Late_Tick();

public:

	_bool   IsClick() { return m_bIsClick; }
	_bool   IsIntersect_Ray(class CCollider* pCollider);

private:

	HRESULT Compute_MousePos(_float2 TextureSize);

private:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

private:

	HWND m_hWnd;

	_float3 m_vMouseRay;
	_float3 m_vMousePivot;
	_float3 m_vResultPos;

	_bool m_bIsClick = false;

public:

	virtual void Free() override;

};

END