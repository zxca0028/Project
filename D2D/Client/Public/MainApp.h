#pragma once

#include "Base.h"

BEGIN(Client)

class CMainApp final : public CBase
{
private:

	explicit CMainApp();
	virtual ~CMainApp() = default;

public:

	HRESULT NativeConstruct();
	_int    Tick     (_double DeltaTime);
	_int	Late_Tick(_double DeltaTime);
	HRESULT Render();

private:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

private:

	HRESULT Ready_Layer_BackGround(wstring LayerTag);

private:

	_uint m_iTickCount = 0;

public:

	static  CMainApp* Create();
	virtual void Free() override;

};

END