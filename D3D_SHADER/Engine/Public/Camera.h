#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:

	typedef struct tagCameraDesc
	{
		_float3 vPos;
		_float3 vLookAt;
		_float3 vAxisY = _float3(0.f, 1.f, 0.f);

		_float fFovy;
		_float fAspect;
		_float fNear;
		_float fFar;

	}CAMERADESC;

protected:

	explicit CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:

	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int    Tick(_double DeltaTime);
	virtual _int    Late_Tick(_double DeltaTime);
	virtual HRESULT Render();

protected:

	CAMERADESC        m_CameraDesc;
	class CPipeLine*  m_pPipeLine = nullptr;
	class CTransform* m_pTransform = nullptr;

protected:

	HRESULT Set_Transform();

public:

	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

};

END