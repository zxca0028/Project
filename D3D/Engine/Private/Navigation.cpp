#include "Navigation.h"
#include "Cell.h"

CNavigation::CNavigation(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CNavigation::CNavigation(const CNavigation& rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
	for (auto& pCell : m_Cells)
	{
		Safe_AddRef(pCell);
	}
}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar* pDataFiles)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pDataFiles, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
	{
		return E_FAIL;
	}
	
	_float3		vPoint[CCell::POINT_END];

	while (true)
	{
		ReadFile(hFile, vPoint, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		CCell* pCell = CCell::Create(m_pGraphic_Device, vPoint[CCell::POINT_A], vPoint[CCell::POINT_B], vPoint[CCell::POINT_C], m_Cells.size());

		if (nullptr == pCell)
		{
			return E_FAIL;
		}

		m_Cells.push_back(pCell);
	}

	if (FAILED(SetUp_Neighbor()))
	{
		return E_FAIL;
	}

	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CNavigation::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		m_NaviDesc = *(NAVIDESC*)pArg;
	}

	return S_OK;
}

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
			{
				continue;
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Points(CCell::POINT_A), pSourCell->Get_Points(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
			}
			if (true == pDestCell->Compare_Points(pSourCell->Get_Points(CCell::POINT_B), pSourCell->Get_Points(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
			}
			if (true == pDestCell->Compare_Points(pSourCell->Get_Points(CCell::POINT_C), pSourCell->Get_Points(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

HRESULT CNavigation::Update_WorldMatrix(const _matrix* pWorldMatrix)
{
	m_WorldMatrix = *pWorldMatrix;

	return S_OK;
}

bool CNavigation::Move_OnNavigation(_float3 vPosition, D3DXPLANE* pPlane)
{
	CCell* pNeighbor = nullptr;
	_uint iCount = 0;
	D3DXPLANE Plane;

	if (true == m_Cells[m_NaviDesc.iCurrentCellIndex]->isIn(vPosition, &pNeighbor))
	{
		_float3 vPointA = m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Points(CCell::POINT_A);
		_float3 vPointB = m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Points(CCell::POINT_B);
		_float3 vPointC = m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Points(CCell::POINT_C);

		D3DXPlaneFromPoints(&Plane, &vPointA, &vPointB, &vPointC);

		*pPlane = Plane;

		m_iCurrentCellIndex = m_NaviDesc.iCurrentCellIndex;

		return true;
	}
	else
	{
		if (nullptr == pNeighbor)
		{

		}
		else
		{
			while (true)
			{
				++iCount;
				if (nullptr == pNeighbor || iCount > 100)
				{
					return false;
				}

				if (true == pNeighbor->isIn(vPosition, &pNeighbor))
				{
					m_NaviDesc.iCurrentCellIndex = pNeighbor->Get_Index();

					_float3 vPointA = m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Points(CCell::POINT_A);
					_float3 vPointB = m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Points(CCell::POINT_B);
					_float3 vPointC = m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Points(CCell::POINT_C);

					D3DXPlaneFromPoints(&Plane, &vPointA, &vPointB, &vPointC);

					*pPlane = Plane;
					
					m_iCurrentCellIndex = m_NaviDesc.iCurrentCellIndex;

					return true;
				}
			}
		}
	}

	return false;
}

#ifdef _DEBUG
HRESULT CNavigation::Render(const _matrix* pWorldMatrix)
{
	for (_uint i = 0; i < m_Cells.size(); ++i)
	{
		if (nullptr != m_Cells[i])
		{
			D3DXCOLOR		Color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

			if (i == m_NaviDesc.iCurrentCellIndex)
			{
				Color = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
			}

			m_Cells[i]->Render(pWorldMatrix, Color);

		}
	}

	return S_OK;
}
HRESULT CNavigation::Render()
{
	for (_uint i = 0; i < m_Cells.size(); ++i)
	{
		if (nullptr != m_Cells[i])
		{
			D3DXCOLOR		Color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

			if (i == m_NaviDesc.iCurrentCellIndex)
			{
				Color = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
			}

			m_Cells[i]->Render(&m_WorldMatrix, Color);

		}
	}

	return S_OK;
}
#endif // _DEBUG

CNavigation* CNavigation::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pDataFiles)
{
	CNavigation* pInstance = new CNavigation(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(pDataFiles)))
	{
		MSGBOX("Failed to Creating Navigation");
		Safe_Release(pInstance);
	}

	return pInstance;

}

CNavigation* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Navigation");
		Safe_Release(pInstance);
	}

	return pInstance;

}
void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
	{
		Safe_Release(pCell);
	}

	m_Cells.clear();
}
