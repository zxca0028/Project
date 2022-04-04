#pragma once

#include "Base.h"

BEGIN(Client)

class CBlock_Manager final : public CBase
{
	DECLARE_SINGLETON(CBlock_Manager);

public:

	typedef struct tagDesc
	{
		_float3 vInitPos;
		_float3 vScale;

		_float fDeletePos;

		_int iState;

	}DESC;

private:

	explicit CBlock_Manager();
	virtual ~CBlock_Manager() = default;

public:

	HRESULT NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device);
	HRESULT Add_Block();

private:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

private:

	DESC m_Desc;

public:

	virtual void Free() override;

};

END