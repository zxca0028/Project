#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
END	

BEGIN(Client)

class CMainApp final : public CBase
{
private:

	explicit CMainApp();
	virtual ~CMainApp() = default;

public:

	HRESULT NativeConstruct();
	_int    Tick	 (_double DeltaTime);
	_int    Late_Tick(_double DeltaTime);
	HRESULT Render_MainApp();

private:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

private:

	CRenderer* m_pRenderer = nullptr;

private:

	HRESULT Add_Component_Prototype();

public:

	static  CMainApp* Create();
	virtual void Free() override;

};

END