#include "VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer(pGraphic_Device)
{}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& rhs)
    :CVIBuffer(rhs)
{}

HRESULT CVIBuffer_Rect::NativeConstruct_Prototype()
{
    m_iNumVertices   = 4;
	m_iStride        = sizeof(VTXTEX);
	m_dwVBUsage      = 0;
	m_dwFVF          = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	m_iNumPrimitive  = 2;
    m_iVPP           = 3;
    m_dwIBUsage      = 0;

    m_iIndexSize     = 2;

    if (FAILED(__super::NativeConstruct_Prototype()))
    {
        return E_FAIL;
    }

    VTXTEX* pVertices = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertices, 0);

    pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
    pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
    pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
    pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
    pVertices[0].vTexUV    = _float2(0.f, 0.f);
    pVertices[1].vTexUV    = _float2(1.f, 0.f);
    pVertices[2].vTexUV    = _float2(1.f, 1.f);
    pVertices[3].vTexUV    = _float2(0.f, 1.f);

    m_pVB->Unlock();

    _ushort* pIndices = nullptr;

    m_pIB->Lock(0, 0, (void**)&pIndices, 0);

    pIndices[0] = 0;
    pIndices[1] = 1;
    pIndices[2] = 2;
    pIndices[3] = 0;
    pIndices[4] = 2;
    pIndices[5] = 3;

    m_pIB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_Rect::NativeConstruct(void* pArg)
{
    return S_OK;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating CVIBuffer_Rect");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
    CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating CVIBuffer_Rect");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CVIBuffer_Rect::Free()
{
    __super::Free();
}
