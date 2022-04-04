#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:

	typedef struct tagNavigationDesc
	{
		_uint iCurrentCellIndex = 0;

	}NAVIDESC;

private:

	explicit CNavigation(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:

	HRESULT NativeConstruct_Prototype(const _tchar * pDataFiles);
	HRESULT NativeConstruct(void* pArg);

	HRESULT SetUp_Neighbor();
	HRESULT Update_WorldMatrix(const _matrix* pWorldMatrix);
	bool    Move_OnNavigation(_float3 vPosition, D3DXPLANE* pPlane);

#ifdef _DEBUG
public:
	HRESULT Render(const _matrix * pWorldMatrix);
	HRESULT Render();
#endif // _DEBUG

private:

	vector<class CCell*>			m_Cells;
	typedef vector<class CCell*>	CELLS;

public:

	CELLS* Get_Cells()
	{
		return &m_Cells;
	}

	_uint Get_CurrentCellIndex()
	{
		return m_iCurrentCellIndex;
	}

private:

	NAVIDESC m_NaviDesc;
	_matrix  m_WorldMatrix;

	_uint m_iCurrentCellIndex = 0;

public:

	static  CNavigation* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pDataFiles);
	virtual CNavigation* Clone(void* pArg);
	virtual void Free() override;

};

END