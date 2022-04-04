#include "pch.h"
#include "Player.h"
#include "GameInstance.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "Transform.h"
#include "Collider.h"
#include "Sound_Manager.h"

BEGIN(Client)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

HRESULT CPlayer::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(150.f, g_iCenterY, 0.f));
    m_pTransform->Set_Scale(_float3(150.f, 150.f, 0.f));

    m_iState = STATE_IDLE;

    return S_OK;
}

_int CPlayer::Tick(_double DeltaTime)
{
    m_fFrame += 20 * DeltaTime;

    if (m_fFrame >= 20.f)
    {
        m_fFrame = 0.f;
    }

    _uint i = m_eLevelIndex;

    Player_Behavior(DeltaTime);

    if (true == OnCollision())
    {
        CSound_Manager::Get_Instance()->PlaySound(TEXT("Crash.mp3"), CSound_Manager::CHANNELID::CRASH);

        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

        m_bIsGameOver = true;

        pGameInstance->OnNotify(m_bIsGameOver);

        RELEASE_INSTANCE(CGameInstance);

        __super::Add_DeleteQueue(m_strMyLayerTag, this);
    }

    m_pCollider->Tick(DeltaTime);

    return _int();
}

_int CPlayer::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_RenderObject(CRenderer::RENDER_PLAYER, this)))
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

HRESULT CPlayer::Render()
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

HRESULT CPlayer::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTexture = CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Player/%d.png"), 20))))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBuffer = CVIBuffer_Rect::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    _float fSpeed = 300.f;

    if (FAILED(__super::Add_Component(TEXT("Com_Transform"), m_pTransform = CTransform::Create(m_pGraphic_Device, &fSpeed))))
    {
        return E_FAIL;
    }

    CCollider::COLLIDERDESC ColliderDesc;
    
    ColliderDesc.pParentWorldMatrix = m_pTransform->Get_WorldMatrix();
    ColliderDesc.vInitScale         = _float3(75.f, 80.f, 0.f);
    ColliderDesc.bCustomizable      = true;
    
    if (FAILED(__super::Add_Component(TEXT("Com_Collider"), m_pCollider = CCollider::Create(m_pGraphic_Device, &ColliderDesc))))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CPlayer::Set_ConstTable()
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
    m_pShader->Set_ConstTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &ProjMatrix, sizeof(_matrix));

    m_pShader->Set_Texture("g_MtrlDiffuse", m_pTexture, (_uint)m_fFrame);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CPlayer::Player_Behavior(_double DeltaTime)
{
    switch (m_iState)
    {
    case STATE_IDLE:
    {
        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

        if (pGameInstance->Key_Down(KEY_SPACE))
        {
            CSound_Manager::Get_Instance()->PlaySound(TEXT("Jump.mp3"), CSound_Manager::CHANNELID::JUMP);

            m_iState = STATE_JUMP;

        }

        RELEASE_INSTANCE(CGameInstance);

        break;
    }
    case STATE_JUMP:
    {
        m_fTimeAcc += DeltaTime;

        m_pTransform->Jumping(m_fTimeAcc, 9.8f * 30, 10.f);

        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

        if (pGameInstance->Key_Down(KEY_SPACE))
        {
            CSound_Manager::Get_Instance()->PlaySound(TEXT("Jump.mp3"), CSound_Manager::CHANNELID::JUMP);

            m_fTimeAcc = 0.f;
        }

        if (pGameInstance->Key_Down(KEY_W))
        {
            m_fTimeAcc = 0.f;

            m_iState = STATE_IDLE;
        }

        RELEASE_INSTANCE(CGameInstance);

        break;
    }
    }

    return S_OK;
}

_bool CPlayer::OnCollision()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CCollider* pGNDBlockCollider    = dynamic_cast<CCollider*>(pGameInstance->Get_ComponentPtr(TEXT("Layer_Block_GND"), TEXT("Com_Collider")));
    CCollider* pTOPBlcokCollider    = dynamic_cast<CCollider*>(pGameInstance->Get_ComponentPtr(TEXT("Layer_Block_TOP"), TEXT("Com_Collider")));
    CCollider* pGNDBoundingCollider = dynamic_cast<CCollider*>(pGameInstance->Get_ComponentPtr(TEXT("Layer_Box_GND"), TEXT("Com_Collider")));
    CCollider* pTOPBoundingCollider = dynamic_cast<CCollider*>(pGameInstance->Get_ComponentPtr(TEXT("Layer_Box_TOP"), TEXT("Com_Collider")));

    RELEASE_INSTANCE(CGameInstance);

    if (nullptr != pGNDBlockCollider && m_pCollider->OnCollision(pGNDBlockCollider))
    {
        return true;
    }
    else if (nullptr != pTOPBlcokCollider && m_pCollider->OnCollision(pTOPBlcokCollider))
    {
        return true;
    }
    else if (nullptr != pGNDBoundingCollider && m_pCollider->OnCollision(pGNDBoundingCollider))
    {
        return true;
    }
    else if (nullptr != pTOPBoundingCollider && m_pCollider->OnCollision(pTOPBoundingCollider))
    {
        return true;
    }

    return false;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CPlayer* pInstance = new CPlayer(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pCollider);
    Safe_Release(m_pTransform);
}

END