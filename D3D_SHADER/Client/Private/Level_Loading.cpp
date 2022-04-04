#include "pch.h"
#include "Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Level_Main.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
    m_eNextLevel = eNextLevel;

    m_pLoader = CLoader::Create(m_pGraphic_Device, m_eNextLevel);

    if (nullptr == m_pLoader)
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CLevel_Loading::Tick(_double DeltaTime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (true == m_pLoader->IsFinished())
    {
        if (pGameInstance->Key_Down(KEY_ENTER))
        {
            CLevel* pLevel = nullptr;

            switch (m_eNextLevel)
            {
            case LEVEL::LEVEL_MAIN:

                pLevel = CLevel_Main::Create(m_pGraphic_Device);

                break;
            }

            pGameInstance->Clear((_uint)LEVEL::LEVEL_LOADING);
            pGameInstance->Set_Level(pLevel);
        }
    }

	RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CLevel_Loading::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CLevel_Loading::Render()
{
    _tchar szCount[1000] = TEXT("");
    _tchar szTitle[30] = TEXT("LEVEL_LOADING ");
    static int iCount = 0;

    swprintf_s(szCount, TEXT("%d"), iCount);

    ++iCount;

    lstrcat(szTitle, szCount);

    if (true == m_pLoader->IsFinished())
    {
        SetWindowText(g_hWnd, TEXT("LEVEL_LOADING(Loading Complete)"));
    }
    else
    {
        SetWindowText(g_hWnd, szTitle);
    }

    return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
    CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(eNextLevel)))
    {
        MSGBOX("Failed to Create Level_Loading");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Loading::Free()
{
    __super::Free();

    Safe_Release(m_pLoader);
}
