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

	virtual HRESULT NativeConstruct_Prototype(wstring pShaderFilePath);
	virtual HRESULT NativeConstruct(void* pArg);

public:

	HRESULT Set_ConstTable(D3DXHANDLE hHandle, const void* pData, _uint iSize);
	HRESULT Set_Texture(D3DXHANDLE hHandle, class CTexture* pTexture, _uint iTextureIndex = 0);
	HRESULT Set_Texture(D3DXHANDLE hHandle, LPDIRECT3DBASETEXTURE9 pTexture);
	HRESULT Set_Bool(D3DXHANDLE hHandle, BOOL ToF);
	HRESULT Begin_Shader(_uint iPassIndex);
	HRESULT End_Shader();
	void    Commit();

protected:

	LPD3DXEFFECT	m_pEffect = nullptr;

public:

	static  CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, wstring pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END