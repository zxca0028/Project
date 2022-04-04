#include "pch.h"
#include "Camera_Free.h"
#include "GameInstance.h"

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCamera(pGraphic_Device)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& rhs)
    : CCamera(rhs)
{
}

HRESULT CCamera_Free::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Free::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    __super::NativeConstruct(pArg);

    return S_OK;
}

_int CCamera_Free::Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _float3 vLook  = m_pTransform->Get_State(CTransform::STATE::STATE_LOOK);
    _float3 vAxisY = _float3(0.f, 1.f, 0.f);

    m_fRestriction = D3DXVec3Dot(&vLook, &vAxisY);

    if (pGameInstance->Key_Pressing(KEY_L_ALT))
    {
        if (pGameInstance->Key_Pressing(KEY_W))
        {
            m_pTransform->Move_Front(m_fCameraSpeed * DeltaTime);
        }
        if (pGameInstance->Key_Pressing(KEY_S))
        {
            m_pTransform->Move_Back(m_fCameraSpeed * DeltaTime);
        }
        if (pGameInstance->Key_Pressing(KEY_A))
        {
            m_pTransform->Move_Left(m_fCameraSpeed * DeltaTime);
        }
        if (pGameInstance->Key_Pressing(KEY_D))
        {
            m_pTransform->Move_Right(m_fCameraSpeed * DeltaTime);
        }
        if (pGameInstance->Key_Pressing(KEY_Q))
        {
            m_pTransform->Move_Up(m_fCameraSpeed * DeltaTime);
        }
        if (pGameInstance->Key_Pressing(KEY_E))
        {
            m_pTransform->Move_Down(m_fCameraSpeed * DeltaTime);
        }

        _long MouseMoveX = pGameInstance->Mouse_Move(CKey_Manager::MOUSE::X);
        _long MouseMoveY = pGameInstance->Mouse_Move(CKey_Manager::MOUSE::Y);

        if (0 != MouseMoveX)
        {
            m_pTransform->Rotation(_float3(0.f, 1.f, 0.f), DeltaTime * MouseMoveX * 0.1f);
        }
        if (0 < MouseMoveY && m_fRestriction > -0.99f)
        {
            _float3 vRight = m_pTransform->Get_State(CTransform::STATE::STATE_RIGHT);

            m_pTransform->Rotation(vRight, DeltaTime * MouseMoveY * 0.1f);
        }
        if (0 > MouseMoveY && m_fRestriction < 0.99f)
        {
            _float3 vRight = m_pTransform->Get_State(CTransform::STATE::STATE_RIGHT);

            m_pTransform->Rotation(vRight, DeltaTime * MouseMoveY * 0.1f);
        }
    }

    RELEASE_INSTANCE(CGameInstance);

    return __super::Tick(DeltaTime);
}

_int CCamera_Free::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CCamera_Free::Render()
{
    return S_OK;
}

HRESULT CCamera_Free::Add_Components(void* pArg)
{
    CTransform::TRANSFORMDESC TransformDesc;
    TransformDesc.fSpeedPerSec  = 10.f;
    TransformDesc.fRotatePerSec = D3DXToRadian(90.f);

    if (FAILED(Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
    {
        return E_FAIL;
    }

    return S_OK;
}

CCamera_Free* CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCamera_Free* pInstance = new CCamera_Free(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Create Camera_Free");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
    CCamera_Free* pInstance = new CCamera_Free(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Camera_Free");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCamera_Free::Free()
{
    __super::Free();
}
