#pragma once

#include "GameObject.h"

BEGIN(Client)

class CResult final : public CGameObject
{
private:

	explicit CResult(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CResult() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg)	 override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

public:

	virtual HRESULT Add_Components(void* pArg = nullptr) override;
	virtual HRESULT Set_ConstTable() override;

private:

	class CShader*        m_pShader    = nullptr;
	class CTexture*       m_pTexture   = nullptr;
	class CCollider*      m_pCollider  = nullptr;
	class CTransform*     m_pTransform = nullptr;
	class CVIBuffer_Rect* m_pVIBuffer  = nullptr;

public:

	static  CResult* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual void Free() override;

};

END