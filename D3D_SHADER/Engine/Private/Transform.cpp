#include "Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform& rhs)
    : CComponent(rhs)
    , m_WorldMatrix(rhs.m_WorldMatrix)
{
}

HRESULT CTransform::NativeConstruct_Prototype()
{
    D3DXMatrixIdentity(&m_WorldMatrix);

    return S_OK;
}

HRESULT CTransform::NativeConstruct(void* pArg)
{
    if (nullptr != pArg)
    {
        memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));
    }

    return S_OK;
}

const _matrix* CTransform::Get_WorldMatrix() const
{
    return &m_WorldMatrix;
}

const _matrix CTransform::Get_WorldMatrixInv() const
{
    _matrix WorldMatrixInv = {};

    D3DXMatrixInverse(&WorldMatrixInv, nullptr, &m_WorldMatrix);

    return WorldMatrixInv;
}

const _float3 CTransform::Get_State(STATE eState) const
{
    return *(_float3*)&m_WorldMatrix.m[(_uint)eState][0];
}

const _float3 CTransform::Get_Scale() const
{
    _float3 vRight = Get_State(STATE::STATE_RIGHT);
    _float3 vUp    = Get_State(STATE::STATE_UP);
    _float3 vLook  = Get_State(STATE::STATE_LOOK);

    _float3 vScale = _float3(D3DXVec3Length(&vRight), D3DXVec3Length(&vUp), D3DXVec3Length(&vLook));

    return vScale;
}

void CTransform::Set_WorldMatrix(const _matrix& WorldMatrix)
{
    m_WorldMatrix = WorldMatrix;
}

void CTransform::Set_State(STATE eState, _float3 vState)
{
    memcpy(&m_WorldMatrix.m[(_uint)eState][0], vState, sizeof(_float3));
}

void CTransform::Set_Scale(_float3 vScale)
{
    _float3 vRight = Get_State(STATE::STATE_RIGHT);
    _float3 vUp    = Get_State(STATE::STATE_UP);
    _float3 vLook  = Get_State(STATE::STATE_LOOK);

    Set_State(STATE::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
    Set_State(STATE::STATE_UP,    *D3DXVec3Normalize(&vUp,    &vUp)    * vScale.y);
    Set_State(STATE::STATE_LOOK,  *D3DXVec3Normalize(&vLook,  &vLook)  * vScale.z);
}

void CTransform::Move_Front(_double DeltaTime)
{
    _float3 vLook = Get_State(STATE::STATE_LOOK);

    D3DXVec3Normalize(&vLook, &vLook);

    _float3 vPos = Get_State(STATE::STATE_POSITION);

    vPos += vLook * m_TransformDesc.fSpeedPerSec * (FLOAT)DeltaTime;

    Set_State(STATE::STATE_POSITION, vPos);
}

void CTransform::Move_Back(_double DeltaTime)
{
    _float3 vLook = Get_State(STATE::STATE_LOOK);

    D3DXVec3Normalize(&vLook, &vLook);

    _float3 vPos = Get_State(STATE::STATE_POSITION);

    vPos -= vLook * m_TransformDesc.fSpeedPerSec * (FLOAT)DeltaTime;

    Set_State(STATE::STATE_POSITION, vPos);
}

void CTransform::Move_Left(_double DeltaTime)
{
    _float3 vRight = Get_State(STATE::STATE_RIGHT);

    D3DXVec3Normalize(&vRight, &vRight);

    _float3 vPos = Get_State(STATE::STATE_POSITION);

    vPos -= vRight * m_TransformDesc.fSpeedPerSec * (FLOAT)DeltaTime;

    Set_State(STATE::STATE_POSITION, vPos);
}

void CTransform::Move_Right(_double DeltaTime)
{
    _float3 vRight = Get_State(STATE::STATE_RIGHT);

    D3DXVec3Normalize(&vRight, &vRight);

    _float3 vPos = Get_State(STATE::STATE_POSITION);

    vPos += vRight * m_TransformDesc.fSpeedPerSec * (FLOAT)DeltaTime;

    Set_State(STATE::STATE_POSITION, vPos);
}

void CTransform::Move_Up(_double DeltaTime)
{
    _float3 vUp = Get_State(STATE::STATE_UP);

    D3DXVec3Normalize(&vUp, &vUp);

    _float3 vPos = Get_State(STATE::STATE_POSITION);

    vPos += vUp * m_TransformDesc.fSpeedPerSec * (FLOAT)DeltaTime;

    Set_State(STATE::STATE_POSITION, vPos);
}

void CTransform::Move_Down(_double DeltaTime)
{
    _float3 vUp = Get_State(STATE::STATE_UP);

    D3DXVec3Normalize(&vUp, &vUp);

    _float3 vPos = Get_State(STATE::STATE_POSITION);

    vPos -= vUp * m_TransformDesc.fSpeedPerSec * (FLOAT)DeltaTime;

    Set_State(STATE::STATE_POSITION, vPos);
}

void CTransform::Rotation(_float3 vAxis, _double DeltaTime)
{
    _float3 vRight = Get_State(STATE::STATE_RIGHT);
    _float3 vUp    = Get_State(STATE::STATE_UP);
    _float3 vLook  = Get_State(STATE::STATE_LOOK);

    _matrix RotationMatrix;

    D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformDesc.fRotatePerSec * (FLOAT)DeltaTime);

    Set_State(STATE::STATE_RIGHT, *D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix));
    Set_State(STATE::STATE_UP,    *D3DXVec3TransformNormal(&vUp,    &vUp,    &RotationMatrix));
    Set_State(STATE::STATE_LOOK,  *D3DXVec3TransformNormal(&vLook,  &vLook,  &RotationMatrix));
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTransform* pInstance = new CTransform(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Create Transform");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
    CTransform* pInstance = new CTransform(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Transform");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTransform::Free()
{
    __super::Free();
}
