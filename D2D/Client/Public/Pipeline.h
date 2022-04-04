#pragma once

#include "Base.h"

BEGIN(Client)

class CPipeline final : public CBase
{
	DECLARE_SINGLETON(CPipeline)

private:

	explicit CPipeline();
	virtual ~CPipeline() = default;

public:

	HRESULT NativeConstruct();
	HRESULT Set_Transform(_matrix TransformMatrix, D3DTRANSFORMSTATETYPE eType);
	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eType);

private:
	
	_matrix m_ViewMatrix;
	_matrix m_ProjMatrix;

public:

	virtual void Free() override;

};

END