#include "pch.h"
#include "Transform.h"

BEGIN(Client)

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent(pGraphic_Device)
{
}

HRESULT CTransform::NativeConstruct(void* pArg)
{
    D3DXMatrixIdentity(&m_WorldMatrix);

    if (nullptr != pArg)
    {
        memcpy(&m_TransformPerSecond, pArg, sizeof(_float));
    }

    return S_OK;
}

const _matrix* CTransform::Get_WorldMatrix() const
{
    return &m_WorldMatrix;
}

const _matrix CTransform::Get_WorldMatrixInv() const
{
    _matrix WorldMatrixInv;

    D3DXMatrixInverse(&WorldMatrixInv, nullptr, &m_WorldMatrix);

    return WorldMatrixInv;
}

void CTransform::Set_WorldMatrix(_matrix WorldMatrix)
{
    m_WorldMatrix = WorldMatrix;
}

const _float3 CTransform::Get_State(STATE eState) const
{
    return *(_float3*)&m_WorldMatrix.m[(_uint)eState][0];
}

const _float3 CTransform::Get_Scale() const
{
    _float3 vRight = Get_State(CTransform::STATE_RIGHT);
    _float3 vUp    = Get_State(CTransform::STATE_UP);
    _float3 vLook  = Get_State(CTransform::STATE_LOOK);

    _float vScaleX = D3DXVec3Length(&vRight);
    _float vScaleY = D3DXVec3Length(&vUp);
    _float vScaleZ = D3DXVec3Length(&vLook);

    _float3 vScale = _float3(vScaleX, vScaleY, vScaleZ);

    return vScale;
}

void CTransform::Set_State(STATE eState, _float3 vState)
{
    memcpy(&m_WorldMatrix.m[(_uint)eState][0], vState, sizeof(_float3));
}

void CTransform::Set_Scale(_float3 vScale)
{
    _float3 vRight = Get_State(CTransform::STATE_RIGHT);
    _float3 vUp    = Get_State(CTransform::STATE_UP);
    _float3 vLook  = Get_State(CTransform::STATE_LOOK);

    Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
    Set_State(CTransform::STATE_UP,    *D3DXVec3Normalize(&vUp,    &vUp)    * vScale.y);
    Set_State(CTransform::STATE_LOOK,  *D3DXVec3Normalize(&vLook,  &vLook)  * vScale.z);
}

void CTransform::Move_Up(_double DeltaTime)
{
    _float3 vUp = Get_State(CTransform::STATE_UP);

    D3DXVec3Normalize(&vUp, &vUp);

    _float3 vPos = Get_State(CTransform::STATE_POSITION);

    vPos += vUp * m_TransformPerSecond * (_float)DeltaTime;

    Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Move_Down(_double DeltaTime)
{
    _float3 vUp = Get_State(CTransform::STATE_UP);

    D3DXVec3Normalize(&vUp, &vUp);

    _float3 vPos = Get_State(CTransform::STATE_POSITION);

    vPos -= vUp * m_TransformPerSecond * (_float)DeltaTime;

    Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Move_Left(_double DeltaTime)
{
    _float3 vRight = Get_State(CTransform::STATE_RIGHT);

    D3DXVec3Normalize(&vRight, &vRight);

    _float3 vPos = Get_State(CTransform::STATE_POSITION);

    vPos -= vRight * m_TransformPerSecond * (_float)DeltaTime;

    Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Move_Right(_double DeltaTime)
{
    _float3 vRight = Get_State(CTransform::STATE_RIGHT);

    D3DXVec3Normalize(&vRight, &vRight);

    _float3 vPos = Get_State(CTransform::STATE_POSITION);

    vPos += vRight * m_TransformPerSecond * (_float)DeltaTime;

    Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Rotation(_float fDeg)
{
    _matrix RotationMatrix;

    _float3 vAxisZ = _float3(0.f, 0.f, 1.f);

    D3DXMatrixRotationAxis(&RotationMatrix, &vAxisZ, D3DXToRadian(fDeg));

    _float3 vUp    = Get_State(CTransform::STATE_UP);
    _float3 vRight = Get_State(CTransform::STATE_RIGHT);

    D3DXVec3TransformNormal(&vUp,    &vUp,    &RotationMatrix);
    D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);

    Set_State(CTransform::STATE_UP,    vUp);
    //Set_State(CTransform::STATE_RIGHT, vRight);
}

void CTransform::Remove_Rotation()
{
    Set_State(CTransform::STATE_RIGHT, _float3(1.f, 0.f, 0.f) * Get_Scale().x);
    Set_State(CTransform::STATE_UP,    _float3(0.f, 1.f, 0.f) * Get_Scale().y);
    Set_State(CTransform::STATE_LOOK,  _float3(0.f, 0.f, 1.f) * Get_Scale().z);
}

void CTransform::Jumping(_double DeltaTime, _float fGravity, _float fPower)
{
    _float3 vPos = Get_State(CTransform::STATE_POSITION);
    _float3 vUp  = Get_State(CTransform::STATE_UP);

    D3DXVec3Normalize(&vUp, &vUp);

	_float fDelta = (fPower + DeltaTime) + (-0.5f * fGravity * DeltaTime * DeltaTime);

    if (fDelta <= -20.f)
    {
        fDelta = -20.f;
    }

    vPos += vUp * fDelta;

    Set_State(CTransform::STATE_POSITION, vPos);
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CTransform* pInstance = new CTransform(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        Safe_Release(pInstance);
        MSGBOX("Failed to Create Transform");
    }
    
    return pInstance;
}

void CTransform::Free()
{
    __super::Free();
}

END