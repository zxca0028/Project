#pragma once

#include "Camera.h"

BEGIN(Client)

class CCamera_QV final : public CCamera
{
private:

	explicit CCamera_QV(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_QV(const CCamera_QV& rhs);
	virtual ~CCamera_QV() = default;

public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg)override;
	virtual _int    Tick(_double DeltaTime)override;
	virtual _int    Late_Tick(_double DeltaTime)override;
	virtual HRESULT Render()override;

private:

	HRESULT Add_Components();

private:
	
	void Camera_FreeSet_Normal();
	void Camera_FreeSet();
	void Camera_FreeSet_Intro();

private:

	_float m_fX      = 0.f;
	_float m_fY		 = 11.f;//8.f;
	_float m_fZ		 = 6.f;
	_float m_fAngle  = 60.f;//50.f;

	_bool m_bInLock  = false;
	_bool m_bOutLock = false;
	_bool m_bFreeSetChange = false;

	_float3 m_vPlayerPos;

	_uint m_iPlayerCurrentCell = 0;

	_float m_fDeltaH   = 0.f;
	_float m_fDeltaH_2 = 0.f;

	_float3 vCameraPos;

	_float m_fTimeAcc = 0.f;

public:

	static  CCamera_QV* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void          Free() override;

};

END