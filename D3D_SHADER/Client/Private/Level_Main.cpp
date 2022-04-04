#include "pch.h"
#include "Level_Main.h"
#include "GameInstance.h"
#include "Camera.h"
#include "MainFrame.h"

CLevel_Main::CLevel_Main(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Main::NativeConstruct()
{
    if (FAILED(Ready_Light())) { return E_FAIL; }
	if (FAILED(Ready_Layer_Camera    (L"Layer_Camera")))     { return E_FAIL; }
	if (FAILED(Ready_Layer_SkyBox    (L"Layer_SkyBox")))     { return E_FAIL; }
    if (FAILED(Ready_Layer_Player    (L"Layer_Player")))     { return E_FAIL; }
    //if (FAILED(Ready_Layer_TestObject(L"Layer_TestObject"))) { return E_FAIL; }
	if (FAILED(Ready_Layer_Terrain   (L"Layer_Terrain")))    { return E_FAIL; }

    return S_OK;
}

_int CLevel_Main::Tick(_double DeltaTime)
{
    return _int();
}

_int CLevel_Main::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CLevel_Main::Render()
{
    _tchar szText[MAX_PATH] = TEXT("LEVEL_MAIN");

    SetWindowText(g_hWnd, szText);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Light()
{
    if (nullptr == m_pGraphic_Device)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    D3DLIGHT9_DERIVED LightDesc;
    ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

    LightDesc.Type      = D3DLIGHT_DIRECTIONAL;
    LightDesc.Diffuse   = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    LightDesc.Ambient   = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f);
    LightDesc.Specular  = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
    LightDesc.Direction = _float3(1.f, -1.f, 1.f);

    LightDesc.vLightEye = _float3(-3.f, 3.f, -3.f);
    LightDesc.vLightAt  = _float3(0.f, 0.f, 0.f);
    LightDesc.vLightUp  = _float3(0.f, 1.f, 0.f);

    //LightDesc.Type     = D3DLIGHT_POINT;
    //LightDesc.Diffuse  = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    //LightDesc.Ambient  = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    //LightDesc.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
    //LightDesc.Position = _float3(0.f, 3.f, 0.f);
    //LightDesc.Range    = 5.f;

    if (FAILED(pGameInstance->Add_Right(m_pGraphic_Device, LightDesc)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_Camera(wstring pLayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CCamera::CAMERADESC CameraDesc;

    CameraDesc.vLookAt = _float3(0.f, 0.f, 0.f);
    CameraDesc.vPos    = _float3(0.f, 1.f, -5.f);
	CameraDesc.fFovy   = D3DXToRadian(60.f);
    CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
    CameraDesc.fNear   = 0.25f;
    CameraDesc.fFar    = 300.f;

    if (FAILED(pGameInstance->Add_GameObject((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Camera_Free", pLayerTag, &CameraDesc)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_SkyBox(wstring pLayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_GameObject((_uint)LEVEL::LEVEL_MAIN, L"Prototype_SkyBox", pLayerTag)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_Terrain(wstring pLayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_GameObject((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Terrain", pLayerTag)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_Player(wstring pLayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_GameObject((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Player", pLayerTag)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_TestObject(wstring pLayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_GameObject((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Sphere", pLayerTag)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CLevel_Main* CLevel_Main::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CLevel_Main* pInstance = new CLevel_Main(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct()))
    {
        MSGBOX("Failed to Create Level_Main");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Main::Free()
{
    __super::Free();
}
