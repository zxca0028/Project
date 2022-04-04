#pragma once

#include "VIBuffer.h"

BEGIN(Client)

class CVIBuffer_Rect final : public CVIBuffer
{
private:

	explicit CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CVIBuffer_Rect() = default;

public:

	virtual HRESULT NativeConstruct() override;

public:

	static  CVIBuffer_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END