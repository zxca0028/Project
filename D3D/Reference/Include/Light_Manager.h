#pragma once

#include "Base.h"
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager);

public:

	explicit CLight_Manager();
	virtual ~CLight_Manager() = default;

public:

	D3DLIGHT9 Get_LightDesc(_uint iIndex = 0);

public:

	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightDesc);
	HRESULT Render();

public:

	list<class CLight*>		    m_Lights;
	typedef list<class CLight*> LIGHTS;

public:

	virtual void Free() override;

};

END