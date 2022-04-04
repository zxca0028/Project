#include "pch.h"
#include "BackGround.h"
#include "GameInstance.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "Transform.h"

BEGIN(Client)

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

HRESULT CBackGround::NativeConstruct(void* pArg)
{
    memcpy(&m_BGDesc, pArg, sizeof(BGDESC));

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(m_BGDesc.fInitPosX, g_iCenterY, 0.f));
    m_pTransform->Set_Scale(_float3(1600, 1000, 0.f));

    return S_OK;
}

_int CBackGround::Tick(_double DeltaTime)
{
    if (true == m_bIsGameOver)
    {
        return _int();
    }

    m_pTransform->Move_Left(DeltaTime);

    _float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (vPos.x <= -100.f && false == m_bCreateLock)
    {
        _float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

        m_BGDesc.fInitPosX = vPos.x + 1600.f;

        if (FAILED(pGameInstance->Add_GameObject(TEXT("Layer_BackGround"), CBackGround::Create(m_pGraphic_Device, (void*)&m_BGDesc))))
        {
            return E_FAIL;
        }

        m_bCreateLock = true;
    }

    if (vPos.x <= -800.f)
    {
        __super::Add_DeleteQueue(TEXT("Layer_BackGround"), this);
    }

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CBackGround::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    
    if (FAILED(pGameInstance->Add_RenderObject(CRenderer::RENDER_BACKGROUND, this)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

HRESULT CBackGround::Render()
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    m_pShader->Begin_Shader(0);

    m_pVIBuffer->Render();

    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CBackGround::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTexture = CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/BackGround.png")))))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBuffer = CVIBuffer_Rect::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }
    
    _float fSpeed = m_BGDesc.fSpeed;
    
    if (FAILED(__super::Add_Component(TEXT("Com_Transform"), m_pTransform = CTransform::Create(m_pGraphic_Device, &fSpeed))))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CBackGround::Set_ConstTable()
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

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CBackGround* pInstance = new CBackGround(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create BackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackGround::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}

END