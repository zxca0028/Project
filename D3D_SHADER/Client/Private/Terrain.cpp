#include "pch.h"
#include "Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain& rhs)
    : CGameObject(rhs)
{
}

HRESULT CTerrain::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CTerrain::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }
    
    m_pTransform->Set_State(CTransform::STATE::STATE_POSITION, _float3(-8.f, 0.f, -8.f));

    return S_OK;
}

_int CTerrain::Tick(_double DeltaTime)
{
    return _int();
}

_int CTerrain::Late_Tick(_double DeltaTime)
{
    if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this)))
    {
        return E_FAIL;
    }

    return _int();
}

HRESULT CTerrain::Render(TYPE eType)
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    m_pShader->Begin_Shader(0);

    m_pVIBuffer->Render_VIBuffer();

    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CTerrain::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_STATIC, L"Prototype_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Texture_Terrain_D", L"Com_Texture_D", (CComponent**)&m_pTexture_D)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Texture_Terrain_N", L"Com_Texture_N", (CComponent**)&m_pTexture_N)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_VIBuffer_Terrain", L"Com_VIBuffer_Terrain", (CComponent**)&m_pVIBuffer)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Shader_Terrain", L"Com_Shader_Terrain", (CComponent**)&m_pShader)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CTerrain::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix,  sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix,  sizeof(_matrix));

    m_pShader->Set_Texture("g_MtrlDiffuse", m_pTexture_D);
    m_pShader->Set_Texture("g_MtrlNormal",  m_pTexture_N);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTerrain* pInstance = new CTerrain(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Create Terrain");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
    CTerrain* pInstance = new CTerrain(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Terrain");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTerrain::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture_D);
    Safe_Release(m_pTexture_N);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}
