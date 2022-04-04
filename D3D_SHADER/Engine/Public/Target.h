#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget final : public CBase
{
private:

	explicit CTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTarget() = default;

public:

	TARGETDESC Get_TargetDesc()
	{
		return m_TargetDesc;
	}

public:

	HRESULT NativeConstruct(_uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
	HRESULT Bind_RenderTarget   (_uint iIndex);
	HRESULT Release_RenderTarget(_uint iIndex);
	HRESULT Clear();

public:

	HRESULT Ready_Debug_Buffer(_float fLeftX, _float fTopY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer();

private:

	class CVIBuffer_Viewport* m_pVIBuffer = nullptr;
	class CShader* m_pShader = nullptr;

private:

	LPDIRECT3DDEVICE9  m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9 m_pTexture        = nullptr;
	LPDIRECT3DSURFACE9 m_pOldSurface     = nullptr;
	LPDIRECT3DSURFACE9 m_pSurface        = nullptr;
	D3DXCOLOR		   m_ClearColor;

	TARGETDESC m_TargetDesc;

public:

	static  CTarget* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
	virtual void Free() override;

};

END