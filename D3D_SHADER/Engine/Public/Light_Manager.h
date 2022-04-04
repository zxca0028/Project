#pragma once

#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager);

private:

	explicit CLight_Manager();
	virtual ~CLight_Manager() = default;

public:

	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9_DERIVED& LightDesc);
	HRESULT Render();

public:

	_matrix Get_LightTransform(D3DTRANSFORMSTATETYPE eStateType);

private:

	typedef vector<class CLight*> LIGHTS;
	LIGHTS m_Lights;

public:

	virtual void Free() override;

};

END