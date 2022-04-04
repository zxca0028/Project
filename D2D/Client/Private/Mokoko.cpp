#include "pch.h"
#include "Mokoko.h"
#include "GameInstance.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "Texture.h"
#include "Shader.h"
#include "Collider.h"
#include "Sound_Manager.h"

BEGIN(Client)

CMokoko::CMokoko(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

HRESULT CMokoko::NativeConstruct(void* pArg)
{
    memcpy(&m_MokokoDesc, pArg, sizeof(MOKOKODESC));

    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    m_pTransform->Set_State(CTransform::STATE_POSITION, m_MokokoDesc.vInitPos);
    m_pTransform->Set_Scale(_float3(104.f, 104.f, 0.f));

    return S_OK;
}

_int CMokoko::Tick(_double DeltaTime)
{
    m_pTransform->Move_Left(DeltaTime);

    m_pCollider->Tick(DeltaTime);

    _float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

    if (vPos.x <= -100.f || true == m_bIsGameOver)
    {
        __super::Add_DeleteQueue(m_strMyLayerTag, this);
    }

    if (true == OnCollision())
    {
        CSound_Manager::Get_Instance()->PlaySound(TEXT("Effect.mp3"), CSound_Manager::CHANNELID::MOKOKO);

        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

        if (FAILED(pGameInstance->Plus_Score()))
        {
            return E_FAIL;
        }

        RELEASE_INSTANCE(CGameInstance);

        __super::Add_DeleteQueue(m_strMyLayerTag, this);
    }

    return _int();
}

_int CMokoko::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    
    if (FAILED(pGameInstance->Add_RenderObject(CRenderer::RENDER_OBJECT, this)))
    {
        return E_FAIL;
    }

#ifdef _DEBUG

    if (FAILED(pGameInstance->Add_RenderComponent(m_pCollider)))
    {
        return E_FAIL;
    }

#endif

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

HRESULT CMokoko::Render()
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

HRESULT CMokoko::Add_Components(void* pArg)
{
    _float fSpeed = m_MokokoDesc.fSpeed;

    if (FAILED(__super::Add_Component(TEXT("Com_Transform"), m_pTransform = CTransform::Create(m_pGraphic_Device, &fSpeed))))
    {
        return E_FAIL;
    }

    CCollider::COLLIDERDESC ColliderDesc;

    ColliderDesc.pParentWorldMatrix = m_pTransform->Get_WorldMatrix();

    if (FAILED(__super::Add_Component(TEXT("Com_Collider"), m_pCollider = CCollider::Create(m_pGraphic_Device, &ColliderDesc))))
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

    if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTexture = CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Seed_Outline.png")))))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CMokoko::Set_ConstTable()
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

_bool CMokoko::OnCollision()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Get_ComponentPtr(TEXT("Layer_Player"),TEXT("Com_Collider")));

    RELEASE_INSTANCE(CGameInstance);

    if (nullptr != pPlayerCollider)
    {
        if (true == m_pCollider->OnCollision(pPlayerCollider))
        {
            return true;
        }
    }

    return false;
}

CMokoko* CMokoko::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CMokoko* pInstance = new CMokoko(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Mokoko");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMokoko::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pCollider);
    Safe_Release(m_pTransform);
}

END