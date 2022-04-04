#include "pch.h"
#include "UI.h"
#include "GameInstance.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "Texture.h"
#include "Shader.h"

BEGIN(Client)

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

HRESULT CUI::NativeConstruct(void* pArg)
{
    memcpy(&m_UIDesc, pArg, sizeof(UIDESC));

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    m_pTransform->Set_State(CTransform::STATE_POSITION, m_UIDesc.vPos);
    m_pTransform->Set_Scale(m_UIDesc.vScale);

    return S_OK;
}

_int CUI::Tick(_double DeltaTime)
{
    if (m_UIDesc.eUsage == USAGE_GAMEPLAY && m_eLevelIndex != LEVEL_GAMEPLAY)
    {
        __super::Add_DeleteQueue(m_strMyLayerTag, this);
    }
    else if (m_UIDesc.eUsage == USAGE_RESULT && m_eLevelIndex != LEVEL_RESULT)
    {
        __super::Add_DeleteQueue(m_strMyLayerTag, this);
    }

    return _int();
}

_int CUI::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_RenderObject(CRenderer::RENDER_SCORE, this)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

HRESULT CUI::Render()
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    m_pShader->Begin_Shader(1);

    m_pVIBuffer->Render();

    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CUI::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(TEXT("Com_Transform"), m_pTransform = CTransform::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBuffer = CVIBuffer_Rect::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
    {
        return E_FAIL;
    }

    if (m_UIDesc.eType == TYPE_STATIC)
    {
        if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTexture = CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Seed_Outline.png")))))
        {
            return E_FAIL;
        }
    }
    else
    {
        if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTexture = CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Number/%d.png"), 10))))
        {
            return E_FAIL;
        }
    }
   
    return S_OK;
}

HRESULT CUI::Set_ConstTable()
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

    m_pShader->Set_Texture("g_MtrlDiffuse", m_pTexture, m_iScore);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CUI* pInstance = new CUI(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}

END