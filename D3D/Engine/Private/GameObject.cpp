#include "GameObject.h"
#include "Component.h"
#include "GameInstance.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device(pGraphic_Device)
{
    Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject& rhs)
    :m_pGraphic_Device(rhs.m_pGraphic_Device)
{
    Safe_AddRef(m_pGraphic_Device);
}


HRESULT CGameObject::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CGameObject::NativeConstruct(void* pArg)
{
    return S_OK;
}

_int CGameObject::Tick(_double DeltaTime)
{
    return _int();
}

_int CGameObject::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CGameObject::Render()
{
    return S_OK;
}

void CGameObject::Free()
{
    for (auto& Pair : m_Components)
    {
        Safe_Release(Pair.second);
    }

    m_Components.clear();

    Safe_Release(m_pGraphic_Device);
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg)
{
    if (nullptr != Find_Component(pComponentTag))
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CComponent* pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);

    m_Components.emplace(pComponentTag, pComponent);

    Safe_AddRef(pComponent);

    *ppOut = pComponent;

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CComponent* CGameObject::Find_Component(const _tchar* pComponentTag)
{
    auto iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

    if (iter == m_Components.end())
    {
        return nullptr;
    }

    return iter->second;
}