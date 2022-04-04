#include "pch.h"
#include "CheckPoint.h"
#include "GameInstance.h"
#include "KeyManager.h"
#include "Collision_Manager.h"

CCheckPoint::CCheckPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CCheckPoint::CCheckPoint(const CCheckPoint& rhs)
	: CGameObject(rhs)
{
}

HRESULT CCheckPoint::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCheckPoint::NativeConstruct(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CCheckPoint::Tick(_double DeltaTime)
{
	//static float Angle = 0.f;
	//
	//_float3 vLook = _float3(0.f, 0.f, 1.f);
	//_float3 vMyLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	//_float3 vMyUp   = m_pTransform->Get_State(CTransform::STATE_UP);
	//
	//D3DXVec3Normalize(&vLook, &vLook);
	//D3DXVec3Normalize(&vMyLook, &vMyLook);
	//
	//_float cosine = D3DXVec3Dot(&vMyLook, &vLook);
	//Angle = acos(cosine) * (180 / 3.14f);
	//
	//ImGui::Begin("CheckPoint");
	//
	//ImGui::Text("Position");
	//ImGui::SliderFloat("X", &vPos.x, -256.f, 256.f, "%.2f");
	//ImGui::SliderFloat("Y", &vPos.y, -256.f, 256.f, "%.2f");
	//ImGui::SliderFloat("Z", &vPos.z, -256.f, 256.f, "%.2f");
	//ImGui::Text("X : %.2f, Y : %.2f, Z : %.2f", vPos.x, vPos.y, vPos.z);
	//ImGui::Separator();
	//ImGui::Text("Scale");
	//ImGui::SliderFloat("##X", &vScale.x, -10.f, 10.f, "%.2f");
	//ImGui::SliderFloat("##Y", &vScale.y, -10.f, 10.f, "%.2f");
	//ImGui::SliderFloat("##Z", &vScale.z, -10.f, 10.f, "%.2f");
	//ImGui::Text("X : %.2f, Y : %.2f, Z : %.2f", vScale.x, vScale.y, vScale.z);
	//ImGui::Separator();
	//ImGui::Text("Angle : %.2f", Angle);
	//ImGui::End();
	//
	//if (CKeyManager::Get_Instance()->Key_Pressing(KEY_Z))
	//{
	//	m_pTransform->Rotation_Axis(vMyUp, DeltaTime);
	//}
	//if (CKeyManager::Get_Instance()->Key_Pressing(KEY_C))
	//{
	//	m_pTransform->Rotation_Axis(vMyUp, -DeltaTime);
	//}
	//
	//m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
	//m_pCollider->Set_Collider(vScale, _float3(0.f,0.f,0.f));
	//m_pCollider->Update_Collider();

	return _int();
}

_int CCheckPoint::Late_Tick(_double DeltaTime)
{
	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
	{
		return E_FAIL;
	}

	return _int();
}

HRESULT CCheckPoint::Render()
{
	return S_OK;
}

HRESULT CCheckPoint::Add_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer))) { return E_FAIL; }

	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fSpeedPerSec    = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(30.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
	
	CCollider* pCollider = nullptr;

	_uint iIndex;
	memcpy(&iIndex, pArg, sizeof(_uint));

	CCollider::COLLIDERDESC ColliderDesc;
	ColliderDesc.pParentMatrix = m_pTransform->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(8.f, 3.f, 1.f);
	ColliderDesc.vInitPos = _float3(0.f, 0.f, 0.f);

	if (iIndex < 4)
	{
		m_pTransform->Rotation_Axis_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(15.f));
	}
	else if (iIndex < 6 || iIndex == 12)
	{
		m_pTransform->Rotation_Axis_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(45.f));
	}
	else
	{
		m_pTransform->Rotation_Axis_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-45.f));
	}

	if (iIndex == 1)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(20.55f, 1.5f, 21.8f));
	}
	else if (iIndex == 2)
	{
		//ColliderDesc.vScale   = _float3(6.5f, 5.f, 1.f);
		//ColliderDesc.vInitPos = _float3(0.f, 2.f, 0.f);

		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(22.f, 1.5f, 28.f));		
	}
	else if (iIndex == 3)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(28.f, 1.5f, 50.5f));
	}
	else if (iIndex == 4)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(34.3f, 1.5f, 58.f));
	}
	else if (iIndex == 5)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(61.17f, 11.f, 85.12f));
	}
	else if (iIndex == 6)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(67.47f, 11.f, 85.87f));
	}
	else if (iIndex == 7)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(83.5f, 11.f, 70.f));
	}
	else if (iIndex == 8)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(61.51f, 11.f, 91.91f));
	}
	else if (iIndex == 9)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(57.61f, 11.f, 95.96f));
	}
	else if (iIndex == 10)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(49.61f, 11.f, 104.f));
	}
	else if (iIndex == 11)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(28.26f, 21.95f, 125.23f));
	}
	else if (iIndex == 12)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(18.95f, 1.5f, 19.00f));
	}

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&pCollider, &ColliderDesc))) { return E_FAIL; }

	CCollision_Manager* pCollision_Manager = GET_INSTANCE(CCollision_Manager);
	
	_tchar* szTag = new _tchar[MAX_PATH];
	_tchar szIndex[MAX_PATH] = TEXT("");

	ZeroMemory(szTag, sizeof(MAX_PATH));

	lstrcpy(szTag, TEXT("CheckPoint"));

	if (iIndex == 12)
	{
		iIndex = 0;
	}

	_itow_s(iIndex, szIndex, 10);

	wcscat(szTag, szIndex);

	pCollision_Manager->Add_Collider(szTag, pCollider);
	Safe_Release(pCollider);
	 
	pCollision_Manager->Update_Collider(szTag);
	
	RELEASE_INSTANCE(CCollision_Manager);

	return S_OK;
}

CCheckPoint* CCheckPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCheckPoint* pInstance = new CCheckPoint(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CheckPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCheckPoint::Clone(void* pArg)
{
	CCheckPoint* pInstance = new CCheckPoint(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CheckPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCheckPoint::Free()
{
	__super::Free();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
}
