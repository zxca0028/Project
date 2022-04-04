#pragma once

#include "Component.h"

BEGIN(Client)

class CTexture final : public CComponent
{
private:

	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTexture() = default;

public:

	LPDIRECT3DBASETEXTURE9 Get_Texture(_uint iIndex)
	{
		return m_Textures[iIndex];
	}

public:

	HRESULT NativeConstruct(wstring pTextureFilePath, _uint iNumTexture);

private:

	vector<LPDIRECT3DBASETEXTURE9> m_Textures;

public:

	static  CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, wstring pTextureFilePath, _uint iNumTexture = 1);
	virtual void Free() override;

};

END