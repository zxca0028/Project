#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube2 final : public CVIBuffer
{
protected:

	explicit CVIBuffer_Cube2(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Cube2(const CVIBuffer_Cube2& rhs);
	virtual ~CVIBuffer_Cube2() = default;

public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:

	static CVIBuffer_Cube2* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*     Clone(void* pArg = nullptr) override;
	virtual void		    Free() override;

};

END