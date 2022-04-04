#pragma once

#include "Component.h"

BEGIN(Client)

class CVIBuffer abstract : public CComponent
{
protected:

	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CVIBuffer() = default;

public:

	virtual HRESULT NativeConstruct() override;

public:

	virtual HRESULT Render();

protected:

	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9  m_pIB = nullptr;

	D3DPRIMITIVETYPE		m_ePrimitiveType = {};

	_uint					m_iStride	   = 0;
	_uint					m_iNumVertices = 0;
	_ulong					m_dwVBUsage    = 0;
	_ulong					m_dwFVF        = 0;

	_uint					m_iNumPrimitive = 0;
	_uint				    m_iVPP		   = 0;     /* Vertex Per Primitive */
	_uint					m_iIndexSize   = 0;		/* Size of one index*/
	_ulong					m_dwIBUsage    = 0;

public:

	virtual void Free() override;

};

END