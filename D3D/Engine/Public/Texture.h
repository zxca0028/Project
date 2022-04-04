#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:

	enum TYPE { TYPE_2D, TYPE_UI, TYPE_CUBE, TYPE_END };

private:

	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:

	virtual HRESULT NativeConstruct_Prototype(TYPE eType, const _tchar * pTextureFilePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct(void* pArg);

public:

	LPDIRECT3DBASETEXTURE9 Get_Texture(_uint iIndex);
	HRESULT Set_Texture(_uint iStage, _uint iTextureIndex = 0);

private:

	vector<IDirect3DBaseTexture9*> m_Textures;

public:

	static  CTexture*   Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar * pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void        Free() override;

};

END