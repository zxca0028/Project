#include "pch.h"
#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "KeyManager.h"
#include "Camera.h"
#include "Collision_Manager.h"
#include "Light.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	__super::NativeConstruct();

	if (FAILED(Ready_Light()))												{ return E_FAIL; }
	if (FAILED(Ready_Layer_CheckPoint     (TEXT("Layer_CheckPoint"))))      { return E_FAIL; }
	if (FAILED(Ready_Layer_SkyBox		  (TEXT("Layer_SkyBox"))))			{ return E_FAIL; }
	if (FAILED(Ready_Layer_Terrain		  (TEXT("Layer_Terrain"))))			{ return E_FAIL; }
	if (FAILED(Ready_Layer_Player		  (TEXT("Layer_Player"))))			{ return E_FAIL; }
	if (FAILED(Ready_Layer_Camera		  (TEXT("Layer_Camera"))))		    { return E_FAIL; }
	if (FAILED(Ready_Layer_Weapon		  (TEXT("Layer_Weapon"))))			{ return E_FAIL; }
	if (FAILED(Ready_Layer_Structure      (TEXT("Layer_Wall_Front"))))		{ return E_FAIL; }

	//if (FAILED(Ready_Layer_Skeleton	      (TEXT("Layer_Skeleton"))))	    { return E_FAIL; }

	if (FAILED(Ready_Layer_UI	          (TEXT("Layer_UI"))))		{ return E_FAIL; }

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Flogas"), TEXT("Layer_Flogas"))))
	//{
	//	return E_FAIL;
	//}

	RELEASE_INSTANCE(CGameInstance);

	CCollision_Manager* pCollision_Manager = GET_INSTANCE(CCollision_Manager);

	pCollision_Manager->Set_Collider();

	RELEASE_INSTANCE(CCollision_Manager);

	/* LightSetting_Live */

	CLight_Manager* pLightManager = GET_INSTANCE(CLight_Manager);

	auto iter = pLightManager->m_Lights.begin();

	m_pLight = *iter;

	++iter;

	m_pLight2 = *iter;

	RELEASE_INSTANCE(CLight_Manager);

	return S_OK;
}

_int CLevel_GamePlay::Tick(_double DeltaTime)
{
	__super::Tick(DeltaTime);

	//static float fX = 1.f;
	//static float fY = -1.f;
	//static float fZ = 1.f;
	//static float fD = 1.f;
	//static float fA = 0.3f;
	//static float fS = 0.3f;
	//
	//ImGui::Begin("Light_Setting");
	//ImGui::DragFloat("X", &fX, 0.01f, -10.f, 10.f, "%.2f");
	//ImGui::DragFloat("Y", &fY, 0.01f, -10.f, 10.f, "%.2f");
	//ImGui::DragFloat("Z", &fZ, 0.01f, -10.f, 10.f, "%.2f");
	//ImGui::DragFloat("Diffuse", &fD, 0.01f, 0.f, 1.f, "%.2f");
	//ImGui::DragFloat("Ambient", &fA, 0.01f, 0.f, 1.f, "%.2f");
	//ImGui::DragFloat("Specular", &fS, 0.01f, 0.f, 1.f, "%.2f");
	//ImGui::End();
	//m_pLight->m_LightDesc.Diffuse.r = fD;
	//m_pLight->m_LightDesc.Diffuse.g = fD;
	//m_pLight->m_LightDesc.Diffuse.b = fD;
	//m_pLight->m_LightDesc.Ambient.r = fA;
	//m_pLight->m_LightDesc.Ambient.g = fA;
	//m_pLight->m_LightDesc.Ambient.b = fA;
	//m_pLight->m_LightDesc.Specular.r = fS;
	//m_pLight->m_LightDesc.Specular.g = fS;
	//m_pLight->m_LightDesc.Specular.b = fS;
	//m_pLight->m_LightDesc.Direction.x = fX;
	//m_pLight->m_LightDesc.Direction.y = fY;
	//m_pLight->m_LightDesc.Direction.z = fZ;

	CCollision_Manager* pCollision_Manager = GET_INSTANCE(CCollision_Manager);
	CKeyManager* pKey = GET_INSTANCE(CKeyManager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	//m_pLight2->m_LightDesc.Position = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
	
	pCollision_Manager->Tick(DeltaTime);
	
	RELEASE_INSTANCE(CCollision_Manager);
	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CKeyManager);

	return _int();
}

_int CLevel_GamePlay::Late_Tick(_double DeltaTime)
{
	CCollision_Manager* pCollision_Manager = GET_INSTANCE(CCollision_Manager);

	pCollision_Manager->Late_Tick();

	RELEASE_INSTANCE(CCollision_Manager);

	return _int();
}

HRESULT CLevel_GamePlay::Render()
{
	__super::Render();

	CCollision_Manager* pCollision_Manager = GET_INSTANCE(CCollision_Manager);
	
	pCollision_Manager->Render();
	
	RELEASE_INSTANCE(CCollision_Manager);

	SetWindowText(g_hWnd, TEXT("LEVEL_GAMEPLAY"));

	return S_OK;
}
	
HRESULT CLevel_GamePlay::Ready_Light()
{
	if (nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	D3DLIGHT9 LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	
	/*LightDesc.Type      = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse   = D3DXCOLOR(0.f,  0.f,  0.f,  0.f);
	LightDesc.Ambient   = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	LightDesc.Specular  = D3DXCOLOR(0.3f,  0.3f,  0.3f,  0.3f);
	LightDesc.Direction =   _float3(-0.64f,  -6.89f,  1.f);*/
	
	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.Direction = _float3(1.f, -1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pGraphic_Device, LightDesc)))
	{
		return E_FAIL;
	}

	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	
	LightDesc.Type = D3DLIGHT_POINT;
	LightDesc.Diffuse  = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	LightDesc.Ambient  = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.Position = _float3(0.f, 0.f, 0.f);
	LightDesc.Range = 10.f;
	
	if (FAILED(pGameInstance->Add_Light(m_pGraphic_Device, LightDesc)))
	{
		return E_FAIL;
	}
	//
	//ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	//
	//LightDesc.Type = D3DLIGHT_POINT;
	//LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Position = _float3(256.f, 0.f, 256.f);
	//LightDesc.Range = 150.f;
	//
	//if (FAILED(pGameInstance->Add_Light(m_pGraphic_Device, LightDesc)))
	//{
	//	return E_FAIL;
	//}
	//
	//ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	//
	//LightDesc.Type = D3DLIGHT_POINT;
	//LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Position = _float3(256.f, 0.f, 0.f);
	//LightDesc.Range = 150.f;
	//
	//if (FAILED(pGameInstance->Add_Light(m_pGraphic_Device, LightDesc)))
	//{
	//	return E_FAIL;
	//}
	//
	//ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	//
	//LightDesc.Type = D3DLIGHT_POINT;
	//LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Position = _float3(0.f, 0.f, 256.f);
	//LightDesc.Range = 150.f;
	//
	//if (FAILED(pGameInstance->Add_Light(m_pGraphic_Device, LightDesc)))
	//{
	//	return E_FAIL;
	//}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_SkyBox(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_SkyBox"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//CCamera::CAMERADESC CameraDesc;

	//_float3 vPlayerPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	////CameraDesc.vLookAt   = _float3(0.f, 0.f, 0.f);
	//CameraDesc.vLookAt = _float3(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);

	//CameraDesc.vPosition = _float3(vPlayerPos.x, vPlayerPos.y + 2.f, vPlayerPos.z - 7.f);
	////CameraDesc.vPosition = _float3(0.f, -5.f, 3.f);
	//CameraDesc.fFovy     = D3DXToRadian(60.f);
	//CameraDesc.fAspect   = (_float)g_iWinCX / g_iWinCY;
	//CameraDesc.fNear     = 0.25f;
	//CameraDesc.fFar      = 500.f;
	//
	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Camera_Free"), pLayerTag, &CameraDesc)))
	//{
	//	return E_FAIL;
	//}

	//RELEASE_INSTANCE(CGameInstance);

	CCamera::CAMERADESC CameraDesc;

	_float3 vPlayerPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	CameraDesc.vLookAt = vPlayerPos;
	CameraDesc.vPosition = _float3(vPlayerPos.x, vPlayerPos.y + 5.f, vPlayerPos.z - 7.f);
	CameraDesc.fFovy = D3DXToRadian(60.f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.25f;
	CameraDesc.fFar = 500.f;
	
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Camera_QV"), pLayerTag, &CameraDesc)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Player"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Terrain(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);	

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Terrain"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_UI"), pLayerTag)))
	{
		return E_FAIL;
	}

	for (_uint i = 0; i < 8; ++i)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_CoolDown"), pLayerTag, &i)))
		{
			return E_FAIL;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Weapon(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_uint iIndex = 0;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Weapon"), pLayerTag, &iIndex))) { return E_FAIL; }

	iIndex = 1;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Weapon"), pLayerTag, &iIndex))) { return E_FAIL; }

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Test(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Test"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Structure(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_int iStructureIndex;

	iStructureIndex =  1; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex =  2; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex =  3; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex =  4; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex =  6; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex =  7; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex =  8; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex =  9; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 10; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 11; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 12; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 13; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 14; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 15; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 16; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 17; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 18; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 19; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 20; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 21; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 22; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 23; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 24; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 25; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 26; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 27; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }
	iStructureIndex = 28; if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Structure"), pLayerTag, &iStructureIndex))) { return E_FAIL; }

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_NPC(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_NPC"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Icon(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Icon"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_CheckPoint(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (_uint i = 1; i <= 12; ++i)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_CheckPoint"), pLayerTag, &i))) { return E_FAIL; }
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Skeleton(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//_float3 vSpawnPos = _float3(5.f, -3.f, 5.f);
	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
	/*Prototype_Skeleton*/
	for (_uint i = 0; i < 5; ++i)
	{
		_float4 vSpawnPos = _float4(-2.65f + 0.79625 * 2*i, -3.f, 1.2f + 0.79625 * 2 * i, 45.f);
		if (i < 2)
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}
		else
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}
		
		vSpawnPos = _float4(-0.725f + 0.792083 * 2 * i, -3.f, -0.675 + 0.79625 * 2 * i, 45.f);
		if (i < 2)
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}
		else
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}

		vSpawnPos = _float4(1.2f + 0.79625 * 2 * i, -3.f, -2.55f + 0.79625 * 2 * i, 45.f);
		if (i < 2)
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}
		else
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}

		vSpawnPos = _float4(16.46f - 0.79625 * 2 * i, -3.f, 20.31f - 0.79625 * 2 * i, 225.f);
		if (i < 2)
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}
		else
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}

		vSpawnPos = _float4(18.285f - 0.792083 * 2 * i, -3.f, 18.335f - 0.79625 * 2 * i, 225.f);
		if (i < 2)
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}
		else
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}

		vSpawnPos = _float4(20.11f - 0.79625 * 2 * i, -3.f, 16.36f - 0.79625 * 2 * i, 225.f);
		if (i < 2)
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}
		else
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
		}
		//vSpawnPos = _float4(19.00f + 0.66f * i, -4.f + -0.4f * i, 25.00f + 2.66f * i, 120.f);
		//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
		//
		//vSpawnPos = _float4(23.00f + 0.78f * i, -4.f + -0.4f * i, 24.00f + 2.67f * i, 300.f);
		//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
	}
	//for (_uint i = 0; i < 5; ++i)
	//{
	//	_float4 vSpawnPos = _float4(19.00f + 0.66f * i, -4.f + -0.4f * i, 25.00f + 2.66f * i, 120.f);
	//	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Normal"), &vSpawnPos);
	//
	//	vSpawnPos = _float4(23.00f + 0.78f * i, -4.f + -0.4f * i, 24.00f + 2.67f * i, -60.f);
	//	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Normal"), &vSpawnPos);
	//}

	//for (_uint i = 0; i < 5; ++i)
	//{
	//	_float4 vSpawnPos = _float4(19.00f + 0.66f * i, -4.f + -0.4f * i, 25.00f + 2.66f * i, 120.f);
	//	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Normal"), &vSpawnPos);
	//
	//	vSpawnPos = _float4(23.00f + 0.78f * i, -4.f + -0.4f * i, 24.00f + 2.67f * i, -60.f);
	//	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Ghost_Soldier"), TEXT("Layer_Normal"), &vSpawnPos);
	//}

	/* /////////////////////////////////////////////////////////////////////////////////////////// */
	
	//for (_uint i = 0; i < 10; i += 2)
	//{
	//	_float4 vSpawnPos = _float4(16.46f - 0.79625 * i, -3.f, 20.31f - 0.79625 * i, 225.f);
	//	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
	//
	//	vSpawnPos = _float4(18.285f - 0.792083 * i, -3.f, 18.335f - 0.79625 * i, 225.f);
	//	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
	//
	//	vSpawnPos = _float4(20.11f - 0.79625 * i, -3.f, 16.36f - 0.79625 * i, 225.f);
	//	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Skeleton"), TEXT("Layer_Skeleton"), &vSpawnPos);
	//}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
