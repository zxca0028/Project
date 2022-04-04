#pragma once

#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)

public:

	typedef struct tagProjDesc
	{
		_float fFovy;
		_float fAspect;
		_float fNear;
		_float fFar;

	}PROJDESC;

private:

	explicit CPipeLine();
	virtual ~CPipeLine() = default;

public:

	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eStateType)
	{
		return eStateType == D3DTS_VIEW ? m_ViewMatrix : m_ProjMatrix;
	}

	_float3 Get_CamPos()
	{
		return m_vCamPos;
	}

public:

	void Set_Transform(D3DTRANSFORMSTATETYPE eStateType, const _matrix& TransformMatrix);
	void Set_ProjDesc(CPipeLine::PROJDESC& ProjDesc)
	{
		m_ProjDesc = ProjDesc;
	}

private:

	_matrix m_ViewMatrix;
	_matrix m_ProjMatrix;

	_float3 m_vCamPos;

private:

	PROJDESC m_ProjDesc;

public:

	virtual void Free() override;

};

END