#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CFrustum;
class CPicking;
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
	_int	Tick(_double DeltaTime);
	_int	Late_Tick(_double DeltaTime);
	HRESULT Render_MainApp();

public:

	HRESULT Init_ImGui();
	HRESULT Update_ImGui();
	HRESULT Render_ImGui();
	void    Clear_ImGui();

private:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;
	CFrustum*		  m_pFrustum		= nullptr;
	CPicking*		  m_pPicking        = nullptr;
	CRenderer*		  m_pRenderer		= nullptr;

private:

	class CObjectTool* m_pObjectTool    = nullptr;

private:

	HRESULT Create_GameObject_Prototype();
	HRESULT Create_Component_Prototype ();
	HRESULT Create_NaviMeshPoint();

public:

	static  CMainApp* Create();
	virtual void	  Free();

};

END

