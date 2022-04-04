#include "pch.h"
#include "PlayerGauge.h"
#include "GameInstance.h"
#include "Collision_Manager.h"

CPlayerGauge::CPlayerGauge(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CPlayerGauge::CPlayerGauge(const CPlayerGauge& rhs)
    : CGameObject(rhs)
{}

HRESULT CPlayerGauge::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CPlayerGauge::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    memcpy(&m_pPlayerHP, pArg, sizeof(_float*));

    m_fPlayerMaxHP = *m_pPlayerHP;

    m_fSizeX = 292;
    m_fSizeY = 16;
    m_fX = (g_iWinCX / 2) - 200;
    m_fY = (g_iWinCY / 2) + 225;
    m_fXt = (g_iWinCX / 2) - 200;
    m_fYt = (g_iWinCY / 2) + 225;

    m_fSizeXt1 = 242;
    m_fSizeYt1 = 8;
    m_fXt1 = (g_iWinCX / 2) - 200;
    m_fYt1 = (g_iWinCY / 2) + 225;
    D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

    D3DXMatrixIdentity(&m_WorldMatrix);
    D3DXMatrixIdentity(&m_ViewMatrix);

    return S_OK;
}

_int CPlayerGauge::Tick(_double TimeDelta)
{
    __super::Tick(TimeDelta);

    /*static int X = 0;
    static int Y = 0;
    static int SX = 0;
    static int SY = 0;
    
    ImGui::Begin("Gauge");
    ImGui::InputInt("X", &X);
    ImGui::InputInt("Y", &Y);
    ImGui::InputInt("SX", &SX);
    ImGui::InputInt("SY", &SY);
    ImGui::End();
    
    
    m_fXt1 = (g_iWinCX / 2) + X;
    m_fYt1 = (g_iWinCY / 2) + Y;*/

    return _int();
}

_int CPlayerGauge::Late_Tick(_double TimeDelta)
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

HRESULT CPlayerGauge::Render()
{
    if (FAILED(Set_ConstTable())) { return E_FAIL; }



    return S_OK;
}

HRESULT CPlayerGauge::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    /* BackGround_HP */
    m_WorldMatrix._11 = m_fSizeX;
    m_WorldMatrix._22 = m_fSizeY;

    m_WorldMatrix._41 =  m_fX - (g_iWinCX / 2);
    m_WorldMatrix._42 = -m_fY + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    int iBG = 0;
    m_pShader->Set_ConstTable("g_ToF", &iBG, sizeof(_int));

    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 0);

    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    /* BackGround_MP */
    m_WorldMatrix._11 = m_fSizeX;
    m_WorldMatrix._22 = m_fSizeY;

    m_WorldMatrix._41 = m_fX - (g_iWinCX / 2) + 430;
    m_WorldMatrix._42 = -m_fY + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    m_pShader->Set_ConstTable("g_ToF", &iBG, sizeof(_int));

    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 0);

    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    /* Gauge_HP */
    m_WorldMatrix._11 = 235;
    m_WorldMatrix._22 = 6;

    m_WorldMatrix._41 = m_fXt - (g_iWinCX / 2);
    m_WorldMatrix._42 = -1.f * m_fYt + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    int iGauge = 1;
    m_pShader->Set_ConstTable("g_ToF", &iGauge, sizeof(_int));

    _float fHP = // -0.5 ~ 0.5
        (*m_pPlayerHP - (m_fPlayerMaxHP / 2.f)) / m_fPlayerMaxHP;
    


    m_pShader->Set_ConstTable("g_fHPDelta", &fHP, sizeof(_float));

    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 3);

    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    /* Gauge_MP */
    m_WorldMatrix._11 = 235;
    m_WorldMatrix._22 = 6;

    m_WorldMatrix._41 = m_fXt - (g_iWinCX / 2) + 430;
    m_WorldMatrix._42 = -1.f * m_fYt + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    iGauge = 0;
    m_pShader->Set_ConstTable("g_ToF", &iGauge, sizeof(_int));

    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 2);

    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    ///* Frame */
    //m_WorldMatrix._11 = m_fSizeX;
    //m_WorldMatrix._22 = m_fSizeY;

    //m_WorldMatrix._41 = m_fX - (g_iWinCX / 2);
    //m_WorldMatrix._42 = -m_fY + (g_iWinCY / 2);

    //m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    //m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    //m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    //int iFrame = 0;
    //m_pShader->Set_ConstTable("g_ToF", &iFrame, sizeof(_int));
    //fHP = (100 - (100 / 2.f)) / 100;
    //m_pShader->Set_ConstTable("g_fHPDelta", &fHP, sizeof(_float));

    //m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 0);

    //m_pShader->Begin_Shader(0);
    //m_pVIBuffer->Render_VIBuffer();
    //m_pShader->End_Shader();

    /* Effect_HP */
    m_WorldMatrix._11 = 235;
    m_WorldMatrix._22 = 8;

    m_WorldMatrix._41 = m_fXt1 - (g_iWinCX / 2);
    m_WorldMatrix._42 = -m_fYt1 + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    int iEffect = 1;
    m_pShader->Set_ConstTable("g_ToF", &iEffect, sizeof(_int));

    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 7);

    m_pShader->Begin_Shader(1);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    /* Effect_MP */
    m_WorldMatrix._11 = 242;
    m_WorldMatrix._22 = 8;

    m_WorldMatrix._41 = m_fXt1 - (g_iWinCX / 2) + 430;
    m_WorldMatrix._42 = -m_fYt1 + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    iEffect = 1;
    m_pShader->Set_ConstTable("g_ToF", &iEffect, sizeof(_int));

    m_pShader->Set_TextureConstTable("g_DiffuseSampler", m_pTexture, 7);

    m_pShader->Begin_Shader(1);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CPlayerGauge::Add_Components()
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

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_PlayerGauge"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
    {
        return E_FAIL;
    }

    return S_OK;
}

CPlayerGauge* CPlayerGauge::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayerGauge* pInstance = new CPlayerGauge(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayerGauge::Clone(void* pArg)
{
    CPlayerGauge* pInstance = new CPlayerGauge(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayerGauge::Free()
{
    CGameObject::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pVIBuffer);
}
