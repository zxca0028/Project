#include "pch.h"
#include "Camera_QV.h"
#include "GameInstance.h"
#include "KeyManager.h"
#include "Collision_Manager.h"

CCamera_QV::CCamera_QV(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera(pGraphic_Device)
{}

CCamera_QV::CCamera_QV(const CCamera_QV& rhs)
	: CCamera(rhs)
{}

HRESULT CCamera_QV::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera_QV::NativeConstruct(void* pArg)
{
	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}

	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CCamera_QV::Tick(_double DeltaTime)
{
	//if (true == m_CameraChange)
	//{
	//	return 0; 
	//}
	CCollision_Manager* pCollision_Manager = GET_INSTANCE(CCollision_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_iPlayerCurrentCell = ((CNavigation*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Navigation")))->Get_CurrentCellIndex();

	if (m_iPlayerCurrentCell > 216 - 210)
	{
		m_bFreeSetChange = true;
	}
	else
	{
		m_bFreeSetChange = false;
	}

	if (true == m_bFreeSetChange)
	{
		Camera_FreeSet();
	}
	else
	{
		Camera_FreeSet_Normal();
	}


	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CCollision_Manager);

	return __super::Tick(DeltaTime);
}

_int CCamera_QV::Late_Tick(_double DeltaTime)
{
	__super::Late_Tick(DeltaTime);

	return _int();
}

HRESULT CCamera_QV::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CCamera_QV::Add_Components()
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

void CCamera_QV::Camera_FreeSet_Normal()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CKeyManager* pKey = GET_INSTANCE(CKeyManager);

	m_vPlayerPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(m_vPlayerPos.x, m_vPlayerPos.y + m_fY, m_vPlayerPos.z - m_fZ));
	m_pTransform->Setup_CameraAngle(_float3(1.f, 0.f, 0.f), D3DXToRadian(m_fAngle));

	//if (m_fY >= 11.f)
	//{
	//	m_bOutLock = true;
	//}
	//else
	//{
	//	m_bOutLock = false;
	//}
	//if (m_fY <= 1.5f)
	//{
	//	m_bInLock = true;
	//}
	//else
	//{
	//	m_bInLock = false;
	//}

	///* ÈÙ ´ç±â±â*/
	//if (pGameInstance->Get_MouseMoveState(CInput::WHEEL) & 0x80)
	//{
	//	if (m_bInLock == false)
	//	{
	//		m_fAngle -= 5.5f;
	//		m_fY -= 1.2f;
	//		m_fZ -= 0.3f;
	//	}
	//}
	///* ÈÙ ¹Ð±â */
	//else if (pGameInstance->Get_MouseMoveState(CInput::WHEEL) * -1 & 0x80)
	//{
	//	if (m_bOutLock == false)
	//	{
	//		m_fAngle += 5.5f;
	//		m_fY += 1.2f;
	//		m_fZ += 0.3f;
	//	}
	//}

	RELEASE_INSTANCE(CKeyManager);
	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_QV::Camera_FreeSet()
{
	m_fDeltaH   = m_iPlayerCurrentCell > 416-210 ? 1.f : (m_iPlayerCurrentCell - 216 + 210) / 200.f;

	if (m_iPlayerCurrentCell > 417 - 210)
	{
		m_fDeltaH_2 = m_iPlayerCurrentCell > 617-210 ? 1.f : (m_iPlayerCurrentCell - 417 + 210) / 200.f;
	}
	else
	{
		m_fDeltaH_2 = 0.f;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_vPlayerPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(
		m_vPlayerPos.x + m_fX + m_fDeltaH * 3.f + m_fDeltaH * 6.f - m_fDeltaH_2 * 4.f,
		m_vPlayerPos.y + m_fY - m_fDeltaH * 7.f + m_fDeltaH_2 * 8.f,
		m_vPlayerPos.z - m_fZ - m_fDeltaH * 3.f + m_fDeltaH_2 * 4.f));
	m_pTransform->Setup_CameraAngle(_float3(1.f, 0.f, 0.f), D3DXToRadian(m_fAngle - (m_fDeltaH * 50.f) + (m_fDeltaH_2 * 50.f)));

	m_pTransform->Rotation_Axis_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(m_fDeltaH * -45.f));

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_QV::Camera_FreeSet_Intro()
{
	static float fX = 0.f;
	static float fY = 0.f;
	static float fZ = 0.f;
	static float Pi = 0.f; /* Right */
	static float Ya = 0.f; /* Up */

	ImGui::Begin("Camera");
	
	ImGui::Text("X : %.2f", fX); ImGui::SameLine();
	if (ImGui::Button("+X")) { ++fX; } ImGui::SameLine();
	if (ImGui::Button("-X")) { --fX; } ImGui::SameLine();
	ImGui::InputFloat("X", &fX);

	ImGui::Text("Y : %.2f", fY); ImGui::SameLine();
	if (ImGui::Button("+Y")) { ++fY; } ImGui::SameLine();
	if (ImGui::Button("-Y")) { --fY; }ImGui::SameLine();
	ImGui::InputFloat("Y", &fY);

	ImGui::Text("Z : %.2f", fZ); ImGui::SameLine();
	if (ImGui::Button("+Z")) { ++fZ; } ImGui::SameLine();
	if (ImGui::Button("-Z")) { --fZ; }ImGui::SameLine();
	ImGui::InputFloat("Z", &fZ);

	ImGui::Text("Pi : %.2f", Pi); ImGui::SameLine();
	if (ImGui::Button("+Pi")) { ++Pi; } ImGui::SameLine();
	if (ImGui::Button("-Pi")) { --Pi; }ImGui::SameLine();
	ImGui::InputFloat("Pi", &Pi);

	ImGui::Text("Ya : %.2f", Ya); ImGui::SameLine();
	if (ImGui::Button("+Ya")) { ++Ya; } ImGui::SameLine();
	if (ImGui::Button("-Ya")) { --Ya; }ImGui::SameLine();
	ImGui::InputFloat("Ya", &Ya);

	ImGui::End();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_vPlayerPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(m_vPlayerPos.x + 1.f, m_vPlayerPos.y + 2.f, m_vPlayerPos.z - 2.f));

	m_pTransform->Setup_CameraAngle(_float3(1.f, 0.f, 0.f), D3DXToRadian(5.f));
	m_pTransform->Rotation_Axis_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-37.f));

	RELEASE_INSTANCE(CGameInstance);
}

CCamera_QV* CCamera_QV::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_QV* pInstance = new CCamera_QV(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Camera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_QV::Clone(void* pArg)
{
	CCamera_QV* pInstance = new CCamera_QV(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Camera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_QV::Free()
{
	__super::Free();
}
