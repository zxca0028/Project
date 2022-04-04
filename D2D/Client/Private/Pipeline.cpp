#include "pch.h"
#include "Pipeline.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(CPipeline)

CPipeline::CPipeline()
{
}

HRESULT CPipeline::NativeConstruct()
{
    D3DXMatrixIdentity(&m_ViewMatrix);
    D3DXMatrixIdentity(&m_ProjMatrix);

    return S_OK;
}

HRESULT CPipeline::Set_Transform(_matrix TransformMatrix, D3DTRANSFORMSTATETYPE eType)
{
    eType == D3DTS_VIEW ? m_ViewMatrix = TransformMatrix : m_ProjMatrix = TransformMatrix;

    return S_OK;
}

_matrix CPipeline::Get_Transform(D3DTRANSFORMSTATETYPE eType)
{
    return eType == D3DTS_VIEW ? m_ViewMatrix : m_ProjMatrix;
}

void CPipeline::Free()
{
}

END