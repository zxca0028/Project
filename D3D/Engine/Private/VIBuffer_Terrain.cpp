#include "VIBuffer_Terrain.h"
#include "Frustum.h"
#include "Transform.h"
#include "QuadTree.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer(pGraphic_Device)
{}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs)
    :CVIBuffer(rhs)
    , m_fInterval(rhs.m_fInterval)
    , m_iNumVerticesX(rhs.m_iNumVerticesX)
    , m_iNumVerticesZ(rhs.m_iNumVerticesZ)
    , m_pQuadTree(rhs.m_pQuadTree)
{
    Safe_AddRef(m_pQuadTree);
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval)
{
    // Vertices
    m_iNumVertices   = iNumVerticesX * iNumVerticesZ;
    m_iNumVerticesX  = iNumVerticesX;
    m_iNumVerticesZ  = iNumVerticesZ;
    m_fInterval      = fInterval;
	m_iStride        = sizeof(VTXTEX);
	m_dwVBUsage      = 0;
	m_dwFVF          = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

    // Indices
	m_iNumPrimitive  = (iNumVerticesX - 1)*(iNumVerticesZ - 1) * 2;
    m_iVPP           = 3;
    m_dwIBUsage      = 0;

    /* 2 or 4*/
    m_iIndexSize = 2;

    if (FAILED(__super::NativeConstruct_Prototype()))
    {
        return E_FAIL;
    }

    m_pVerticesPos = new _float3[m_iNumVertices];
    ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

    VTXTEX* pVertices = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertices, 0);

    for (_uint i = 0; i < iNumVerticesZ; ++i)
    {
        for (_uint j = 0; j < iNumVerticesX; ++j)
        {
            _uint iIndex = i * iNumVerticesX + j;

            m_pVerticesPos[iIndex]      = _float3(j * fInterval, 0.f, i * fInterval);

            pVertices[iIndex].vPosition = _float3(j * fInterval, 0.f, i * fInterval);
            pVertices[iIndex].vTexUV    = _float2(_float(j) / (iNumVerticesX - 1) * 30.f, _float(i)/(iNumVerticesZ - 1) * 20.f);
        }
    }

    m_pVB->Unlock();

    /* m_iIndexSize == 4 라면 int형으로 쓰기*/
    /* m_iIndexSize == 2 라면 short형으로 쓰기*/
    /* 안맞으면 안됌*/
    _short* pIndices  = 0;

    _uint iVertexIndex = 0;

    m_pIB->Lock(0, 0, (void**)&pIndices, 0);

    for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
    {
        for (_uint j = 0; j < iNumVerticesX - 1; ++j)
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

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar* pHeightMapPath, _float fInterval)
{
    _uint iNumVerticesX = 0;
    _uint iNumVerticesZ = 0;

    HANDLE hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (0 == hFile)
    {
        return E_FAIL;
    }

    _ulong dwByte = 0;

    BITMAPFILEHEADER fh;
    if (!ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr))
    {
        return E_FAIL;
    }

    BITMAPINFOHEADER ih;
    if (!ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr))
    {
        return E_FAIL;
    }

    iNumVerticesX = ih.biWidth;
    iNumVerticesZ = ih.biHeight;

    _ulong* pPixel = new _ulong[iNumVerticesX * iNumVerticesZ];
    if (!ReadFile(hFile, pPixel, sizeof(_ulong) * iNumVerticesX * iNumVerticesZ, &dwByte, nullptr))
    {
        return E_FAIL;
    }

    CloseHandle(hFile);

    // Vertices
    m_iNumVertices   = iNumVerticesX * iNumVerticesZ;
    m_iNumVerticesX  = iNumVerticesX;
    m_iNumVerticesZ  = iNumVerticesZ;
    m_fInterval      = fInterval;
    m_iStride        = sizeof(VTXNORTEX);
    m_dwVBUsage      = 0;
    m_dwFVF          = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
    m_ePrimitiveType = D3DPT_TRIANGLELIST;

    // Indices
    m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
    m_iVPP          = 3;
    m_dwIBUsage     = 0;

    /* 2 or 4*/
    m_iIndexSize    = 4;

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

            m_pVerticesPos[iIndex]      = _float3(j * fInterval, (pPixel[iIndex] & 0x000000ff) * 0.28f, i * fInterval);

            pVertices[iIndex].vPosition = _float3(j * fInterval, (pPixel[iIndex] & 0x000000ff) * 0.28f, i * fInterval);
            pVertices[iIndex].vNormal   = _float3(0.f, 0.f, 0.f);
            pVertices[iIndex].vTexUV    = _float2(_float(j) / (iNumVerticesX - 1), _float(i) / (iNumVerticesZ - 1));
        }
    }  

    /* m_iIndexSize == 4 라면 int형으로 쓰기*/
    /* m_iIndexSize == 2 라면 short형으로 쓰기*/
    /* 안맞으면 안됌*/
    _uint* pIndices = 0;

    _uint iVertexIndex = 0;

    m_pIB->Lock(0, 0, (void**)&pIndices, 0);

    for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
    {
        for (_uint j = 0; j < iNumVerticesX - 1; ++j)
        {
            _uint iIndex = i * iNumVerticesX + j;

            /* 1 */pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + iNumVerticesX;
            /* 2 */pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + iNumVerticesX + 1;
            /* 3 */pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + 1;

            _float3 vNormal, v32, v21;

            v32 = pVertices[iIndex + 1].vPosition - pVertices[iIndex + iNumVerticesX + 1].vPosition;
            v21 = pVertices[iIndex + iNumVerticesX + 1].vPosition - pVertices[iIndex + iNumVerticesX].vPosition;

            D3DXVec3Cross(&vNormal, &v21, &v32);
            D3DXVec3Normalize(&vNormal, &vNormal);

            pVertices[iIndex + iNumVerticesX].vNormal     += vNormal;
            pVertices[iIndex + iNumVerticesX + 1].vNormal += vNormal;
            pVertices[iIndex + 1].vNormal                 += vNormal;

            ///////////////////////////////////////////////////////////////////////

            /* 1 */pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + iNumVerticesX;
            /* 2 */pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + 1;
            /* 3 */pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex;

            v32 = pVertices[iIndex].vPosition - pVertices[iIndex + 1].vPosition;
            v21 = pVertices[iIndex + 1].vPosition - pVertices[iIndex + iNumVerticesX].vPosition;

            D3DXVec3Cross(&vNormal, &v21, &v32);
            D3DXVec3Normalize(&vNormal, &vNormal);

            pVertices[iIndex + iNumVerticesX].vNormal += vNormal;
            pVertices[iIndex + 1].vNormal             += vNormal;
            pVertices[iIndex].vNormal                 += vNormal;
        }
    }

    for (_uint i = 0; i < iNumVerticesX * iNumVerticesZ; ++i)
    {
        D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);
    }

    m_pVB->Unlock();
    m_pIB->Unlock();

    Safe_Delete_Array(pPixel);

    m_pQuadTree = CQuadTree::Create
    (
        m_iNumVerticesX * m_iNumVerticesZ - m_iNumVerticesX,
        m_iNumVerticesX * m_iNumVerticesZ - 1, 
        m_iNumVerticesX - 1,
        0
    );

    if (nullptr == m_pQuadTree)
    {
        return E_FAIL;
    }

    if (FAILED(m_pQuadTree->SetUp_Neighbor()))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void* pArg)
{
    return S_OK;
}

D3DXPLANE CVIBuffer_Terrain::Get_Plane(_float3& vPosition)
{
    _uint iIndex = _int(vPosition.z / m_fInterval) * m_iNumVerticesX + _int(vPosition.x / m_fInterval);

    _uint iIndices[4] =
    {
        iIndex + m_iNumVerticesX,
        iIndex + m_iNumVerticesX + 1,
        iIndex + 1,
        iIndex
    };

    _float fX = vPosition.x - m_pVerticesPos[iIndices[0]].x;
    _float fZ = m_pVerticesPos[iIndices[0]].z - vPosition.z;

    D3DXPLANE Plane;

    if (fX > fZ)
    {
        D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]]);
    }
    else
    {
        D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[2]], &m_pVerticesPos[iIndices[3]]);
    }

    return Plane;
}

HRESULT CVIBuffer_Terrain::Culling(CFrustum* pFrustum, CTransform* pTransform)
{
    _ulong dwIndex = 0;

    m_pQuadTree->Culling(pFrustum, m_pVerticesPos, (_ulong*)m_pIndices, dwIndex);

    _ulong* pIndices = nullptr;

    m_pIB->Lock(0, 0, (void**)&pIndices, 0);

    memcpy(pIndices, m_pIndices, sizeof(_ulong) * dwIndex);

    m_pIB->Unlock();

    m_iNumPrimitive = dwIndex / 3;

    return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval)
{
    CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ, fInterval)))
    {
        MSGBOX("Failed to Creating CVIBuffer_Rect");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapPath, _float fInterval)
{
    CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype(pHeightMapPath, fInterval)))
    {
        MSGBOX("Failed to Creating CVIBuffer_Rect");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
    CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating CVIBuffer_Rect");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CVIBuffer_Terrain::Free()
{
    __super::Free();

    if (false == m_isClone)
    {
        Safe_Delete_Array(m_pVerticesPos);
    }

    Safe_Release(m_pQuadTree);
}
