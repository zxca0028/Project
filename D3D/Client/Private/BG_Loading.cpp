#include "pch.h"
#include "BG_Loading.h"
#include "GameInstance.h"

CBG_Loading::CBG_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CBG_Loading::CBG_Loading(const CBG_Loading& rhs)
    : CGameObject(rhs)
{}

HRESULT CBG_Loading::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CBG_Loading::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Component()))
    {
        return E_FAIL;
    }

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, -1.f));

    return S_OK;
}

_int CBG_Loading::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    return _int();
}

_int CBG_Loading::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

    return _int();
}

HRESULT CBG_Loading::Render()
{
    __super::Render();

    _matrix IdentityMatrix, ScaleMatrix;

    D3DXMatrixScaling(&ScaleMatrix, 2.f, 2.f, 1.f);
    D3DXMatrixIdentity(&IdentityMatrix);

    m_pShaderCom->Set_ConstTable("g_WorldMatrix", &ScaleMatrix, sizeof(_matrix));
    m_pShaderCom->Set_ConstTable("g_ViewdMatrix", &IdentityMatrix, sizeof(_matrix));
    m_pShaderCom->Set_ConstTable("g_ProjdMatrix", &IdentityMatrix, sizeof(_matrix));
    
    m_pShaderCom->Set_TextureConstTable("g_MtrlDiffuse", m_pTextureCom);

    if (FAILED(m_pShaderCom->Begin_Shader(1)))
    {
        return E_FAIL;
    }
    if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
    {
        return E_FAIL;
    }
    if (FAILED(m_pShaderCom->End_Shader()))
    {
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CBG_Loading::Add_Component()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
    {
        return E_FAIL;
    }

    CTransform::TRANSFORMDESC TransformDesc;
    TransformDesc.fSpeedPerSec = 3.f;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Texture_Loading"), TEXT("Com_Texture_Loading"), (CComponent**)&m_pTextureCom)))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"), TEXT("Com_Shader_Rect"), (CComponent**)&m_pShaderCom)))
    {
        return E_FAIL;
    }

    return S_OK;
}

CBG_Loading* CBG_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBG_Loading* pInstance = new CBG_Loading(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating BackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBG_Loading::Clone(void* pArg)
{
    CBG_Loading* pInstance = new CBG_Loading(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating BackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBG_Loading::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
}
