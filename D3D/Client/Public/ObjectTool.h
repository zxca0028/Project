#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CPicking;
class CTransform;
END

BEGIN(Client)

class CObjectTool final : public CGameObject
{
private:

	explicit CObjectTool(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CObjectTool() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	HRESULT Create_Object();
	HRESULT Set_Objects(_double DeltaTime);
	HRESULT Set_Navigation(_double DeltaTime);

private:

	void Save_Data();
	void Save_NaviData();

private:

	LPD3DXMESH  m_pMesh		 = nullptr;
	CShader*	m_pShader	 = nullptr;
	CPicking*	m_pPicking	 = nullptr;
	CTransform* m_pTransform = nullptr;
	
	
private:

	/* System */
	DWORD  m_dwFrame = 0;
	DWORD  m_dwFrameCounter = 0;
	_float m_fTimeAcc = 0.f;

private:

	/* Map */
	vector<CTransform*> m_ObjectTransform;
	vector<int>			m_Index;

	_float3 vPos   = _float3(64.f, 0.f, 64.f);
	_float  fScale = 0.01f;
	_float  fAngle = 0.f;

	int m_iKey		   = 0;
	int m_iIndex       = 0;

	int m_iTemp		   = 0;
	int m_iObjectIndex = 0;

	bool m_bCheck	   = false;

	_float m_fDelay    = 0.f;

	_bool m_bSlow = false;

private:

	/* Navigation */
	vector<_float3> m_NaviPoints;
	_float3 vNaviPos = _float3(50.f, 10.f, 100.f);

public:

	static CObjectTool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END