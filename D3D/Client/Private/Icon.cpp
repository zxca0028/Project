#include "pch.h"
#include "Icon.h"
#include "GameInstance.h"
#include "Collision_Manager.h"
#include "Skeleton.h"

CIcon::CIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CIcon::CIcon(const CIcon& rhs)
    : CGameObject(rhs)
{}

HRESULT CIcon::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CIcon::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }
    
    memcpy(&m_IconInfo, pArg, sizeof(ICONINFO));

    m_fObjectMaxHP = *(m_IconInfo.fObjectHP);

    return S_OK;
}

_int CIcon::Tick(_double TimeDelta)
{
    __super::Tick(TimeDelta);

    _float3 vObjectPos = *(_float3*)(m_IconInfo.WorldMatrix)->m[3];

    vObjectPos.y = 12.f;

    m_pTransform->Set_State(CTransform::STATE_POSITION, vObjectPos);

    if (*(m_IconInfo.fObjectHP) <= 0.f)
    {
        m_bisDelete = true;

        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

        pGameInstance->Add_DeleteContainer(LEVEL_GAMEPLAY, TEXT("Layer_HPGauge"), this);

        RELEASE_INSTANCE(CGameInstance);
    }

    return _int();
}

_int CIcon::Late_Tick(_double TimeDelta)
{
    __super::Late_Tick(TimeDelta);

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    
    _matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
    
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
    
    m_pTransform->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
    m_pTransform->Set_State(CTransform::STATE_UP,    *(_float3*)&ViewMatrix.m[1][0]);
    m_pTransform->Set_State(CTransform::STATE_LOOK,  *(_float3*)&ViewMatrix.m[2][0]);
    
    m_pTransform->Set_Scale(_float3(24.5f/15.f, 0.1f, 0.1f));

    RELEASE_INSTANCE(CGameInstance);

    if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
    {
        return -1;
    }

    return _int();
}

HRESULT CIcon::Render()
{
    if (true == *(m_IconInfo.bRender))
    {
        if (FAILED(Set_ConstTable()))
        {
            return E_FAIL;
        }
    }

    return S_OK;
}

HRESULT CIcon::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix PorjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &PorjMatrix, sizeof(_matrix));    

    _float fHPGauge = (*(m_IconInfo.fObjectHP) - (m_fObjectMaxHP /2.f))/ m_fObjectMaxHP;

    m_pShader->Set_ConstTable("g_fHPDelta", &fHPGauge, sizeof(_float));

    m_pShader->Set_TextureConstTable("g_FrameDiffuse", m_pTexture, 1);
    m_pShader->Set_TextureConstTable("g_HPDiffuse", m_pTexture, 3);

    m_pShader->Begin_Shader(2);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CIcon::Add_Components()
{
    CTransform::TRANSFORMDESC TransformDesc;
    TransformDesc.fSpeedPerSec = 0.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(0.f);

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShader)))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_HPGauge"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
    {
        return E_FAIL;
    }

    return S_OK;
}

CIcon* CIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CIcon* pInstance = new CIcon(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CIcon::Clone(void* pArg)
{
    CIcon* pInstance = new CIcon(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIcon::Free()
{
    CGameObject::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}
