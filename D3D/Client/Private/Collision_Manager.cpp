#include "pch.h"
#include "Collision_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
{
}

HRESULT CCollision_Manager::Add_Collider(const _tchar* pColliderTag, CCollider* pCollider)
{
	//f (nullptr != Find_Collider(pColliderTag))
	//
	//	return E_FAIL;
	//

	m_Colliders.emplace(pColliderTag, pCollider);

	Safe_AddRef(pCollider);

	return S_OK;
}

HRESULT CCollision_Manager::Set_Collider()
{
	m_pPlayer_Body_OBB		 = Find_Collider(TEXT("Player_Body_OBB"));
	m_pPlayer_Body_OBB_W	 = Find_Collider(TEXT("Player_Body_OBB_W"));
	m_pPlayer_Body_OBB_N	 = Find_Collider(TEXT("Player_Body_OBB_N"));

	m_pFlogas_Body_OBB		 = Find_Collider(TEXT("Flogas_Body_OBB"));
	m_pFlogas_Weapon_OBB	 = Find_Collider(TEXT("Flogas_Weapon_OBB"));

	m_pCheckPoint0			  = Find_Collider(TEXT("CheckPoint0"));
	m_pCheckPoint1			  = Find_Collider(TEXT("CheckPoint1"));
	m_pCheckPoint2			  = Find_Collider(TEXT("CheckPoint2"));
	m_pCheckPoint3			  = Find_Collider(TEXT("CheckPoint3"));
	m_pCheckPoint4			  = Find_Collider(TEXT("CheckPoint4"));
	m_pCheckPoint5			  = Find_Collider(TEXT("CheckPoint5"));
	m_pCheckPoint6			  = Find_Collider(TEXT("CheckPoint6"));
	m_pCheckPoint7			  = Find_Collider(TEXT("CheckPoint7"));
	m_pCheckPoint8			  = Find_Collider(TEXT("CheckPoint8"));
	m_pCheckPoint9			  = Find_Collider(TEXT("CheckPoint9"));
	m_pCheckPoint10			  = Find_Collider(TEXT("CheckPoint10"));
	m_pCheckPoint11			  = Find_Collider(TEXT("CheckPoint11"));

	return S_OK;
}

_int CCollision_Manager::Tick(_double DeltaTime)
{
	for (auto& Pair : m_Colliders)
	{
		Pair.second->Update_Collider();
	}

	return _int();
}																						   
																						   
_int CCollision_Manager::Late_Tick()
{
	m_bRestrict_Move = false;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PART_2

	if (false == m_bTriggerLock7)
	{
		if (m_pCheckPoint7->Collision_OBB(m_pPlayer_Body_OBB))
		{
			Trigger_CheckPoint7();
		}
	}

#pragma endregion

#pragma region BOSS

	if (false == m_bTriggerLock11)
	{
		if (m_pCheckPoint11->Collision_OBB(m_pPlayer_Body_OBB))
		{
			Trigger_CheckPoint11();
		}
	}
	
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}
																						   
HRESULT CCollision_Manager::Render()													   
{																						   
#ifdef _DEBUG																			   

	if (true == m_CollisionView)
	{
		for (auto& Pair : m_Colliders)
		{
			Pair.second->Render_Debug();
		}
	}

#endif

	return S_OK;
}

HRESULT CCollision_Manager::Update_Collider(const _tchar* pColliderTag)
{
	CCollider* pCollider = Find_Collider(pColliderTag);

	if (nullptr == pCollider)
	{
		return E_FAIL;
	}

	pCollider->Update_Collider();

	return S_OK;
}

HRESULT CCollision_Manager::Trigger_CheckPoint7()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float3 vPos;

	/*for (_uint i = 0; i < 10; ++i)
	{
		vPos = _float3(73.5f - 2.41f * i, 6.5f, 75.5f + 2.46f * i);
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vPos))) { return E_FAIL; }

		vPos = _float3(75.3f - 2.41f * i, 6.5f, 77.3 + 2.43 * i);
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vPos))) { return E_FAIL; }

		vPos = _float3(77.2f - 2.40f * i, 6.5f, 79.2f + 2.40f * i);
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vPos))) { return E_FAIL; }

		vPos = _float3(77.2f - 2.40f * i, 6.5f, 79.2f + 2.40f * i);
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vPos))) { return E_FAIL; }
	}*/

	for (_uint i = 0; i < 1; ++i)
	{
		//vPos = _float3(88.6f - 1.90f * i, 6.5f, 60.9f + 1.95f * i);
		//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Ghost_Soldier"), &vPos))) { return E_FAIL; }

		/*vPos = _float3(90.5f - 1.90f * i, 6.5f, 62.2f + 2.00f * i);
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Ghost_Soldier"), &vPos))) { return E_FAIL; }

		vPos = _float3(92.4f - 1.90f * i, 6.5f, 63.7f + 2.15f * i);
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Ghost_Soldier"), &vPos))) { return E_FAIL; }*/
	}

	m_bTriggerLock7 = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CCollision_Manager::Trigger_CheckPoint11()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Flogas"), TEXT("Layer_Flogas")))) { return E_FAIL; }

	m_bTriggerLock11 = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CCollider* CCollision_Manager::Find_Collider(const _tchar* pColliderTag)
{
	auto iter = find_if(m_Colliders.begin(), m_Colliders.end(), CTagFinder(pColliderTag));

	if (iter == m_Colliders.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CCollision_Manager::Free()
{
	for (auto& Pair : m_Colliders)
	{
		Safe_Release(Pair.second);
	}

	m_Colliders.clear();
}
