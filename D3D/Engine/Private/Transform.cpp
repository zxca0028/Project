#include "Transform.h"
#include "VIBuffer_Terrain.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{}

HRESULT CTransform::NativeConstruct_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_TransformState, pArg, sizeof(TRANSFORMDESC));
	}
	return S_OK;
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Transform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Transform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}


const _matrix* CTransform::Get_WorldMatrix() const
{
	return &m_WorldMatrix;
}

const _matrix CTransform::Get_WorldMatrixInverse() const
{
	_matrix WorldMatrixInverse;

	D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &m_WorldMatrix);

	return WorldMatrixInverse;
}

const _float3 CTransform::Get_State(STATE eState) const
{
	return *(_float3*)&m_WorldMatrix.m[eState][0];
}

const _float3 CTransform::Get_Scale() const
{
	_float3 vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE::STATE_RIGHT);
	vUp    = Get_State(CTransform::STATE::STATE_UP);
	vLook  = Get_State(CTransform::STATE::STATE_LOOK);

	_float3 vScale;

	vScale.x = D3DXVec3Length(&vRight);
	vScale.y = D3DXVec3Length(&vUp);
	vScale.z = D3DXVec3Length(&vLook);

	return vScale;
}

void CTransform::Set_State(STATE eState, _float3 vState)
{
	memcpy(&m_WorldMatrix.m[eState][0], vState, sizeof(_float3));
}

void CTransform::Set_Scale(_float3 vScale)
{
	_float3 vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp    = Get_State(CTransform::STATE_UP);
	vLook  = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP,    *D3DXVec3Normalize(&vUp, &vUp)       * vScale.y);
	Set_State(CTransform::STATE_LOOK,  *D3DXVec3Normalize(&vLook, &vLook)   * vScale.z);
}

void CTransform::Move_Front(_double DeltaTime)
{
	_float3 vLook = Get_State(CTransform::STATE_LOOK);
	_float3 vPos  = Get_State(CTransform::STATE_POSITION);

	D3DXVec3Normalize(&vLook, &vLook);

	vPos += vLook * m_TransformState.fSpeedPerSec * DeltaTime;

	Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Move_Back(_double DeltaTime)
{
	_float3 vLook = Get_State(CTransform::STATE_LOOK);
	_float3 vPos  = Get_State(CTransform::STATE_POSITION);

	D3DXVec3Normalize(&vLook, &vLook);

	vPos -= vLook * m_TransformState.fSpeedPerSec * DeltaTime;

	Set_State(CTransform::STATE_POSITION, vPos);

}

void CTransform::Move_Left(_double DeltaTime)
{
	_float3 vRight = Get_State(CTransform::STATE_RIGHT);
	_float3 vPos   = Get_State(CTransform::STATE_POSITION);

	D3DXVec3Normalize(&vRight, &vRight);

	vPos -= vRight * m_TransformState.fSpeedPerSec * DeltaTime;

	Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Move_Right(_double DeltaTime)
{
	_float3 vRight = Get_State(CTransform::STATE_RIGHT);
	_float3 vPos   = Get_State(CTransform::STATE_POSITION);

	D3DXVec3Normalize(&vRight, &vRight);

	vPos += vRight * m_TransformState.fSpeedPerSec * DeltaTime;

	Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Move_Up(_double DeltaTime)
{
	_float3 vUp  = Get_State(CTransform::STATE_UP);
	_float3 vPos = Get_State(CTransform::STATE_POSITION);

	D3DXVec3Normalize(&vUp, &vUp);

	vPos += vUp * m_TransformState.fSpeedPerSec * DeltaTime;

	Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Move_Down(_double DeltaTime)
{
	_float3 vUp = Get_State(CTransform::STATE_UP);
	_float3 vPos = Get_State(CTransform::STATE_POSITION);

	D3DXVec3Normalize(&vUp, &vUp);

	vPos -= vUp * m_TransformState.fSpeedPerSec * DeltaTime;

	Set_State(CTransform::STATE_POSITION, vPos);
}

void CTransform::Rotation_Axis(_float3 vAxis, _double DeltaTime)
{
	_float3 vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp    = Get_State(CTransform::STATE_UP);
	vLook  = Get_State(CTransform::STATE_LOOK);

	_matrix RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformState.fRotationPerSec * DeltaTime);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix));
	Set_State(CTransform::STATE_UP,    *D3DXVec3TransformNormal(&vUp,    &vUp,    &RotationMatrix));
	Set_State(CTransform::STATE_LOOK,  *D3DXVec3TransformNormal(&vLook,  &vLook,  &RotationMatrix));
}

void CTransform::Rotation_Axis_Radian(_float3 vAxis, _float vRadian)
{
	_float3 vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp    = Get_State(CTransform::STATE_UP);
	vLook  = Get_State(CTransform::STATE_LOOK);

	_matrix RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, vRadian);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix));
	Set_State(CTransform::STATE_UP,    *D3DXVec3TransformNormal(&vUp,    &vUp,    &RotationMatrix));
	Set_State(CTransform::STATE_LOOK,  *D3DXVec3TransformNormal(&vLook,  &vLook,  &RotationMatrix));
}

void CTransform::Setup_Rotation(_float3 vAxis, _float fRadian)
{
	_float3 vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp    = Get_State(CTransform::STATE_UP);
	vLook  = Get_State(CTransform::STATE_LOOK);

	_matrix RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix));
	Set_State(CTransform::STATE_UP,    *D3DXVec3TransformNormal(&vUp,    &vUp,    &RotationMatrix));
	Set_State(CTransform::STATE_LOOK,  *D3DXVec3TransformNormal(&vLook,  &vLook,  &RotationMatrix));
}

void CTransform::Setup_CameraAngle(_float3 vAxis, _float fRadian)
{
	_float3 vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp = Get_State(CTransform::STATE_UP);
	vLook = Get_State(CTransform::STATE_LOOK);

	vRight = _float3(1.f, 0.f, 0.f);
	vUp = _float3(0.f, 1.f, 0.f);
	vLook = _float3(0.f, 0.f, 1.f);

	_matrix RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix));
	Set_State(CTransform::STATE_UP, *D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix));
	Set_State(CTransform::STATE_LOOK, *D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix));
}

void CTransform::Chase_Target(const CTransform* pTargetTransform, _double DeltaTime)
{
	_float3 vMyPos     = Get_State(CTransform::STATE_POSITION);
	_float3 vTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vChase = vTargetPos - vMyPos;

	D3DXVec3Normalize(&vChase, &vChase);

	vMyPos += vChase * m_TransformState.fSpeedPerSec * DeltaTime;

	Set_State(CTransform::STATE_POSITION, vMyPos);

	Look_Target(pTargetTransform);
}

void CTransform::Look_Target(const CTransform* pTargetTransform)
{
	_float3 vMyPos     = Get_State(CTransform::STATE_POSITION);
	_float3 vTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vLook = vTargetPos - vMyPos;
	D3DXVec3Normalize(&vLook, &vLook);
	vLook *= Get_Scale().z;

	_float3	vRight;
	_float3 vUp = Get_State(CTransform::STATE_UP);
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vRight = vRight * Get_Scale().x;

	Set_State(CTransform::STATE_LOOK,  vLook);
	Set_State(CTransform::STATE_RIGHT, vRight);
}

bool CTransform::Chase_Target(_float3 vTargetPos, _double DeltaTime)
{
	if (false == m_bIsMoving)
	{
		return false;
	}

	_float3 vMyPos = Get_State(CTransform::STATE_POSITION);
	_float3 vDir   = vTargetPos - vMyPos;
	vDir.y = 0;
	_float  fDist  = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);

	if (fDist > 0.1f)
	{
		vMyPos += vDir * m_TransformState.fSpeedPerSec * DeltaTime;
	}
	else
	{
		m_bIsMoving = false;
	}	

	Set_State(CTransform::STATE_POSITION, vMyPos);

	Look_Target(vTargetPos);

	return true;
}

void CTransform::Look_Target(_float3 vTargetPos)
{
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);
	//_float3 vMyLook   = Get_State(CTransform::STATE_LOOK);
	//
	//_float3 vChaseLook = vTargetPos - vPosition;
	//vChaseLook.y = 0.f;
	//D3DXVec3Normalize(&vChaseLook, &vChaseLook);
	//D3DXVec3Normalize(&vMyLook, &vMyLook);
	//
	//_float fAngle = D3DXVec3Dot(&vChaseLook, &vMyLook);

	_float3	vLook = vTargetPos - vPosition;
	vLook.y = 0;
	D3DXVec3Normalize(&vLook, &vLook);
	vLook = vLook * Get_Scale().z;
	
	_float3 vUp = Get_State(CTransform::STATE_UP);
	
	_float3	vRight;
	_float3 vAxisY = _float3(0.f, 1.f, 0.f);
	D3DXVec3Cross(&vRight, &vAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vRight = vRight * Get_Scale().x;
	
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	vUp = vUp * Get_Scale().y;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Ride_Terrain(CVIBuffer_Terrain* pVIBuffer, const _matrix* pTerrainWorldMatrix)
{
	_float3 vMyWorldPos = Get_State(CTransform::STATE_POSITION);

	_matrix vTerrainWorldMatrixInv = *(D3DXMatrixInverse(&vTerrainWorldMatrixInv, nullptr, pTerrainWorldMatrix));

	_float3 vMyLocalPos = *(D3DXVec3TransformCoord(&vMyLocalPos, &vMyWorldPos, &vTerrainWorldMatrixInv));

	D3DXPLANE Plane = pVIBuffer->Get_Plane(vMyLocalPos);

	vMyLocalPos.y = vMyLocalPos.y - D3DXPlaneDotCoord(&Plane, &vMyLocalPos);

	D3DXVec3TransformCoord(&vMyWorldPos, &vMyLocalPos, pTerrainWorldMatrix);

	Set_State(CTransform::STATE_POSITION, vMyWorldPos);
}

void CTransform::Ride_Navigation(D3DXPLANE Plane)
{
	_float3 vMyWorldPos = Get_State(CTransform::STATE_POSITION);

	vMyWorldPos.y = vMyWorldPos.y - D3DXPlaneDotCoord(&Plane, &vMyWorldPos);

	Set_State(CTransform::STATE_POSITION, vMyWorldPos);
}

void CTransform::Remove_Rotation()
{
	Set_State(CTransform::STATE_RIGHT, _float3(1.f, 0.f, 0.f) * Get_Scale().x);
	Set_State(CTransform::STATE_UP,    _float3(0.f, 1.f, 0.f) * Get_Scale().y);
	Set_State(CTransform::STATE_LOOK,  _float3(0.f, 0.f, 1.f) * Get_Scale().z);
}
