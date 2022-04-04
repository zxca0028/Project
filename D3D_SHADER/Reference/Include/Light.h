#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:

	explicit CLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLight() = default;

public:

	HRESULT NativeConstruct(const D3DLIGHT9_DERIVED& LightDesc);
	HRESULT Render();

public:

	_matrix Get_LightTransform(D3DTRANSFORMSTATETYPE eStateType);

private:

	D3DLIGHT9_DERIVED m_LightDesc;
	LPDIRECT3DDEVICE9 m_pGraphic_Device;

	class CShader*			  m_pShader   = nullptr;
	class CVIBuffer_Viewport* m_pVIBuffer = nullptr;

private:

	_matrix m_ViewMatrix;
	_matrix m_ProjMatrix;
	
public:

	static  CLight* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9_DERIVED& LightDesc);
	virtual void Free() override;

};

END