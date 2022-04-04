#include "pch.h"
#include "VIBuffer_Point.h"

BEGIN(Client)

CVIBuffer_Point::CVIBuffer_Point(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

HRESULT CVIBuffer_Point::NativeConstruct(_uint iNumInstance)
{
#pragma region VERTICES
	m_iNumVertices   = iNumInstance;
	m_iStride        = sizeof(VTXPOINT);
	m_dwVBUsage      = 0;
	m_dwFVF			 = D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_POINTLIST;
#pragma endregion

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, m_dwVBUsage, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
	{
		return E_FAIL;
	}

	VTXPOINT* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < iNumInstance; ++i)
	{
		pVertices[i].vPosition = _float3(0.f, 0.f, 0.f);
		pVertices[i].fSize     = 1.f;
		pVertices[i].vTexUV    = _float2(0.f, 0.f);
	}

	m_pVB->Unlock();

#pragma region INSTANCEBUFFER

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXMATRIX) * iNumInstance, D3DUSAGE_DYNAMIC, D3DFVF_TEX4 | D3DFVF_TEXCOORDSIZE4(0) | D3DFVF_TEXCOORDSIZE4(1) | D3DFVF_TEXCOORDSIZE4(2) | D3DFVF_TEXCOORDSIZE4(3), D3DPOOL_DEFAULT, &m_pVBInstance, nullptr)))
	{
		return E_FAIL;
	}

	VTXMATRIX* pMatrix = nullptr;

	m_pVBInstance->Lock(0, 0, (void**)&pMatrix, 0);

	for (_uint i = 0; i < iNumInstance; ++i)
	{
		pMatrix[i].vRight    = _float4(1.f, 0.f, 0.f, 0.f);
		pMatrix[i].vUp       = _float4(0.f, 1.f, 0.f, 0.f);
		pMatrix[i].vLook	 = _float4(0.f, 0.f, 1.f, 0.f);
		pMatrix[i].vPosition = _float4(150.f * i, 150.f * i, 0.f, 1.f);
	}

	m_pVBInstance->Unlock();

	D3DVERTEXELEMENT9 Decl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE,    0 },		
		{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },			
		{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },

		D3DDECL_END()
	};

	if (FAILED(m_pGraphic_Device->CreateVertexDeclaration(Decl, &m_pVertexDeclaration)))
	{
		return E_FAIL;
	}

	m_iNumInstance = iNumInstance;

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Point::Render()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);

	_uint iFreq = 0;

	m_pGraphic_Device->GetStreamSourceFreq(0, &iFreq);
	m_pGraphic_Device->SetStreamSourceFreq(0, 1);

	m_pGraphic_Device->SetStreamSource(1, m_pVBInstance, 0, sizeof(VTXMATRIX));
	m_pGraphic_Device->SetStreamSourceFreq(1, 1);

	m_pGraphic_Device->SetVertexDeclaration(m_pVertexDeclaration);

	m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumInstance);

	m_pGraphic_Device->SetStreamSourceFreq(0, 1);
	m_pGraphic_Device->SetStreamSourceFreq(1, 1);

	return S_OK;
}

CVIBuffer_Point* CVIBuffer_Point::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumInstance)
{
	CVIBuffer_Point* pInstance = new CVIBuffer_Point(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(iNumInstance)))
	{
		MSGBOX("Failed to Creating VIBuffer_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Point::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);
	Safe_Release(m_pVertexDeclaration);
}

END