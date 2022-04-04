#pragma once

#include "GameObject.h"

BEGIN(Client)

class CPoint final : public CGameObject
{
private:

	explicit CPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPoint() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg)	 override;
	virtual _int    Tick(_double DeltaTime)      override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

public:

	virtual HRESULT Add_Components(void* pArg = nullptr) override;
	virtual HRESULT Set_ConstTable()					 override;

private:

	class CShader* m_pShader = nullptr;
	class CTexture* m_pTexture = nullptr;
	class CTransform* m_pTransform = nullptr;
	class CVIBuffer_Point* m_pVIBuffer = nullptr;


	class CCollider* m_pCollider = nullptr;
public:

	static  CPoint* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual void Free() override;

};

END