#include "pch.h"
#include "VIBuffer.h"

BEGIN(Client)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

HRESULT CVIBuffer::NativeConstruct()
{
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, m_dwVBUsage, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexSize * m_iNumPrimitive * m_iVPP, m_dwIBUsage, m_iIndexSize == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, nullptr)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);

	m_pGraphic_Device->SetFVF(m_dwFVF);

	m_pGraphic_Device->SetIndices(m_pIB);

	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}

END