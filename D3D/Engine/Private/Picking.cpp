#include "Picking.h"
#include "VIBuffer.h"
#include "PipeLine.h"
#include "Navigation.h"
#include "Cell.h"

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{}

HRESULT CPicking::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPicking::Compute_MousePosInWorld(HWND hWnd)
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_float4 vMousePos = {};
	POINT   ptMouse   = {};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9 ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width / 2.f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height/ 2.f) + 1.f;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	_matrix ProjMatrix    = pPipeLine->Get_Transform(D3DTS_PROJECTION);
	_matrix ProjMatrixInv = *D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrix);
	D3DXVec4Transform(&vMousePos, &vMousePos, &ProjMatrixInv);

	m_vMousePivot = _float3(0.f, 0.f, 0.f);
	m_vMouseRay   = _float3(
		 vMousePos.x - m_vMousePivot.x, 
		 vMousePos.y - m_vMousePivot.y, 
		 vMousePos.z - m_vMousePivot.z);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	_matrix ViewMatrix    = pPipeLine->Get_Transform(D3DTS_VIEW);
	_matrix ViewMatrixInv = *D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrix);

	D3DXVec3TransformCoord(&m_vMousePivot, &m_vMousePivot, &ViewMatrixInv);
	D3DXVec3TransformNormal(&m_vMouseRay, &m_vMouseRay, &ViewMatrixInv);

	D3DXVec3Normalize(&m_vMouseRay, &m_vMouseRay);

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

_float3* CPicking::Compute_PickingPoint(CVIBuffer* pVIBuffer, _matrix WorldMatrix)
{
	_float3 vMouseLocalRay, vMouseLocalPivot;

	_matrix WorldMatrixInv = *D3DXMatrixInverse(&WorldMatrixInv, nullptr, &WorldMatrix);

	D3DXVec3TransformCoord(&vMouseLocalPivot, &m_vMousePivot, &WorldMatrixInv);
	D3DXVec3TransformNormal(&vMouseLocalRay, &m_vMouseRay, &WorldMatrixInv);

	_float3 vVertexPoint[3];
	_uint iIndex = pVIBuffer->Get_NumPloygons();
	for (_uint i = 0; i < iIndex; ++i)
	{
		pVIBuffer->Get_PolygonVertexPositions(i, vVertexPoint);

		_float fU, fV, fDist;

		if (D3DXIntersectTri(&vVertexPoint[0], &vVertexPoint[1], &vVertexPoint[2], &vMouseLocalPivot, &vMouseLocalRay, &fU, &fV, &fDist))
		{
			m_vResultPos = vMouseLocalPivot + *D3DXVec3Normalize(&vMouseLocalRay, &vMouseLocalRay) * fDist;
		
			return &m_vResultPos;
		}
	}

	return nullptr;
}

_float3* CPicking::Compute_PickingPoint(CNavigation* pNavigation, _float3 vPlayerPos)
{
	_uint iIndex = (*pNavigation->Get_Cells()).size();

	for (_uint i = 0; i < iIndex; ++i)
	{
		_float fU, fV, fDist;

		_float3 PointA = (*pNavigation->Get_Cells())[i]->Get_Points(CCell::POINT_A);
		_float3 PointB = (*pNavigation->Get_Cells())[i]->Get_Points(CCell::POINT_B);
		_float3 PointC = (*pNavigation->Get_Cells())[i]->Get_Points(CCell::POINT_C);

		if (D3DXIntersectTri(&PointA, &PointB, &PointC, &m_vMousePivot, &m_vMouseRay, &fU, &fV, &fDist))
		{
			m_vResultPos = m_vMousePivot + *D3DXVec3Normalize(&m_vMouseRay, &m_vMouseRay) * fDist;

			m_iCount = 0;

			return &m_vResultPos;
		}
	}

	/* Nagigation Mesh 밖을 클릭했을 경우 */
	_float3 vLook_PivotToPlayer = vPlayerPos - m_vMousePivot;
	_float3 vLook = *D3DXVec3Normalize(&vLook_PivotToPlayer, &vLook_PivotToPlayer) - m_vMouseRay;

	m_vMouseRay += vLook / 10.f;

	if (m_iCount > 100)
	{
		m_iCount = 0;
		return nullptr;
	}

	++m_iCount;

	return Compute_PickingPoint(pNavigation, vPlayerPos);
}

CPicking* CPicking::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPicking* pInstance = new CPicking(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Picking");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CPicking::Clone(void* pArg)
{
	AddRef();

	return this;
}

void CPicking::Free()
{
	__super::Free();
}
