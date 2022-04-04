#include "pch.h"
#include "Level_Start.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Start::CLevel_Start(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Start::NativeConstruct()
{
    return S_OK;
}

_int CLevel_Start::Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    
    if (pGameInstance->Key_Down(KEY_ENTER))
    {
        if (FAILED(pGameInstance->Set_Level(CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_MAIN))))
        {
            RELEASE_INSTANCE(CGameInstance);

            return E_FAIL;
        }
        else
        {
            pGameInstance->Clear((_uint)LEVEL::LEVEL_START);
        }
    }

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CLevel_Start::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CLevel_Start::Render()
{
    _tchar szText[MAX_PATH] = TEXT("LEVEL_START");

    SetWindowText(g_hWnd, szText);

    return S_OK;
}

CLevel_Start* CLevel_Start::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CLevel_Start* pInstance = new CLevel_Start(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct()))
    {
        MSGBOX("Failed to Create Level_Main");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Start::Free()
{
    __super::Free();
}
