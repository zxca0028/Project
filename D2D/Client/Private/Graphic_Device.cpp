#include "pch.h"
#include "Graphic_Device.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
{

}

HRESULT CGraphic_Device::NativeConstruct(HWND hWnd, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device)
{
    D3DCAPS9 d3dcaps;
    ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

    LPDIRECT3D9 pSDK = Direct3DCreate9(D3D_SDK_VERSION);

    DWORD vp = 0;

    if (FAILED(pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
    {
        goto ERR;
    }

    if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
    }
    else
    {
        vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

    d3dpp.BackBufferWidth            = iWinCX;
    d3dpp.BackBufferHeight           = iWinCY;
    d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount            = 1;
                                     
    d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality         = 0;
                                     
    d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow              = hWnd;
    d3dpp.Windowed                   = 1;
    d3dpp.EnableAutoDepthStencil     = TRUE;
    d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;

    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_DEFAULT;
    d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

    if (FAILED(pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pGraphic_Device)))
    {
        goto ERR;
    }

    if (nullptr != ppGraphic_Device)
    {
        *ppGraphic_Device = m_pGraphic_Device;

        Safe_AddRef(m_pGraphic_Device);
    }
    
    pSDK->Release();

    return S_OK;

ERR:

    MSGBOX("Failed to Create Graphic_Device");

    return E_FAIL;

}

void CGraphic_Device::Render_Begin()
{
    m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0, 0, 255, 255), 1.f, 0);
    m_pGraphic_Device->BeginScene();
}

void CGraphic_Device::Render_End(HWND hWnd)
{
	m_pGraphic_Device->EndScene();
	m_pGraphic_Device->Present(nullptr, nullptr, hWnd, nullptr);
}

void CGraphic_Device::Free()
{
    if (0 != Safe_Release(m_pGraphic_Device))
    {
        MSGBOX("Failed to Release D3DXDevice");
    }
}

END