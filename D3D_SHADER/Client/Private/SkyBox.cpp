#include "pch.h"
#include "SkyBox.h"
#include "GameInstance.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CSkyBox::CSkyBox(const CSkyBox& rhs)
    : CGameObject(rhs)
{
}

HRESULT CSkyBox::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CSkyBox::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CSkyBox::Tick(_double DeltaTime)
{
    return _int();
}

_int CSkyBox::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);

    _matrix CamWorldMatrix = *D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);
    
    m_pTransform->Set_State(CTransform::STATE::STATE_POSITION, *(_float3*)&CamWorldMatrix.m[3][0]);
    
    if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_PRIORITY, this)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

HRESULT CSkyBox::Render(TYPE eType)
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

HRESULT CSkyBox::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_STATIC, L"Prototype_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Texture_SkyBox", L"Com_Texture", (CComponent**)&m_pTexture)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_VIBuffer_Cube", L"Com_VIBuffer_Cube", (CComponent**)&m_pVIBuffer)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Shader_SkyBox", L"Com_Shader_SkyBox", (CComponent**)&m_pShader)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CSkyBox::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix  = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrix  = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix,  sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix,  sizeof(_matrix));

    m_pShader->Set_Texture("g_MtrlDiffuse", m_pTexture);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSkyBox* pInstance = new CSkyBox(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Create SkyBox");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkyBox::Clone(void* pArg)
{
    CSkyBox* pInstance = new CSkyBox(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create SkyBox");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkyBox::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}
