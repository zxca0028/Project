#include "pch.h"
#include "Fade.h"
#include "GameInstance.h"
#include "Collision_Manager.h"

CFade::CFade(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CFade::CFade(const CFade& rhs)
    : CGameObject(rhs)
{}

HRESULT CFade::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CFade::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CFade::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    m_fFadeIn = m_fFadeIn > 1.f ? 1.f : m_fFadeIn += DeltaTime / 2.f;

    CCollision_Manager* pCollision_Manager = GET_INSTANCE(CCollision_Manager);
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (m_fFadeIn >= 1.f)
    {
        pCollision_Manager->FadeIn_Complete();
        m_bisDelete = true;

        pGameInstance->Add_DeleteContainer(LEVEL_GAMEPLAY, TEXT("Layer_Fade"), this);
    }

    RELEASE_INSTANCE(CCollision_Manager);
    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CFade::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

    return _int();
}

HRESULT CFade::Render()
{
    __super::Render();

    _matrix IdentityMatrix, ScaleMatrix;

    D3DXMatrixScaling(&ScaleMatrix, 2.f, 2.f, 1.f);
    D3DXMatrixIdentity(&IdentityMatrix);

    m_pShaderCom->Set_ConstTable("g_WorldMatrix", &ScaleMatrix,    sizeof(_matrix));
    m_pShaderCom->Set_ConstTable("g_ViewMatrix",  &IdentityMatrix, sizeof(_matrix));
    m_pShaderCom->Set_ConstTable("g_ProjMatrix",  &IdentityMatrix, sizeof(_matrix));

    m_pShaderCom->Set_ConstTable("g_fAlpha", &m_fFadeIn, sizeof(_float));

    m_pShaderCom->Begin_Shader(2);

    m_pVIBufferCom->Render_VIBuffer();

    m_pShaderCom->End_Shader();


    return S_OK;
}

HRESULT CFade::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))    {        return E_FAIL;    }

    CTransform::TRANSFORMDESC TransformDesc;
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"),     TEXT("Com_Transform"),   (CComponent**)&m_pTransformCom, &TransformDesc))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"),    (CComponent**)&m_pVIBufferCom))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Texture_Logo"),  TEXT("Com_Texture"),     (CComponent**)&m_pTextureCom)))  { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"),   TEXT("Com_Shader_Rect"), (CComponent**)&m_pShaderCom)))   { return E_FAIL; }

    return S_OK;
}
CFade* CFade::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFade* pInstance = new CFade(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating Fade");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFade::Clone(void* pArg)
{
    CFade* pInstance = new CFade(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating Fade");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFade::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
}


