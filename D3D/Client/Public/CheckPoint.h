#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CCheckPoint final : public CGameObject
{
private:

	explicit CCheckPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCheckPoint(const CCheckPoint& rhs);
	virtual ~CCheckPoint() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	CRenderer*  m_pRenderer  = nullptr;
	CTransform* m_pTransform = nullptr;

private:

	HRESULT Add_Components(void* pArg = nullptr);

private:

	_float3 vPos   = _float3(0.f, 0.f, 0.f);
	_float3 vScale =_float3(3.f, 3.f, 3.f);

public:

	static  CCheckPoint* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END