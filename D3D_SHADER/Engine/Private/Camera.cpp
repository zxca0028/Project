#include "Camera.h"
#include "PipeLine.h"
#include "Transform.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
    , m_pPipeLine(CPipeLine::Get_Instance())
{
    Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera& rhs)
    : CGameObject(rhs)
    , m_pPipeLine(rhs.m_pPipeLine)
{
    Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CCamera::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));
	}

	_float3 vLook = m_CameraDesc.vLookAt - m_CameraDesc.vPos;
	D3DXVec3Normalize(&vLook, &vLook);

	_float3 vRight;
	D3DXVec3Cross(&vRight, &m_CameraDesc.vAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_float3 vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE::STATE_UP,    vUp);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK,  vLook);
	m_pTransform->Set_State(CTransform::STATE::STATE_POSITION, m_CameraDesc.vPos);

	Set_Transform();

	return S_OK;
}

_int CCamera::Tick(_double DeltaTime)
{
	Set_Transform();

    return _int();
}

_int CCamera::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CCamera::Render()
{
    return S_OK;
}

HRESULT CCamera::Set_Transform()
{
	_matrix ViewMatrix = m_pTransform->Get_WorldMatrixInv();
	m_pPipeLine->Set_Transform(D3DTS_VIEW, ViewMatrix);

	_matrix ProjMatrix;
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	m_pPipeLine->Set_Transform(D3DTS_PROJECTION, ProjMatrix);

	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pPipeLine);
}
