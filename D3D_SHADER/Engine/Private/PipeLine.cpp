#include "PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixIdentity(&m_ProjMatrix);
}

void CPipeLine::Set_Transform(D3DTRANSFORMSTATETYPE eStateType, const _matrix& TransformMatrix)
{
	if (eStateType == D3DTS_VIEW)
	{
		m_ViewMatrix = TransformMatrix;

		_matrix ViewMatrixInv;

		D3DXMatrixInverse(&ViewMatrixInv, nullptr, &m_ViewMatrix);

		memcpy(&m_vCamPos, &ViewMatrixInv.m[3][0], sizeof(_float3));
	}
	else
	{
		m_ProjMatrix = TransformMatrix;
	}
}

void CPipeLine::Free()
{
}


