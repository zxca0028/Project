#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:

	enum class TYPE { TYPE_2D, TYPE_CUBE, TYPE_UI ,TYPE_END };

private:

	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:

	LPDIRECT3DBASETEXTURE9 Get_Texture(_uint iIndex)
	{
		return m_Textures[iIndex];
	}

public:

	HRESULT NativeConstruct_Prototype(TYPE eType, wstring pTextureFilePath, _uint iNumTexture);
	HRESULT NativeConstruct(void* pArg);

public:

	vector<IDirect3DBaseTexture9*>	m_Textures;

public:

	static  CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, wstring pTextureFilePath, _uint iNumTexture = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END