#include "pch.h"
#include "Layer.h"

BEGIN(Client)

CLayer::CLayer()
{
}

HRESULT CLayer::NativeConstruct()
{
    return S_OK;
}

_int CLayer::Tick(_double DeltaTime)
{
    for (auto& pGameObject : m_GameObjects)
    {
        if (true == pGameObject->IsValid())
        {
            pGameObject->Tick(DeltaTime);
        }
    }

    return _int();
}

_int CLayer::Late_Tick(_double DeltaTime)
{
    for (auto& pGameObject : m_GameObjects)
    {
        if (true == pGameObject->IsValid())
        {
            pGameObject->Late_Tick(DeltaTime);
        }
    }

    return _int();
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
    m_GameObjects.emplace_back(pGameObject);

    return S_OK;
}

HRESULT CLayer::Add_DeleteQueue(CGameObject* pGameObject)
{
    m_InvalidGameObjects.emplace(pGameObject);

    return S_OK;
}

HRESULT CLayer::Delete_GameObject()
{
    if (m_iTickCount > 3)
    {
        if (0 != m_InvalidGameObjects.size())
        {
            m_GameObjects.remove(m_InvalidGameObjects.front());

            Safe_Release(m_InvalidGameObjects.front());

            m_InvalidGameObjects.pop();
        }

        m_iTickCount = 0;
    }
    else
    {
        ++m_iTickCount;
    }

    return S_OK;
}

CComponent* CLayer::Get_ComponentPtr(wstring ComponentTag, _uint iIndex)
{
    if (iIndex >= m_GameObjects.size())
    {
        return nullptr;
    }

    auto iter = m_GameObjects.begin();

    for (_uint i = 0; i < iIndex; ++i)
    {
        ++iter;
    }

    return (*iter)->Find_Component(ComponentTag);
}

void CLayer::OnNotify(_bool bIsGameOver)
{
    for (auto& pGameObject : m_GameObjects)
    {
        if (true == pGameObject->IsValid())
        {
            pGameObject->OnNotify(bIsGameOver);
        }
    }
}

void CLayer::OnNotify(LEVEL eLevel)
{
    for (auto& pGameObject : m_GameObjects)
    {
        if (true == pGameObject->IsValid())
        {
            pGameObject->OnNotify(eLevel);
        }
    }
}

CLayer* CLayer::Create()
{
    CLayer* pInstance = new CLayer;

    if (FAILED(pInstance->NativeConstruct()))
    {
        MSGBOX("Failed to Create Layer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLayer::Free()
{
    for (auto& pGameObject : m_GameObjects)
    {
        Safe_Release(pGameObject);
    }

    m_GameObjects.clear();

    while (0 != m_InvalidGameObjects.size())
    {
        Safe_Release(m_InvalidGameObjects.front());

        m_InvalidGameObjects.pop();
    }
}

END