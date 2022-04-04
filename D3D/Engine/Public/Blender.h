#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CBlender abstract : public CGameObject
{
protected:

	explicit CBlender(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBlender(const CBlender& rhs);
	virtual ~CBlender() = default;

public:

	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int    Tick(_double DeltaTime);
	virtual _int    Late_Tick(_double DeltaTime);
	virtual HRESULT Render();

protected:

	void Compute_Z(class CTransform* pTransform);

public:

	const _float Get_DistZ() const
	{
		return m_fDistZ;
	}

private:

	_float m_fDistZ = 0.f;

public:

	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void         Free();

};

END