#pragma once

#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_Point final : public CVIBuffer
{
private:

	explicit CVIBuffer_Point(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CVIBuffer_Point() = default;

public:

	virtual HRESULT NativeConstruct(_uint iNumInstance);
	virtual HRESULT Render()	      override;

private:

	LPDIRECT3DVERTEXBUFFER9       m_pVBInstance		  = nullptr;
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration = nullptr;

private:

	_uint m_iNumInstance = 0;

public:

	static  CVIBuffer_Point* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumInstance);
	virtual void Free() override;

};

END	