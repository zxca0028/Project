#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer(pGraphic_Device)
{}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& rhs)
    : CVIBuffer(rhs)
{}

HRESULT CVIBuffer_Cube::NativeConstruct_Prototype()
{
	m_iNumVertices   = 8;
	m_iStride        = sizeof(VTXCUBETEX);
	m_dwVBUsage      = 0;
	m_dwFVF          = D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	m_iNumPrimitive  = 12;
	m_iVPP           = 3;
	m_dwIBUsage      = 0;

	m_iIndexSize     = 2;

	if (FAILED(__super::NativeConstruct_Prototype()))
	{
	    return E_FAIL;
	}

	VTXCUBETEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	_float3 v = _float3(0.5f, 0.5f, 0.5f);

	_float3 vNormal_0 = _float3(-1.f, 1.f, -1.f);
	_float3 vNormal_1 = _float3(1.f, 1.f, -1.f);
	_float3 vNormal_2 = _float3(1.f, -1.f, -1.f);
	_float3 vNormal_3 = _float3(-1.f, -1.f, -1.f);
	_float3 vNormal_4 = _float3(-1.f, 1.f, 1.f);
	_float3 vNormal_5 = _float3(1.f, 1.f, 1.f);
	_float3 vNormal_6 = _float3(1.f, -1.f, 1.f);
	_float3 vNormal_7 = _float3(-1.f, -1.f, 1.f);

	D3DXVec3Normalize(&vNormal_0, &vNormal_0);
	D3DXVec3Normalize(&vNormal_1, &vNormal_1);
	D3DXVec3Normalize(&vNormal_2, &vNormal_2);
	D3DXVec3Normalize(&vNormal_3, &vNormal_3);
	D3DXVec3Normalize(&vNormal_4, &vNormal_4);
	D3DXVec3Normalize(&vNormal_5, &vNormal_5);
	D3DXVec3Normalize(&vNormal_6, &vNormal_6);
	D3DXVec3Normalize(&vNormal_7, &vNormal_7);

	pVertices[0].vPosition = _float3(0.f, 1.f, 0.f) - v;
	pVertices[1].vPosition = _float3(1.f, 1.f, 0.f) - v;
	pVertices[2].vPosition = _float3(1.f, 0.f, 0.f) - v;
	pVertices[3].vPosition = _float3(0.f, 0.f, 0.f) - v;
	pVertices[0].vNormal = vNormal_0;
	pVertices[1].vNormal = vNormal_1;
	pVertices[2].vNormal = vNormal_2;
	pVertices[3].vNormal = vNormal_3;
	pVertices[0].vTexUV = pVertices[0].vPosition;
	pVertices[1].vTexUV = pVertices[1].vPosition;
	pVertices[2].vTexUV = pVertices[2].vPosition;
	pVertices[3].vTexUV = pVertices[3].vPosition;

	pVertices[4].vPosition = _float3(0.f, 1.f, 1.f) - v;
	pVertices[5].vPosition = _float3(1.f, 1.f, 1.f) - v;
	pVertices[6].vPosition = _float3(1.f, 0.f, 1.f) - v;
	pVertices[7].vPosition = _float3(0.f, 0.f, 1.f) - v;
	pVertices[4].vNormal = vNormal_4;
	pVertices[5].vNormal = vNormal_5;
	pVertices[6].vNormal = vNormal_6;
	pVertices[7].vNormal = vNormal_7;
	pVertices[4].vTexUV = pVertices[4].vPosition;
	pVertices[5].vTexUV = pVertices[5].vPosition;
	pVertices[6].vTexUV = pVertices[6].vPosition;
	pVertices[7].vTexUV = pVertices[7].vPosition;

	m_pVB->Unlock();

	_ushort* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	/* Front */
	pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
	pIndices[3] = 0; pIndices[4] = 2; pIndices[5] = 3;

	/* Back  */
	pIndices[6] = 5; pIndices[7]  = 4; pIndices[8]  = 7;
	pIndices[9] = 5; pIndices[10] = 7; pIndices[11] = 6;

	/* Left  */
	pIndices[12] = 4; pIndices[13] = 0; pIndices[14] = 3;
	pIndices[15] = 4; pIndices[16] = 3; pIndices[17] = 7;

	/* Right */
	pIndices[18] = 1; pIndices[19] = 5; pIndices[20] = 6;
	pIndices[21] = 1; pIndices[22] = 6; pIndices[23] = 2;

	/* Up    */
	pIndices[24] = 4; pIndices[25] = 5; pIndices[26] = 1;
	pIndices[27] = 4; pIndices[28] = 1; pIndices[29] = 0;

	/* Down  */
	pIndices[30] = 3; pIndices[31] = 2; pIndices[32] = 6;
	pIndices[33] = 3; pIndices[34] = 6; pIndices[35] = 7;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Cube::NativeConstruct(void* pArg)
{
    return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating CVIBuffer_Cube");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
    CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating CVIBuffer_Cube");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CVIBuffer_Cube::Free()
{
    __super::Free();
}
