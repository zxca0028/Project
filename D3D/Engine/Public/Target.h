#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget final : public CBase
{
private:

	explicit CTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTarget() = default;

public:

	LPDIRECT3DTEXTURE9 Get_TargetTexture()
	{
		return m_pTexture;
	}

public:

	HRESULT NativeConstruct(_uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
	HRESULT Bind_RenderTarget(_uint iIndex);
	HRESULT Release_RenderTarget(_uint iIndex);
	HRESULT Clear();

#ifdef _DEBUG

public:
	
	HRESULT Ready_Debug_Buffer(_float fLeftX, _float fTopY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer();

private:

	class CVIBuffer_Rect_Viewport* m_pVIBuffer = nullptr;
	class CShader*				   m_pShader   = nullptr;

#endif

private:

	LPDIRECT3DDEVICE9  m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9 m_pTexture		 = nullptr;
	LPDIRECT3DSURFACE9 m_pOldSurface	 = nullptr;
	LPDIRECT3DSURFACE9 m_pSurface		 = nullptr;
	D3DXCOLOR		   m_ClearColor;

public:

	static  CTarget* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
	virtual void Free() override;

};

END