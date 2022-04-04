#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:

	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:

	virtual HRESULT NativeConstruct_Prototype(const _tchar * pShaderPilePath);
	virtual HRESULT NativeConstruct(void* pArg);

public:

	HRESULT Set_ConstTable(D3DXHANDLE hHnd, const void* pData, _uint iLengthByte);
	HRESULT Set_Bool(D3DXHANDLE hHandle, BOOL isBool);
	HRESULT Set_TextureConstTable(D3DXHANDLE hHnd, class CTexture* pTextureCom, _uint iTextureIndex = 0);
	HRESULT Set_TextureConstTable(D3DXHANDLE hHnd, LPDIRECT3DBASETEXTURE9 pTexture);
	HRESULT Begin_Shader(_uint iPassIndex);
	HRESULT End_Shader();

	void    Commit();

protected:

	LPD3DXEFFECT m_pEffect = nullptr;

public:

	static  CShader*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pShaderPilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void		Free() override;

};

END