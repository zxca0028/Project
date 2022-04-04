#pragma once

#include "Base.h"

BEGIN(Client)

class CComponent abstract : public CBase
{
protected:

	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CComponent() = default;

public:

	virtual HRESULT NativeConstruct();
	virtual _int    Tick	 (_double DeltaTime);
	virtual _int    Late_Tick(_double DeltaTime);
	virtual HRESULT Render();

protected:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:

	virtual void Free() override;

};

END