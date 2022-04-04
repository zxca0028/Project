#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "GameInstance.h"

BEGIN(Client)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device(pGraphic_Device)
{
    Safe_AddRef(m_pGraphic_Device);
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

HRESULT CGameObject::Add_Components(void* pArg)
{
    return S_OK;
}

HRESULT CGameObject::Set_ConstTable()
{
    return S_OK;
}

HRESULT CGameObject::Add_Component(wstring ComponentTag, CComponent* pComponent)
{
    if (nullptr == pComponent)
    {
        return E_FAIL;
    }

    if (nullptr != Find_Component(ComponentTag))
    {
        return E_FAIL;
    }

    m_Components.emplace(ComponentTag, pComponent);

    Safe_AddRef(pComponent);

    return S_OK;
}

CComponent* CGameObject::Find_Component(wstring ComponentTag)
{
    auto iter = m_Components.find(ComponentTag);

    if (iter == m_Components.end())
    {
        return nullptr;
    }

    return iter->second;
}

HRESULT CGameObject::Add_DeleteQueue(wstring LayerTag, CGameObject* pGameObject)
{
    m_bIsValid = false;

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    pGameInstance->Add_DeleteQueue(LayerTag, pGameObject);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

void CGameObject::Free()
{
    Safe_Release(m_pGraphic_Device);

    for (auto& Pair : m_Components)
    {
        Safe_Release(Pair.second);
    }
    
    m_Components.clear();
}

END