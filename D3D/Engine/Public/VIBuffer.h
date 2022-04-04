#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:

	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:

	const _uint Get_NumPloygons() const;
	const void  Get_PolygonVertexPositions(_uint iPolygonIndex, _float3 * pPoint) const;

public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render_VIBuffer();

protected:

	LPDIRECT3DVERTEXBUFFER9 m_pVB			 = nullptr;
	LPDIRECT3DINDEXBUFFER9  m_pIB			 = nullptr;

	D3DPRIMITIVETYPE		m_ePrimitiveType = {};

	_uint					m_iStride		 = 0;
	_uint					m_iNumVertices	 = 0;
	_ulong					m_dwVBUsage		 = 0;
	_ulong					m_dwFVF			 = 0;
	_float3*				m_pVerticesPos   = nullptr;

	_uint					m_iNumPrimitive  = 0;
	_uint				    m_iVPP			 = 0;
	_uint					m_iIndexSize	 = 0;
	_ulong					m_dwIBUsage		 = 0;
	void*					m_pIndices		 = nullptr;

public:

	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void		Free() override;
	
};

END