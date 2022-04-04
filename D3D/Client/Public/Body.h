#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CTransform;
END

BEGIN(Client)

class CBody final : public CGameObject
{
private:

	explicit CBody(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBody(const CBody& rhs);
	virtual ~CBody() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	CCollider* m_pOBB = nullptr;
	CTransform* m_pTransformCom = nullptr;

private:

	_matrix		   m_OriginMatrix;
	const _matrix* m_pParentBoneMatrix;
	const _matrix* m_pParentWorldMatrix;

private:

	_float3 vPos = _float3(-1.f, 0.f, -0.4f);
	_float3 vScale = _float3(6.9f, 3.7f, 3.6f);

private:

	HRESULT Add_Components(void* pArg = nullptr);

public:

	static  CBody* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free()						 override;

};

END