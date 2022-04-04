#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

private:

	explicit CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:

	HRESULT NativeConstruct(HWND hWnd, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device);
	void    Render_Begin();
	void    Render_End(HWND hWnd = nullptr);

private:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:

	virtual void Free() override;

};

END