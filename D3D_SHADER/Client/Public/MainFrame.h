#pragma once

#include "Base.h"

BEGIN(Engine)
class CPicking;
class CGameInstance;
END

BEGIN(Client)

class CMainFrame final : public CBase
{
	DECLARE_SINGLETON(CMainFrame)

private:

	explicit CMainFrame(/*LPDIRECT3DDEVICE9 pGraphic_Device*/);
	virtual ~CMainFrame() = default;

public:

	HRESULT NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device);
	_int    Tick();
	HRESULT Render();
	void    Release_MainFrame();

public:

	HRESULT Begin_ImGui();

private:

	HRESULT MainFrame();
	HRESULT Viewport();
	HRESULT RenderTarget();

	HRESULT Navigator();

public:

	bool IsViewportFocused() { return m_bIsViewportFocused; }

private:

	bool m_bIsViewportFocused;

	bool m_bViewport    = true;
	bool m_bDiffuse     = false;
	bool m_bNormal      = false;
	bool m_bShade       = false;
	bool m_bSpecular    = false;
	bool m_bDepth       = false;
	bool m_bOutline		= false;
	bool m_bShadowDepth = false;

private:

	_float2 m_vTextureSize;
	_float2 m_vViewportSize;
	_float2 Screen_MousePos;
	_float2 Viewport_LT_Pos;
	_float2 Viewport_MousePos;
	_float2 Fixed_MousePos;

private:

	LPDIRECT3DTEXTURE9 m_pViewportTexture = nullptr;

private:

	unordered_map<string, TARGETDESC> m_TargetDesc;

private:

	CPicking*		  m_pPicking = nullptr;
	CGameInstance*    m_pGameInstance   = nullptr;
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:

	virtual void Free() override;

};

END