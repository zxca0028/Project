#include "pch.h"
#include "UI.h"
#include "GameInstance.h"
#include "Collision_Manager.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CUI::CUI(const CUI& rhs)
    :CGameObject(rhs)
{}

HRESULT CUI::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CUI::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    D3DXMatrixIdentity(&m_WorldMatrix);
    D3DXMatrixIdentity(&m_ViewMatrix);

                                     // 창의 X,Y 크기
    D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

    m_fSizeX = 718;
    m_fSizeY = 122;

    // 좌상단이 0, 0 기준 좌표계에서의 UI의 중심 좌표
    m_fX     = (g_iWinCX / 2) + 19;
    m_fY     = (g_iWinCY - m_fSizeY / 2) - 20;

    m_WorldMatrix._11 = m_fSizeX;
    m_WorldMatrix._22 = m_fSizeY;

    // D3DXMatrixOrthoLH는 창 중앙이 0, 0이 되므로 적절히 변환
    m_WorldMatrix._41 =  m_fX - (g_iWinCX / 2);
    m_WorldMatrix._42 = -m_fY + (g_iWinCY / 2);

    return S_OK;
}

_int CUI::Tick(_double TimeDelta)
{
    __super::Tick(TimeDelta);

    return _int();
}

_int CUI::Late_Tick(_double TimeDelta)
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

HRESULT CUI::Render()
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CUI::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &m_ViewMatrix,  sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &m_ProjMatrix,  sizeof(_matrix));

    m_pShader->Set_TextureConstTable("g_tMtrlDiffuse", m_pTexture, 0);
    
    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CUI::Add_Components()
{
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

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_UI"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
    {
        return E_FAIL;
    }

    return S_OK;
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUI* pInstance = new CUI(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI::Clone(void* pArg)
{
    CUI* pInstance = new CUI(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI::Free()
{
    CGameObject::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pVIBuffer);
}
