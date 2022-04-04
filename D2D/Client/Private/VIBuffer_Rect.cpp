#include "pch.h"
#include "VIBuffer_Rect.h"

BEGIN(Client)

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

HRESULT CVIBuffer_Rect::NativeConstruct()
{
#pragma region VERTICES
	m_iNumVertices   = 4;
	m_iStride        = sizeof(VTXTEX);
	m_dwVBUsage      = 0;
	m_dwFVF			 = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
#pragma endregion

#pragma region INDICES
	m_iNumPrimitive = 2;
	m_iVPP			= 3;
	m_dwIBUsage     = 0;
	m_iIndexSize    = 4;
#pragma endregion

	if (FAILED(__super::NativeConstruct()))
	{
		return E_FAIL;
	}

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f,  0.5f, 0.f);
	pVertices[0].vTexUV    = _float2( 0.0f,  0.0f);

	pVertices[1].vPosition = _float3( 0.5f,  0.5f, 0.f);
	pVertices[1].vTexUV    = _float2( 1.0f,  0.0f);

	pVertices[2].vPosition = _float3( 0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV    = _float2( 1.0f,  1.0f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV    = _float2( 0.0f,  1.0f);		

	m_pVB->Unlock();

	_uint*	pIndices = 0;

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

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating VIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}

END