#include "pch.h"
#include "Component.h"

BEGIN(Client)

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device(pGraphic_Device)
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CComponent::NativeConstruct()
{
    return S_OK;
}

_int CComponent::Tick(_double DeltaTime)
{
    return _int();
}

_int CComponent::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CComponent::Render()
{
    return S_OK;
}

void CComponent::Free()
{
    Safe_Release(m_pGraphic_Device);
}

END