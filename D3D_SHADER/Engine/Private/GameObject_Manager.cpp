#include "GameObject_Manager.h"
#include "Layer.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)

CGameObject_Manager::CGameObject_Manager()
{
}

HRESULT CGameObject_Manager::NativeConstruct(_uint iNumLevel)
{
    if (nullptr != m_pLayers)
    {
        return E_FAIL;
    }

    m_pLayers = new LAYERS[iNumLevel];

    m_iNumLevel = iNumLevel;

    return S_OK;
}

HRESULT CGameObject_Manager::Add_GameObject_Prototype(wstring pPrototypeTag, CGameObject* pPrototype)
{
    if (nullptr == pPrototype || nullptr != Find_Prototype(pPrototypeTag))
    {
        return E_FAIL;
    }

    m_Prototypes.emplace(pPrototypeTag, pPrototype);

    return S_OK;
}

HRESULT CGameObject_Manager::Add_GameObject(_uint iLevelIndex, wstring pPrototypeTag, wstring pLayerTag, void* pArg)
{
    if (m_iNumLevel <= iLevelIndex)
    {
        return E_FAIL;
    }

    CGameObject* pPrototype = Find_Prototype(pPrototypeTag);

    if (nullptr == pPrototype)
    {
        return E_FAIL;
    }

    CGameObject* pGameObject = pPrototype->Clone(pArg);

    if (nullptr == pGameObject)
    {
        return E_FAIL;
    }

    CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

    if (nullptr == pLayer)
    {
        pLayer = CLayer::Create();

        if (nullptr == pLayer)
        {
            return E_FAIL;
        }

        pLayer->Add_GameObject(pGameObject);

        m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
    }
    else
    {
        pLayer->Add_GameObject(pGameObject);
    }

    return S_OK;
}

_int CGameObject_Manager::Tick(_double DeltaTime)
{
    for (_uint i = 0; i < m_iNumLevel; ++i)
    {
        for (auto& Pair : m_pLayers[i])
        {
            Pair.second->Tick(DeltaTime);
        }
    }

    return _int();
}

_int CGameObject_Manager::Late_Tick(_double DeltaTime)
{
    for (_uint i = 0; i < m_iNumLevel; ++i)
    {
        for (auto& Pair : m_pLayers[i])
        {
            Pair.second->Late_Tick(DeltaTime);
        }
    }

    return _int();
}

void CGameObject_Manager::Clear(_uint iLevelIndex)
{
    for (auto& Pair : m_pLayers[iLevelIndex])
    {
        Safe_Release(Pair.second);
    }

    m_pLayers[iLevelIndex].clear();
}

CComponent* CGameObject_Manager::Get_ComponentPtr(_uint iLevelIndex, wstring pLayerTag, wstring pComponentTag, _uint iIndex)
{
    CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

    if (nullptr == pLayer)
    {
        return nullptr;
    }

    return pLayer->Get_ComponentPtr(pComponentTag, iIndex);
}

CGameObject* CGameObject_Manager::Find_Prototype(wstring pPrototypeTag)
{
    if (m_Prototypes.end() == m_Prototypes.find(pPrototypeTag))
    {
        return nullptr;
    }

    return m_Prototypes.find(pPrototypeTag)->second;
}

CLayer* CGameObject_Manager::Find_Layer(_uint iLevelIndex, wstring pLayerTag)
{
    if (m_pLayers[iLevelIndex].end() == m_pLayers[iLevelIndex].find(pLayerTag))
    {
        return nullptr;
    }

    return m_pLayers[iLevelIndex].find(pLayerTag)->second;
}

void CGameObject_Manager::Free()
{
    for (_uint i = 0; i < m_iNumLevel; ++i)
    {
        for (auto& Pair : m_pLayers[i])
        {
            Safe_Release(Pair.second);
        }

        m_pLayers[i].clear();
    }

    Safe_Delete_Array(m_pLayers);

    for (auto& Pair : m_Prototypes)
    {
        Safe_Release(Pair.second);
    }

    m_Prototypes.clear();
}
