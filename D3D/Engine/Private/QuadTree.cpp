#include "QuadTree.h"
#include "Frustum.h"
#include "PipeLine.h"

CQuadTree::CQuadTree()
{
}

HRESULT CQuadTree::NativeConstruct(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
    m_iCornerIndex[CORNER_LT] = iLT;
    m_iCornerIndex[CORNER_RT] = iRT;
    m_iCornerIndex[CORNER_RB] = iRB;
    m_iCornerIndex[CORNER_LB] = iLB;

    if (iRT - iLT == 1)
    {
        return S_OK;
    }

    m_iCenterIndex = (iLT + iRB) >> 1 ;

    _uint iTC, iRC, iBC, iLC;

    iTC = (iLT + iRT) >> 1;
    iRC = (iRT + iRB) >> 1;
    iBC = (iLB + iRB) >> 1;
    iLC = (iLT + iLB) >> 1;

    m_pChild[CHILD_LT] = CQuadTree::Create(m_iCornerIndex[CORNER_LT], iTC, m_iCenterIndex, iLC);
    m_pChild[CHILD_RT] = CQuadTree::Create(iTC, m_iCornerIndex[CORNER_RT], iRC, m_iCenterIndex);
    m_pChild[CHILD_RB] = CQuadTree::Create(m_iCenterIndex, iRC, m_iCornerIndex[CORNER_RB], iBC);
    m_pChild[CHILD_LB] = CQuadTree::Create(iLC, m_iCenterIndex, iBC, m_iCornerIndex[CORNER_LB]);

    return S_OK;
}

HRESULT CQuadTree::Culling(CFrustum* pFrustum, _float3* pVerticesPosition, _ulong* pIndices, _ulong& iIndex)
{
    if (1 == m_iCornerIndex[CORNER_RT] - m_iCornerIndex[CORNER_LT] || LevelofDetail(pVerticesPosition))
    {
        _bool isIn[4] =
        {
            pFrustum->isIn(pVerticesPosition[m_iCornerIndex[CORNER_LT]]),
            pFrustum->isIn(pVerticesPosition[m_iCornerIndex[CORNER_RT]]),
            pFrustum->isIn(pVerticesPosition[m_iCornerIndex[CORNER_RB]]),
            pFrustum->isIn(pVerticesPosition[m_iCornerIndex[CORNER_LB]])
        };

        _bool isRender[NEIGHBOR_END] = { true,true,true,true };

        if (nullptr != m_pChild[CHILD_LT])
        {
            if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
            {
                isRender[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->LevelofDetail(pVerticesPosition);
            }
            if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
            {
                isRender[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->LevelofDetail(pVerticesPosition);
            }
            if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
            {
                isRender[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->LevelofDetail(pVerticesPosition);
            }
            if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
            {
                isRender[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->LevelofDetail(pVerticesPosition);
            }
        }

		if (true == isRender[NEIGHBOR_LEFT] && true == isRender[NEIGHBOR_TOP] && true == isRender[NEIGHBOR_RIGHT] && true == isRender[NEIGHBOR_BOTTOM])
        {
            if (true == isIn[0] || true == isIn[1] || true == isIn[2])
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LT];
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RT];
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RB];
            }
            if (true == isIn[0] || true == isIn[2] || true == isIn[3])
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LT];
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RB];
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LB];
            }
            
            return S_OK;
        }

        _uint iTC = (m_iCornerIndex[CORNER_RT] + m_iCornerIndex[CORNER_LT]) / 2;
        _uint iRC = (m_iCornerIndex[CORNER_RT] + m_iCornerIndex[CORNER_RB]) / 2;
        _uint iBC = (m_iCornerIndex[CORNER_LB] + m_iCornerIndex[CORNER_RB]) / 2;
        _uint iLC = (m_iCornerIndex[CORNER_LT] + m_iCornerIndex[CORNER_LB]) / 2;

        if (true == isIn[0] || true == isIn[1] || true == isIn[2])
        {
            if (false == isRender[NEIGHBOR_TOP])
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LT];
                pIndices[iIndex++] = iTC;
                pIndices[iIndex++] = m_iCenterIndex;

                pIndices[iIndex++] = m_iCenterIndex;
                pIndices[iIndex++] = iTC;
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RT];
            }
            else
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LT];
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RT];
                pIndices[iIndex++] = m_iCenterIndex;
            }
            if (false == isRender[NEIGHBOR_RIGHT])
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RT];
                pIndices[iIndex++] = iRC;
                pIndices[iIndex++] = m_iCenterIndex;

                pIndices[iIndex++] = m_iCenterIndex;
                pIndices[iIndex++] = iRC;
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RB];
            }
            else
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RT];
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RB];
                pIndices[iIndex++] = m_iCenterIndex;
            }
        }
        if (true == isIn[0] || true == isIn[2] || true == isIn[3])
        {
            if (false == isRender[NEIGHBOR_LEFT])
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LT];
                pIndices[iIndex++] = m_iCenterIndex;
                pIndices[iIndex++] = iLC;

                pIndices[iIndex++] = iLC;
                pIndices[iIndex++] = m_iCenterIndex;
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LB];
            }
            else
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LT];
                pIndices[iIndex++] = m_iCenterIndex;
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LB];
            }
            if (false == isRender[NEIGHBOR_BOTTOM])
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LB];
                pIndices[iIndex++] = m_iCenterIndex;
                pIndices[iIndex++] = iBC;

                pIndices[iIndex++] = iBC;
                pIndices[iIndex++] = m_iCenterIndex;
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RB];
            }
            else
            {
                pIndices[iIndex++] = m_iCornerIndex[CORNER_LB];
                pIndices[iIndex++] = m_iCenterIndex;
                pIndices[iIndex++] = m_iCornerIndex[CORNER_RB];
            }
        }

        return S_OK;
    }

    _float3 vDir = pVerticesPosition[m_iCornerIndex[CORNER_LT]] - pVerticesPosition[m_iCenterIndex];
    m_fRadius = D3DXVec3Length(&vDir);

    if (true == pFrustum->isIn(pVerticesPosition[m_iCenterIndex], m_fRadius))
    {
        for (_uint i = 0; i < CHILD_END; ++i)
        {
            if (nullptr != m_pChild[i])
            {
                m_pChild[i]->Culling(pFrustum, pVerticesPosition, pIndices, iIndex);
            }
        }
    }

    return S_OK;
}

HRESULT CQuadTree::SetUp_Neighbor()
{
    if (nullptr == m_pChild[CHILD_LT])
    {
        return S_OK;
    }

    m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_RIGHT]  = m_pChild[CHILD_RT];
    m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_LB];

    m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_LEFT]   = m_pChild[CHILD_LT];
    m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_RB];

    m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_LEFT]   = m_pChild[CHILD_LB];
    m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_TOP]    = m_pChild[CHILD_RT];
                                                     
    m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_RIGHT]  = m_pChild[CHILD_RB];
    m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_TOP]    = m_pChild[CHILD_LT];

    if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
    {
        m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LT];
        m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LB];
    }
    if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
    {
        m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_LT];
        m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_RT];
    }
    if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
    {
        m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RT];
        m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RB];
    }
    if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
    {
        m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_LB];
        m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_RB];
    }

    for (_uint i = 0; i < CHILD_END; ++i)
    {
        if (nullptr != m_pChild[i])
        {
            m_pChild[i]->SetUp_Neighbor();
        }
    }

    return S_OK;
}

_bool CQuadTree::LevelofDetail(_float3* pVerticesPos)
{
    CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

    _float3 vDistance = pPipeLine->Get_CamPosition() - pVerticesPos[m_iCenterIndex];

    _float fDistance = D3DXVec3Length(&vDistance);

    _float3 vWidth = pVerticesPos[m_iCornerIndex[CORNER_LT]] - pVerticesPos[m_iCornerIndex[CORNER_RT]];

    _float fWidth = D3DXVec3Length(&vWidth);

    RELEASE_INSTANCE(CPipeLine);

    return fDistance * 0.1f > fWidth;
}

CQuadTree* CQuadTree::Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
    CQuadTree* pInstance = new CQuadTree();

    if (FAILED(pInstance->NativeConstruct(iLT, iRT, iRB, iLB)))
    {
        MSGBOX("Failed to Creating QuadTree");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CQuadTree::Free()
{
    for (_uint i = 0; i < CHILD_END; ++i)
    {
        Safe_Release(m_pChild[i]);
    }
}
