#include "pch.h"
#include "Camera_Free.h"
#include "GameInstance.h"
#include "KeyManager.h"

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera(pGraphic_Device)
{}

CCamera_Free::CCamera_Free(const CCamera_Free& rhs)
	: CCamera(rhs)
{}

HRESULT CCamera_Free::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera_Free::NativeConstruct(void* pArg)
{
	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}

	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CCamera_Free::Tick(_double DeltaTime)
{
	CKeyManager::Get_Instance()->Key_Update();

	//if (m_CameraChange == false)
	//{
	//	return 0;
	//}

	//ImGui_Camera();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CKeyManager*   pKey			 = GET_INSTANCE(CKeyManager);

	if (pKey->Key_Down(KEY_ALT))
	{
		if (m_bLock == false)
		{
			m_bLock = true;
		}
		else
		{
			m_bLock = false;
		}
	}

	if (m_bLock == false)
	{
		// 마우스 중앙 고정
		//POINT ptPos = { g_iWinCX / 2 , g_iWinCY / 2 };
		//ClientToScreen(g_hWnd, &ptPos);
		//SetCursorPos(ptPos.x, ptPos.y);

		if (pKey->Key_Pressing(KEY_W))
		{
			m_pTransform->Move_Front(DeltaTime * m_fCameraSpeed);
		}
		if (pKey->Key_Pressing(KEY_S))
		{
			m_pTransform->Move_Back(DeltaTime * m_fCameraSpeed);
		}
		if (pKey->Key_Pressing(KEY_A))
		{
			m_pTransform->Move_Left(DeltaTime * m_fCameraSpeed);
		}
		if (pKey->Key_Pressing(KEY_D))
		{
			m_pTransform->Move_Right(DeltaTime * m_fCameraSpeed);
		}

		m_pTransform->Rotation_Axis(_float3(0.f, 1.f, 0.f), pGameInstance->Get_MouseMoveState(CInput::X) * DeltaTime * 0.1);
		m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), pGameInstance->Get_MouseMoveState(CInput::Y) * DeltaTime * 0.1);
	}	

	RELEASE_INSTANCE(CKeyManager);
	RELEASE_INSTANCE(CGameInstance);

	return __super::Tick(DeltaTime);
}

_int CCamera_Free::Late_Tick(_double DeltaTime)
{
	__super::Late_Tick(DeltaTime);

	return _int();
}

HRESULT CCamera_Free::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CCamera_Free::Add_Components()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fSpeedPerSec    = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CCamera_Free::ImGui_Camera()
{
	ImGui::Begin("Camera_Setting");
	if (m_bLock == false)
	{
		ImGui::Text("Lock = OFF");
	}
	else
	{
		ImGui::Text("Lock = ON");
	}
	if (ImGui::Button("+Speed"))
	{
		m_fCameraSpeed += 0.5f;
	}
	ImGui::SameLine();
	if (ImGui::Button("-Speed"))
	{
		m_fCameraSpeed -= 0.5f;
	}
	ImGui::Text("Speed = x%.2f", m_fCameraSpeed);
	ImGui::End();
}

CCamera_Free* CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free* pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Camera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Camera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();	
}
