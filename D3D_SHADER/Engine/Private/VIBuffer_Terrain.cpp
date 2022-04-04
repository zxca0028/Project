#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs)
    : CVIBuffer(rhs)
    , m_fInterval(rhs.m_fInterval)
    , m_iNumVerticesX(rhs.m_iNumVerticesX)
    , m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval)
{
    m_iNumVertices   = iNumVerticesX * iNumVerticesZ;
    m_iNumVerticesX  = iNumVerticesX;
    m_iNumVerticesZ  = iNumVerticesZ;
    m_fInterval      = fInterval;
    m_iStride        = sizeof(VTXNORTEX);
    m_dwVBUsage      = 0;
    m_dwFVF          = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
    m_ePrimitiveType = D3DPT_TRIANGLELIST;

    m_iNumPrimitive  = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
    m_iVPP           = 3;
    m_dwIBUsage      = 0;
    m_iIndexSize     = 4;

    if (FAILED(__super::NativeConstruct_Prototype()))
    {
        return E_FAIL;
    }

    m_pVerticesPos = new _float3[m_iNumVertices];
    ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

    VTXNORTEX* pVertices = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertices, 0);

    for (_uint i = 0; i < iNumVerticesZ; ++i)
    {
        for (_uint j = 0; j < iNumVerticesX; ++j)
        {
            _uint iIndex = i * iNumVerticesX + j;

            m_pVerticesPos[iIndex]      = _float3(j * fInterval, 0.f, i * fInterval);

            pVertices[iIndex].vPosition = _float3(j * fInterval, 0.f, i * fInterval);
            pVertices[iIndex].vNormal   = _float3(0.f, 1.f, 0.f);
			pVertices[iIndex].vTexUV    = _float2(_float(j) / (iNumVerticesX - 1), _float(i) / (iNumVerticesZ - 1));
        }
    }

    m_pVB->Unlock();

    _uint* pIndices     = 0;
    _uint  iVertexIndex = 0;

    m_pIB->Lock(0, 0, (void**)&pIndices, 0);

    for (_uint i = 0; i < iNumVerticesZ -1; ++i)
    {
        for (_uint j = 0; j < iNumVerticesX -1; ++j)
        {
            _uint iIndex = i * iNumVerticesX + j;

            pIndices[iVertexIndex++] = iIndex + iNumVerticesX;
            pIndices[iVertexIndex++] = iIndex + iNumVerticesX + 1;
            pIndices[iVertexIndex++] = iIndex + 1;

            pIndices[iVertexIndex++] = iIndex + iNumVerticesX;
            pIndices[iVertexIndex++] = iIndex + 1;
            pIndices[iVertexIndex++] = iIndex;
        }
    }

    m_pIB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void* pArg)
{
    return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval)
{
    CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ, fInterval)))
    {
        MSGBOX("Failed to Creating VIBuffer_Terrain");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
    CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating VIBuffer_Terrain");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVIBuffer_Terrain::Free()
{
    __super::Free();
}
