#include "PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
    D3DXMatrixIdentity(&m_ViewMatrix);
    D3DXMatrixIdentity(&m_ProjMatrix);
}

_matrix CPipeLine::Get_Transform(D3DTRANSFORMSTATETYPE eStateType)
{
    return eStateType == D3DTS_VIEW ? m_ViewMatrix : m_ProjMatrix;
}

void CPipeLine::Set_Transform(D3DTRANSFORMSTATETYPE eStateType, const _matrix& TransformMatrix)
{
    if (eStateType == D3DTS_VIEW)
    {
        m_ViewMatrix = TransformMatrix;

        _matrix ViewMatrixInverse;

        D3DXMatrixInverse(&ViewMatrixInverse, nullptr, &m_ViewMatrix);

        memcpy(&m_vCamPos, &ViewMatrixInverse.m[3][0], sizeof(_float3));
    }
    else
    {
        m_ProjMatrix = TransformMatrix;
    }
}

_float3 CPipeLine::Get_CamPosition()
{
    return m_vCamPos;
}

void CPipeLine::Free()
{}
