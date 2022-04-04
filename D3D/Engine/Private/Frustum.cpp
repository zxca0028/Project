#include "Frustum.h"
#include "PipeLine.h"

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

HRESULT CFrustum::NativeConstruct_Prototype()
{
	m_vOriginal_Point[0] = _float3(-1.f,  1.f,  0.f);
	m_vOriginal_Point[1] = _float3( 1.f,  1.f,  0.f);
	m_vOriginal_Point[2] = _float3( 1.f, -1.f,  0.f);
	m_vOriginal_Point[3] = _float3(-1.f, -1.f,  0.f);

	m_vOriginal_Point[4] = _float3(-1.f,  1.f,  1.f);
	m_vOriginal_Point[5] = _float3( 1.f,  1.f,  1.f);
	m_vOriginal_Point[6] = _float3( 1.f, -1.f,  1.f);
	m_vOriginal_Point[7] = _float3(-1.f, -1.f,  1.f);

	return S_OK;
}

HRESULT CFrustum::NativeConstruct(void* pArg)
{
	return S_OK;
}

void CFrustum::Update_Frustum()
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix		ProjMatrix = pPipeLine->Get_Transform(D3DTS_PROJECTION);
	_matrix		ViewMatrix = pPipeLine->Get_Transform(D3DTS_VIEW);

	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vWorldPoint[i], &m_vOriginal_Point[i], &ProjMatrix);
		D3DXVec3TransformCoord(&m_vWorldPoint[i], &m_vWorldPoint[i],     &ViewMatrix);
	}

	D3DXPlaneFromPoints(&m_Plane[0], &m_vWorldPoint[1], &m_vWorldPoint[5], &m_vWorldPoint[6]);
	D3DXPlaneFromPoints(&m_Plane[1], &m_vWorldPoint[4], &m_vWorldPoint[0], &m_vWorldPoint[3]);
	D3DXPlaneFromPoints(&m_Plane[2], &m_vWorldPoint[4], &m_vWorldPoint[5], &m_vWorldPoint[1]);
	D3DXPlaneFromPoints(&m_Plane[3], &m_vWorldPoint[3], &m_vWorldPoint[2], &m_vWorldPoint[6]);
	D3DXPlaneFromPoints(&m_Plane[4], &m_vWorldPoint[5], &m_vWorldPoint[4], &m_vWorldPoint[7]);
	D3DXPlaneFromPoints(&m_Plane[5], &m_vWorldPoint[0], &m_vWorldPoint[1], &m_vWorldPoint[2]);

	RELEASE_INSTANCE(CPipeLine);
}

void CFrustum::Transform_ToLocalSpace(_matrix WorldMatrix)
{
	_matrix		WorldMatrixInverse;
	_float3		vLocalPos[8];

	D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vLocalPos[i], &m_vWorldPoint[i], &WorldMatrixInverse);
	}		

	D3DXPlaneFromPoints(&m_Plane[0], &vLocalPos[1], &vLocalPos[5], &vLocalPos[6]);
	D3DXPlaneFromPoints(&m_Plane[1], &vLocalPos[4], &vLocalPos[0], &vLocalPos[3]);
	D3DXPlaneFromPoints(&m_Plane[2], &vLocalPos[4], &vLocalPos[5], &vLocalPos[1]);
	D3DXPlaneFromPoints(&m_Plane[3], &vLocalPos[3], &vLocalPos[2], &vLocalPos[6]);
	D3DXPlaneFromPoints(&m_Plane[4], &vLocalPos[5], &vLocalPos[4], &vLocalPos[7]);
	D3DXPlaneFromPoints(&m_Plane[5], &vLocalPos[0], &vLocalPos[1], &vLocalPos[2]);
}

void CFrustum::Transform_ToWorldSpace()
{
	D3DXPlaneFromPoints(&m_Plane[0], &m_vWorldPoint[1], &m_vWorldPoint[5], &m_vWorldPoint[6]);
	D3DXPlaneFromPoints(&m_Plane[1], &m_vWorldPoint[4], &m_vWorldPoint[0], &m_vWorldPoint[3]);
	D3DXPlaneFromPoints(&m_Plane[2], &m_vWorldPoint[4], &m_vWorldPoint[5], &m_vWorldPoint[1]);
	D3DXPlaneFromPoints(&m_Plane[3], &m_vWorldPoint[3], &m_vWorldPoint[2], &m_vWorldPoint[6]);
	D3DXPlaneFromPoints(&m_Plane[4], &m_vWorldPoint[5], &m_vWorldPoint[4], &m_vWorldPoint[7]);
	D3DXPlaneFromPoints(&m_Plane[5], &m_vWorldPoint[0], &m_vWorldPoint[1], &m_vWorldPoint[2]);
}

bool CFrustum::isIn(_float3 vWorldPos, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < D3DXPlaneDotCoord(&m_Plane[i], &vWorldPos))
		{
			return false;
		}
	}

	return true;
}

CFrustum* CFrustum::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFrustum* pInstance = new CFrustum(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Frustum");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CFrustum::Clone(void* pArg)
{
	AddRef();

	return this;
}

void CFrustum::Free()
{
	__super::Free();
}
