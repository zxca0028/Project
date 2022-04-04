#include "VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CComponent(pGraphic_Device)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
    : CComponent(rhs)

    , m_pVB(rhs.m_pVB)
    , m_pIB(rhs.m_pIB)

    , m_ePrimitiveType(rhs.m_ePrimitiveType)

    , m_iStride(rhs.m_iStride)
    , m_iNumVertices(rhs.m_iNumVertices)
    , m_dwVBUsage(rhs.m_dwVBUsage)
    , m_dwFVF(rhs.m_dwFVF)
    , m_pVerticesPos(rhs.m_pVerticesPos)

    , m_iNumPrimitive(rhs.m_iNumPrimitive)
    , m_iVPP(rhs.m_iVPP)
    , m_iIndexSize(rhs.m_iIndexSize)
    , m_dwIBUsage(rhs.m_dwIBUsage)
    , m_pIndices(rhs.m_pIndices)
{
    Safe_AddRef(m_pVB);
    Safe_AddRef(m_pIB);
}

const _uint CVIBuffer::Get_NumPloygons() const
{
    return m_iNumPrimitive;
}

const void CVIBuffer::Get_PolygonVertexPositions(_uint iPolygonIndex, _float3* pPoint) const
{
    _uint iIndex = iPolygonIndex * 3;

    if (m_iIndexSize == 2)
    {
        pPoint[0] = m_pVerticesPos[((_ushort*)m_pIndices)[iIndex + 0]];
        pPoint[1] = m_pVerticesPos[((_ushort*)m_pIndices)[iIndex + 1]];
        pPoint[2] = m_pVerticesPos[((_ushort*)m_pIndices)[iIndex + 2]];
    }
    else if (m_iIndexSize == 4)
    {
        pPoint[0] = m_pVerticesPos[((_uint*)m_pIndices)[iIndex + 0]];
        pPoint[1] = m_pVerticesPos[((_uint*)m_pIndices)[iIndex + 1]];
        pPoint[2] = m_pVerticesPos[((_uint*)m_pIndices)[iIndex + 2]];
    }
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, m_dwVBUsage, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
    {
        return E_FAIL;
    }
    if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexSize * m_iNumPrimitive * m_iVPP, m_dwIBUsage, m_iIndexSize == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, nullptr)))
    {
        return E_FAIL;
    }

    m_pVerticesPos = new _float3[m_iNumVertices];

    if (m_iIndexSize == 2)
    {
        m_pIndices = new _ushort[m_iNumPrimitive * 3];
    }
    else if(m_iIndexSize == 4)
    {
        m_pIndices = new _uint[m_iNumPrimitive * 3];
    }

    return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer::Render_VIBuffer()
{
    m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);

    m_pGraphic_Device->SetFVF(m_dwFVF);

    m_pGraphic_Device->SetIndices(m_pIB);

    m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);;

    return S_OK;
}

void CVIBuffer::Free()
{
    __super::Free();

    if (false == m_isClone)
    {
        Safe_Delete_Array(m_pVerticesPos);
        Safe_Delete_Array(m_pIndices);
    }

    Safe_Release(m_pVB);
    Safe_Release(m_pIB);
}
