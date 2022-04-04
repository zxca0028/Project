#pragma once

#include "Camera.h"

BEGIN(Client)

class CCamera_3D final : public CCamera
{
private:

	explicit CCamera_3D(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_3D(const CCamera_3D& rhs);
	virtual ~CCamera_3D() = default;

public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg)override;
	virtual _int    Tick(_double DeltaTime)override;
	virtual _int    Late_Tick(_double DeltaTime)override;
	virtual HRESULT Render()override;

private:

	HRESULT Add_Components();
	void	ImGui_Camera();

private:

	_float m_fCameraSpeed = 1.f;
	_float m_fCameraDist = 3.333f;
	_float m_fCameraAngleXZ = 0.f;
	_float m_fCameraAngleYZ = 0.f;

	_float m_fCameraPosY = 2.f;
	_float m_fCameraPosZ = 0.f;

	_bool m_bLock = false;

private:

	class CTransform* m_pPlayerTransform = nullptr;

public:

	static  CCamera_3D*   Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*  Clone(void* pArg = nullptr)override;
	virtual void          Free() override;

};

END