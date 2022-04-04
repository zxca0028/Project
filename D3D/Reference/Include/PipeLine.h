#pragma once

#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)

private:

	explicit CPipeLine();
	virtual ~CPipeLine() = default;

public:

	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eStateType);
	void    Set_Transform(D3DTRANSFORMSTATETYPE eStateType, const _matrix& TransformMatrix);
	_float3 Get_CamPosition();

private:

	_matrix m_ViewMatrix;
	_matrix m_ProjMatrix;

	_float3 m_vCamPos;

public:

	virtual void Free() override;
	
};

END	