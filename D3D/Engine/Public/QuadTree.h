#pragma once

#include "Base.h"

BEGIN(Engine)

class CQuadTree final : public CBase
{
public:

	enum CORNER   { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END };
	enum CHILD    { CHILD_LT,  CHILD_RT,  CHILD_RB,  CHILD_LB,  CHILD_END };
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };

private:

	explicit CQuadTree();
	virtual ~CQuadTree() = default;

public:

	HRESULT NativeConstruct(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	HRESULT Culling(class CFrustum* pFrustum, _float3* pVerticesPosition, _ulong* pIndices, _ulong& iIndex);
	HRESULT SetUp_Neighbor();

private:

	CQuadTree* m_pChild[CHILD_END] = { nullptr,nullptr,nullptr,nullptr };
	_uint      m_iCornerIndex[CORNER_END];
	_uint      m_iCenterIndex;
	_float     m_fRadius = 0.f;
	CQuadTree* m_pNeighbor[NEIGHBOR_END] = { nullptr,nullptr,nullptr,nullptr };

private:

	_bool LevelofDetail(_float3* pVerticesPos);

public:

	static  CQuadTree* Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	virtual void Free() override;
};

END