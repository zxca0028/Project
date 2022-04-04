#pragma once

#include "GameObject.h"

BEGIN(Client)

class CBox final : public CGameObject
{
public:

	typedef struct tagBoxDesc
	{
		_float3 vPos      = _float3(0.f, 0.f, 0.f);
		_float3 vScale    = _float3(1.f, 1.f, 0.f);

	}BOXDESC;

private:

	explicit CBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBox() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg)	 override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

public:

	virtual HRESULT Add_Components(void* pArg = nullptr) override;
	virtual HRESULT Set_ConstTable() override;

private:

	BOXDESC m_BoxDesc;

private:

	class CCollider*  m_pCollider  = nullptr;
	class CTransform* m_pTransform = nullptr;

public:

	static  CBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual void Free() override;

};

END