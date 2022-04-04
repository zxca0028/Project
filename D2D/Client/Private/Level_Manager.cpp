#include "pch.h"
#include "Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(CLevel_Manager);

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::Set_Level(CLevel* pLevel)
{
    if (nullptr == pLevel)
    {
        return E_FAIL;
    }

    if (nullptr != m_pCurrentLevel)
    {
        Safe_Release(m_pCurrentLevel);
    }

    m_pCurrentLevel = pLevel;

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    pGameInstance->OnNotify(pLevel->CurrentLevelIndex());

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

_int CLevel_Manager::Tick(_double DeltaTime)
{
    if (nullptr == m_pCurrentLevel)
    {
        return -1;
    }

    return m_pCurrentLevel->Tick(DeltaTime);
}

_int CLevel_Manager::Late_Tick(_double DeltaTime)
{
    if (nullptr == m_pCurrentLevel)
    {
        return -1;
    }

    return m_pCurrentLevel->Late_Tick(DeltaTime);
}

HRESULT CLevel_Manager::Render()
{
    if (nullptr == m_pCurrentLevel)
    {
        return -1;
    }

    return m_pCurrentLevel->Render();
}

void CLevel_Manager::OnNotify(_bool bIsPlayerAlive)
{
    if (nullptr != m_pCurrentLevel)
    {
        m_pCurrentLevel->OnNotify(bIsPlayerAlive);
    }
}

void CLevel_Manager::Free()
{
    Safe_Release(m_pCurrentLevel);
}

END