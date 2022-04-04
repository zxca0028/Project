#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:

	explicit CLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLight() = default;

public:

	D3DLIGHT9 Get_LightDesc() const
	{
		return m_LightDesc;
	}

public:

	HRESULT NativeConstruct(const D3DLIGHT9& LightDesc);
	HRESULT Render();

public:

	D3DLIGHT9         m_LightDesc;
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

	class CShader* m_pShader = nullptr;
	class CVIBuffer_Rect_Viewport* m_pVIBuffer = nullptr;

public:

	static  CLight* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightDesc);
	virtual void    Free() override;

};

END