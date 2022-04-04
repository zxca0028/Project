#include "Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

HRESULT CLight_Manager::Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9_DERIVED& LightDesc)
{
    CLight* pLight = CLight::Create(pGraphic_Device, LightDesc);

    if (nullptr == pLight)
    {
        return E_FAIL;
    }

    m_Lights.emplace_back(pLight);

    return S_OK;
}

HRESULT CLight_Manager::Render()
{
    for (auto& pLight : m_Lights)
    {
        pLight->Render();
    }

    return S_OK;
}

_matrix CLight_Manager::Get_LightTransform(D3DTRANSFORMSTATETYPE eStateType)
{
    if (m_Lights.size() == 0)
    {
        _matrix IdentityMatrix;
        D3DXMatrixIdentity(&IdentityMatrix);

        return IdentityMatrix;
    }

    auto iter = m_Lights.begin();

    return (*iter)->Get_LightTransform(eStateType);
}

void CLight_Manager::Free()
{
    for (auto& pLight : m_Lights)
    {
        Safe_Release(pLight);
    }

    m_Lights.clear();
}
