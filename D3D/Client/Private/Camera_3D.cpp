#include "pch.h"
#include "Camera_3D.h"
#include "GameInstance.h"

CCamera_3D::CCamera_3D(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera(pGraphic_Device)
{}

CCamera_3D::CCamera_3D(const CCamera_3D& rhs)
	: CCamera(rhs)
{}

HRESULT CCamera_3D::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera_3D::NativeConstruct(void* pArg)
{

	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}

	__super::NativeConstruct(pArg);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = (CTransform*)(pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

	Safe_AddRef(m_pPlayerTransform);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CCamera_3D::Tick(_double DeltaTime)
{
	//ImGui_Camera();

	// 마우스 중앙 고정
	//POINT ptPos = { g_iWinCX / 2 , g_iWinCY / 2 };
	//ClientToScreen(g_hWnd, &ptPos);
	//SetCursorPos(ptPos.x, ptPos.y);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float3 vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vPlayerNewPos = _float3(vPlayerPos.x, vPlayerPos.y + 2.f, vPlayerPos.z);

	m_fCameraAngleXZ = pGameInstance->Get_MouseMoveState(CInput::X) * DeltaTime * 0.1f;
	m_fCameraAngleYZ = pGameInstance->Get_MouseMoveState(CInput::Y) * DeltaTime * 0.1f;

	m_pTransform->Rotation_Axis(_float3(0.f, 1.f, 0.f), m_fCameraAngleXZ);

	_float3 vCameraRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);

	D3DXVec3Normalize(&vCameraRight, &vCameraRight);

	m_pTransform->Rotation_Axis(vCameraRight, m_fCameraAngleYZ);

	_float3 vCameraLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vCameraLook, &vCameraLook);

	m_fCameraDist -= pGameInstance->Get_MouseMoveState(CInput::WHEEL) * DeltaTime / 6;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPlayerNewPos - vCameraLook * m_fCameraDist);

	RELEASE_INSTANCE(CGameInstance);

	return __super::Tick(DeltaTime);
}

_int CCamera_3D::Late_Tick(_double DeltaTime)
{
	__super::Late_Tick(DeltaTime);

	return _int();
}

HRESULT CCamera_3D::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CCamera_3D::Add_Components()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CCamera_3D::ImGui_Camera()
{
	ImGui::Begin("Camera_Option");
	ImGui::Text("CamDist = %.3f", m_fCameraDist);
	ImGui::End();
}

CCamera_3D* CCamera_3D::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_3D* pInstance = new CCamera_3D(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Camera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_3D::Clone(void* pArg)
{
	CCamera_3D* pInstance = new CCamera_3D(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Camera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_3D::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerTransform);
}
