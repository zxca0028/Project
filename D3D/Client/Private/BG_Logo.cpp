#include "pch.h"
#include "BG_Logo.h"
#include "GameInstance.h"

CBG_Logo::CBG_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CBG_Logo::CBG_Logo(const CBG_Logo& rhs)
    : CGameObject(rhs)
{}

HRESULT CBG_Logo::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CBG_Logo::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CBG_Logo::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    return _int();
}

_int CBG_Logo::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

    return _int();
}

HRESULT CBG_Logo::Render()
{
    __super::Render();

    _matrix IdentityMatrix, ScaleMatrix;

    D3DXMatrixScaling (&ScaleMatrix, 2.f, 2.f, 1.f);
    D3DXMatrixIdentity(&IdentityMatrix);

    m_pShaderCom->Set_ConstTable("g_WorldMatrix", &ScaleMatrix, sizeof(_matrix));
    m_pShaderCom->Set_ConstTable("g_ViewMatrix", &IdentityMatrix, sizeof(_matrix));
    m_pShaderCom->Set_ConstTable("g_ProjMatrix", &IdentityMatrix, sizeof(_matrix));

    if (FAILED(m_pShaderCom->Begin_Shader(0)))
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

HRESULT CBG_Logo::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
    {
        return E_FAIL;
    }

    CTransform::TRANSFORMDESC TransformDesc;
    TransformDesc.fSpeedPerSec    = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Texture_Logo"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"), TEXT("Com_Shader_Rect"), (CComponent**)&m_pShaderCom)))
    {
        return E_FAIL;
    }

    return S_OK;
}
CBG_Logo* CBG_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBG_Logo* pInstance = new CBG_Logo(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating BackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBG_Logo::Clone(void* pArg)
{
    CBG_Logo* pInstance = new CBG_Logo(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating BackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBG_Logo::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
}


