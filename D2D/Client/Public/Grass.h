#pragma once

#include "GameObject.h"

BEGIN(Client)

class CGrass final : public CGameObject
{
public:

	typedef struct tagGrassDesc
	{
		_float3 vInitPos;
		_float3 vScale;

		_float fDeletePos;
		_float fSpeed;

	}GRASSDESC;

private:

	explicit CGrass(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CGrass() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()				     override;

private:

	virtual HRESULT Add_Components(void* pArg = nullptr) override;
	virtual HRESULT Set_ConstTable() override;

private:

	class CShader*        m_pShader    = nullptr;
	class CTexture*       m_pTexture   = nullptr;
	class CTransform*     m_pTransform = nullptr;
	class CVIBuffer_Rect* m_pVIBuffer  = nullptr;

private:

	GRASSDESC m_GrassDesc;

public:

	static  CGrass* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual void Free() override;

};

END