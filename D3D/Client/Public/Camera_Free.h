#pragma once

#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{
private:

	explicit CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg)override;
	virtual _int    Tick(_double DeltaTime)override;
	virtual _int    Late_Tick(_double DeltaTime)override;
	virtual HRESULT Render()override;
	
private:

	HRESULT Add_Components();

private:

	void ImGui_Camera();

private:

	_float m_fCameraSpeed   = 1.f;
	_float m_fCameraDist    = 2.f;
	_float m_fCameraAngleXZ = 0.f;
	_float m_fCameraAngleYZ = 0.f;

	_bool  m_bLock			= false;

public:

	static  CCamera_Free* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*  Clone(void* pArg = nullptr)override;
	virtual void          Free() override;

};

END