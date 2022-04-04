#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:

	enum WINMODE {MODE_FULL, MODE_WIN};

private:

	explicit CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:

	HRESULT Ready_Graphic_Device(HWND hWnd, WINMODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device = nullptr);
	void	Render_Begin(); 
	void	Render_End(HWND hWND = nullptr); 

private:

	LPDIRECT3D9		  m_pSDK    = nullptr;
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;

public:

	virtual void Free() override;

};

END