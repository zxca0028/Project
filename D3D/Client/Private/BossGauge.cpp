#include "pch.h"
#include "BossGauge.h"
#include "GameInstance.h"
#include "Collision_Manager.h"

CBossGauge::CBossGauge(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CBossGauge::CBossGauge(const CBossGauge& rhs)
    : CGameObject(rhs)
{}

HRESULT CBossGauge::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CBossGauge::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    m_fSizeX = 853;
    m_fSizeY = 155;
    m_fX = (g_iWinCX / 2);
    m_fY = (g_iWinCY / 2) - 275;

    D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

    D3DXMatrixIdentity(&m_WorldMatrix);
    D3DXMatrixIdentity(&m_ViewMatrix);

    memcpy(&m_fBossHP, pArg, sizeof(_float*));

    m_fMaxHP = *m_fBossHP;

    return S_OK;
}

_int CBossGauge::Tick(_double TimeDelta)
{
    __super::Tick(TimeDelta);

    return _int();
}

_int CBossGauge::Late_Tick(_double TimeDelta)
{
    __super::Late_Tick(TimeDelta);

    CCollision_Manager* pCollision_Manager = GET_INSTANCE(CCollision_Manager);

    if (false == pCollision_Manager->Release_Controller())
    {
        if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
        {
            return -1;
        }
    }

    RELEASE_INSTANCE(CCollision_Manager);

    return _int();
}

HRESULT CBossGauge::Render()
{
	if (FAILED(Set_ConstTable())) { return E_FAIL; }



    return S_OK;
}

HRESULT CBossGauge::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    /* BackGround */
    m_WorldMatrix._11 = m_fSizeX;
    m_WorldMatrix._22 = m_fSizeY;

    m_WorldMatrix._41 =  m_fX - (g_iWinCX / 2);
    m_WorldMatrix._42 = -m_fY + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &m_ProjMatrix, sizeof(_matrix));

    int iBG = 0;
    m_pShader->Set_ConstTable("g_ToF", &iBG, sizeof(_int));

    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 1);

    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    /* Gauge */
    m_WorldMatrix._11 = 680;
    m_WorldMatrix._22 = 16;

    m_WorldMatrix._41 = ((g_iWinCX / 2) + 32) - (g_iWinCX / 2);
    m_WorldMatrix._42 = -1.f * ((g_iWinCY / 2) -276) + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    int iGauge = 1;
    m_pShader->Set_ConstTable("g_ToF", &iGauge, sizeof(_int));

    _float fHP = (*m_fBossHP - (m_fMaxHP / 2.f)) / m_fMaxHP;
    m_pShader->Set_ConstTable("g_fHPDelta", &fHP, sizeof(_float));

    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 2);

    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    /* Frame */
    m_WorldMatrix._11 = m_fSizeX;
    m_WorldMatrix._22 = m_fSizeY;

    m_WorldMatrix._41 =  m_fX - (g_iWinCX / 2);
    m_WorldMatrix._42 = -m_fY + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    int iFrame = 0;
    m_pShader->Set_ConstTable("g_ToF", &iFrame, sizeof(_int));
    fHP = (*m_fBossHP - (m_fMaxHP / 2.f)) / m_fMaxHP;
    m_pShader->Set_ConstTable("g_fHPDelta", &fHP, sizeof(_float));

    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 0);

    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    /* Effect */

    m_WorldMatrix._11 = 680;
    m_WorldMatrix._22 = 16;
    
    m_WorldMatrix._41 = ((g_iWinCX / 2) + 32) - (g_iWinCX / 2);
    m_WorldMatrix._42 = -1.f * ((g_iWinCY / 2) - 276) + (g_iWinCY / 2);
    
    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    int iEffect = 1;
    m_pShader->Set_ConstTable("g_ToF", &iEffect, sizeof(_int));
    
    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 3);
    
    m_pShader->Begin_Shader(1);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CBossGauge::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Shader_BossGauge"), TEXT("Com_Shader"), (CComponent**)&m_pShader)))
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

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_BossGauge"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
    {
        return E_FAIL;
    }

    return S_OK;
}

CBossGauge* CBossGauge::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBossGauge* pInstance = new CBossGauge(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBossGauge::Clone(void* pArg)
{
    CBossGauge* pInstance = new CBossGauge(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBossGauge::Free()
{
    CGameObject::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pVIBuffer);
}
