#include "pch.h"
#include "Level_Loading.h"
#include "GameInstance.h"
#include "Loader.h"
#include "Level_GamePlay.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CLevel(pGraphic_Device)
{}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
    __super::NativeConstruct();

    m_eNextLevel = eNextLevel;

    m_pLoader = CLoader::Create(m_pGraphic_Device, m_eNextLevel);

    if (nullptr == m_pLoader)
    {
        return E_FAIL;
    }

    if (FAILED(Ready_Layer_BG_Logo(TEXT("Layer_BG_Logo"))))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CLevel_Loading::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    if (true == m_pLoader->Get_Finisied())
    {
        if (GetAsyncKeyState(VK_RETURN) & 0x0001)
        {
            CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

            CLevel* pLevel = nullptr;

            switch (m_eNextLevel)
            {
            case LEVEL_GAMEPLAY:
                pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
                break;
            }

            pGameInstance->Clear(LEVEL_LOADING);
            pGameInstance->Set_Level(pLevel);

            RELEASE_INSTANCE(CGameInstance);
        }
    }    

    return _int();
}

HRESULT CLevel_Loading::Render()
{
    __super::Render();

    _tchar szCount[1000] = TEXT("");
    _tchar szTitle[30] = TEXT("LEVEL_LOADING ");
    static int iCount = 0;

    swprintf_s(szCount, TEXT("%d"), iCount);

    ++iCount;

    lstrcat(szTitle, szCount);

    if (true == m_pLoader->Get_Finisied())
    {
        SetWindowText(g_hWnd, TEXT("LEVEL_LOADING(Loading Complete)"));
    }
    else
    {
        SetWindowText(g_hWnd, szTitle);
    }

    return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BG_Logo(const _tchar* pLayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, TEXT("Prototype_BG_Loading"), pLayerTag)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
    CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(eNextLevel)))
    {
        MSGBOX("Failed to Creating CLevel_Loading");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CLevel_Loading::Free()
{
    __super::Free();

    Safe_Release(m_pLoader);
}
