#include "pch.h"
#include "MainApp.h"

#include "GameInstance.h"
#include "Level_Logo.h"
#include "BG_Logo.h"
#include "BG_Loading.h"
#include "KeyManager.h"
#include "Collision_Manager.h"
#include "Skill_Manager.h"

#include "ObjectTool.h"
#include "Sound_Manager.h"

CMainApp::CMainApp()
{}

HRESULT CMainApp::NativeConstruct()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, g_hWnd)))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Ready_Graphic_Device(g_hWnd, CGraphic_Device::MODE_WIN, g_iWinCX, g_iWinCY, &m_pGraphic_Device)))
	{
		return E_FAIL;
	}
	if (FAILED(Create_GameObject_Prototype()))
	{
		return E_FAIL;
	}
	if (FAILED(Create_Component_Prototype()))
	{
		return E_FAIL;
	}
	/*if (FAILED(Create_NaviMeshPoint()))
	{
		return E_FAIL;
	}*/
	if (FAILED(Init_ImGui()))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Set_Level(CLevel_Logo::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}

	//m_pObjectTool = CObjectTool::Create(m_pGraphic_Device);
	//if (nullptr == m_pObjectTool)
	//{
	//	return E_FAIL;
	//}

	//CSound_Manager::Get_Instance()->Initialize();
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CMainApp::Tick(_double DeltaTime)
{
	CGameInstance*      pGameInstance      = GET_INSTANCE(CGameInstance);
	CKeyManager*        pKey		       = GET_INSTANCE(CKeyManager);

	if (FAILED(Update_ImGui())) { return E_FAIL; }

	m_pPicking->Compute_MousePosInWorld(g_hWnd);
	m_pFrustum->Update_Frustum();
	//m_pObjectTool->Tick(DeltaTime);

	pGameInstance->Tick(DeltaTime);

	RELEASE_INSTANCE(CKeyManager);
	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CMainApp::Late_Tick(_double DeltaTime)
{
	CGameInstance*		pGameInstance      = GET_INSTANCE(CGameInstance);

	pGameInstance->Late_Tick(DeltaTime);

	//CSkill_Manager::Get_Instance()->Switch_Off();

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

HRESULT CMainApp::Render_MainApp()
{
	CGameInstance*		pGameInstance	   = GET_INSTANCE(CGameInstance);

	ImGui::EndFrame();

	pGameInstance->Render_Begin();

	m_pRenderer->Render_GameObject(m_bTargetRender);
	//m_pObjectTool->Render();

	//if (FAILED(Render_ImGui()))				   { return E_FAIL; }
	if (FAILED(pGameInstance->Render_Level())) { return E_FAIL; }

	pGameInstance->Render_End();

	//pGameInstance->Delete_GameObject();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMainApp::Init_ImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	return S_OK;
}

HRESULT CMainApp::Update_ImGui()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return S_OK;
}

HRESULT CMainApp::Render_ImGui()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

void CMainApp::Clear_ImGui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

HRESULT CMainApp::Create_GameObject_Prototype()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_BG_Logo"),    CBG_Logo::Create(m_pGraphic_Device))))    { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_BG_Loading"), CBG_Loading::Create(m_pGraphic_Device)))) { return E_FAIL; }

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMainApp::Create_Component_Prototype()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pRenderer = CRenderer::Create(m_pGraphic_Device);
	m_pPicking  = CPicking::Create(m_pGraphic_Device);
	m_pFrustum  = CFrustum::Create(m_pGraphic_Device);

#pragma region PROTOTYPE_COMPONENT
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Renderer"), m_pRenderer))) { return E_FAIL; } Safe_AddRef(m_pRenderer);
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Picking"),  m_pPicking)))  { return E_FAIL; } Safe_AddRef(m_pPicking);
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Frustum"),  m_pFrustum)))  { return E_FAIL; } Safe_AddRef(m_pFrustum);
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Transform"),       CTransform::Create(m_pGraphic_Device))))     { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"),   CVIBuffer_Rect::Create(m_pGraphic_Device)))) { return E_FAIL; }
#pragma endregion

#pragma region PROTOTYPE_TEXTURE
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Texture_Logo"),    CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Logo.png")))))    { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Texture_Loading"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_UI, TEXT("../Bin/Resource/Bless/Logo/Back/Back_3.png"))))) { return E_FAIL; }
#pragma endregion

#pragma region PROTOTYPE_SHADER
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"),    CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))    { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Shader_Terrain"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Terrain.hlsl"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Shader_Mesh"),    CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Mesh.hlsl")))))    { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Shader_UI"),	  CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_UI.hlsl"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXT("Prototype_Shader_Effect"),  CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Effect.hlsl")))))      { return E_FAIL; }
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMainApp::Create_NaviMeshPoint()
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(TEXT("../Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (0 == hFile)
	{
		return E_FAIL;
	}

	_float3			vPoint[3];

	/*ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(-3.55f, 0.00f, 1.25f);
	vPoint[1] = _float3(21.92f, -0.00f, 17.46f);
	vPoint[2] = _float3(1.02f, -0.00f, -3.45f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(-3.55f, 0.00f, 1.25f);
	vPoint[1] = _float3(17.01f, 0.00f, 21.91f);
	vPoint[2] = _float3(21.92f, -0.00f, 17.46f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(17.01f, 0.00f, 21.91f);
	vPoint[1] = _float3(23.27f, -0.00f, 19.56f);
	vPoint[2] = _float3(21.92f, -0.00f, 17.46f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(17.01f, 0.00f, 21.91f);
	vPoint[1] = _float3(31.62f, -0.00f, 51.01f);
	vPoint[2] = _float3(23.27f, -0.00f, 19.56f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(17.01f, 0.00f, 21.91f);
	vPoint[1] = _float3(25.06f, 0.00f, 52.06f);
	vPoint[2] = _float3(31.62f, -0.00f, 51.01f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(25.06f, 0.00f, 52.06f);
	vPoint[1] = _float3(26.61f, 0.00f, 55.26f);
	vPoint[2] = _float3(31.62f, -0.00f, 51.01f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(26.61f, 0.00f, 55.26f);
	vPoint[1] = _float3(45.07f, -0.00f, 64.36f);
	vPoint[2] = _float3(31.62f, -0.00f, 51.01f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(26.61f, 0.00f, 55.26f);
	vPoint[1] = _float3(40.41f, 0.00f, 69.11f);
	vPoint[2] = _float3(45.07f, -0.00f, 64.36f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	_float DeltaX = 1.051f/10.f;
	_float DeltaY = 0.95f /10.f;
	_float DeltaZ = 1.05f /10.f;

	for (_uint i = 0; i < 100; ++i)
	{
		ZeroMemory(vPoint, sizeof(_float3) * 3);
		vPoint[0] = _float3(40.41f + DeltaX * i, 0.00f + DeltaY * i, 69.11f + DeltaZ * i);
		vPoint[1] = _float3(45.07f + DeltaX * (i + 1), -0.00f + DeltaY * (i + 1), 64.36f + DeltaZ * (i + 1));
		vPoint[2] = _float3(45.07f + DeltaX * i, -0.00f + DeltaY * i, 64.36f + DeltaZ * i);
		WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

		ZeroMemory(vPoint, sizeof(_float3) * 3);
		vPoint[0] = _float3(40.41f + DeltaX * i, 0.00f + DeltaY * i, 69.11f + DeltaZ * i);
		vPoint[1] = _float3(40.41f + DeltaX * (i + 1), 0.00f + DeltaY * (i + 1), 69.11f + DeltaZ * (i + 1));
		vPoint[2] = _float3(45.07f + DeltaX * (i + 1), -0.00f + DeltaY * (i + 1), 64.36f + DeltaZ * (i + 1));
		WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);
	}*/

	/*ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(50.92f, 9.50f, 79.61f);
	vPoint[1] = _float3(64.23f, 9.50f, 83.61f);
	vPoint[2] = _float3(55.58f, 9.50f, 74.86f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(50.92f, 9.50f, 79.61f);
	vPoint[1] = _float3(59.58f, 9.50f, 88.31f);
	vPoint[2] = _float3(64.23f, 9.50f, 83.61f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);*/

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(64.23f, 9.50f, 83.61f);
	vPoint[1] = _float3(94.39f, 9.50f, 64.36f);
	vPoint[2] = _float3(88.89f, 9.50f, 58.91f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(64.23f, 9.50f, 83.61f);
	vPoint[1] = _float3(69.98f, 9.50f, 88.77f);
	vPoint[2] = _float3(94.39f, 9.50f, 64.36f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);
	
	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(64.23f, 9.50f, 83.61f);
	vPoint[1] = _float3(59.58f, 9.50f, 88.31f);
	vPoint[2] = _float3(69.98f, 9.50f, 88.77f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(59.58f, 9.50f, 88.31f);
	vPoint[1] = _float3(64.62f, 9.50f, 93.92f);
	vPoint[2] = _float3(69.98f, 9.50f, 88.77f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(59.58f, 9.50f, 88.31f);
	vPoint[1] = _float3(45.97f, 9.50f, 101.97f);
	vPoint[2] = _float3(64.62f, 9.50f, 93.92f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(45.97f, 9.50f, 101.97f);
	vPoint[1] = _float3(51.32f, 9.50f, 107.27f);
	vPoint[2] = _float3(64.62f, 9.50f, 93.92f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	_float DeltaX_L = -1.075f/10.f;
	_float DeltaY_L = 0.88f  /10.f;
	_float DeltaZ_L = 1.08f  /10.f;
	_float DeltaX_R = -1.05f /10.f;
	_float DeltaY_R = 0.88f  /10.f;
	_float DeltaZ_R = 1.095f /10.f;

	_float3 vTemp[3];

	for (_uint i = 0; i < 100; ++i)
	{
		ZeroMemory(vPoint, sizeof(_float3) * 3);
		vPoint[0] = _float3(45.97f + DeltaX_L * i, 9.50f + DeltaY_L * i, 101.97f + DeltaZ_L * i);
		vPoint[1] = _float3(45.97f + DeltaX_L * (i + 1), 9.50f + DeltaY_L * (i + 1), 101.97f + DeltaZ_L * (i + 1));
		vPoint[2] = _float3(51.32f + DeltaX_R * i, 9.50f + DeltaY_R * i, 107.27f + DeltaZ_R * i);
		WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

		ZeroMemory(vPoint, sizeof(_float3) * 3);
		vPoint[0] = _float3(45.97f + DeltaX_L * (i + 1), 9.50f + DeltaY_L * (i + 1), 101.97f + DeltaZ_L * (i + 1));
		vPoint[1] = _float3(51.32f + DeltaX_R * (i + 1), 9.50f + DeltaY_R * (i + 1), 107.27f + DeltaZ_R * (i + 1));
		vPoint[2] = _float3(51.32f + DeltaX_R * i, 9.50f + DeltaY_R * i, 107.27f + DeltaZ_R * i);
		WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

		if (i == 99)
		{
			vTemp[0] = vPoint[0];
			vTemp[1] = vPoint[1];
			vTemp[2] = vPoint[2];
		}
	}

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[1] = vTemp[0];
	vPoint[2] = _float3(29.82f, 18.30f, 118.07f);
	vPoint[0] = vTemp[1];
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(29.82f, 18.30f, 118.07f);
	vPoint[1] = _float3(35.47f, 18.30f, 123.62f);
	vPoint[2] = vTemp[1];
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	DeltaX_L = -0.341f  / 10.f;
	DeltaY_L = 0.215f   / 10.f;
	DeltaZ_L = 0.345f   / 10.f;
	DeltaX_R = -0.35f   / 10.f;
	DeltaY_R = 0.215f   / 10.f;
	DeltaZ_R = 0.336f   / 10.f;
	
	_float3 vTemp2[3];

	for (_uint i = 0; i < 100; ++i)
	{
		ZeroMemory(vPoint, sizeof(_float3) * 3);
		vPoint[0] = _float3(29.82f + DeltaX_L * i, 18.30f + DeltaY_L * i, 118.07f + DeltaZ_L * i);
		vPoint[1] = _float3(29.82f + DeltaX_L * (i + 1), 18.30f + DeltaY_L * (i + 1), 118.07f + DeltaZ_L * (i + 1));
		vPoint[2] = _float3(35.47f + DeltaX_R * i, 18.30f + DeltaY_R * i, 123.62f + DeltaZ_R * i);
		WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);
	
		ZeroMemory(vPoint, sizeof(_float3) * 3);
		vPoint[0] = _float3(29.82f + DeltaX_L * (i + 1), 18.30f + DeltaY_L * (i + 1), 118.07f + DeltaZ_L * (i + 1));
		vPoint[1] = _float3(35.47f + DeltaX_R * (i + 1), 18.30f + DeltaY_R * (i + 1), 123.62f + DeltaZ_R * (i + 1));
		vPoint[2] = _float3(35.47f + DeltaX_R * i, 18.30f + DeltaY_R * i, 123.62f + DeltaZ_R * i);
		WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

		if (i == 99)
		{
			vTemp2[0] = vPoint[0];
			vTemp2[1] = vPoint[1];
			vTemp2[2] = vPoint[2];
		}
	}
	
	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = vTemp2[0];
	vPoint[1] = _float3(24.66f, 20.45f, 123.12f);
	vPoint[2] = vTemp2[1];
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);
	
	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(24.66f, 20.45f, 123.12f);
	vPoint[1] = _float3(30.37f, 20.45f, 128.58f);
	vPoint[2] = vTemp2[1];
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	/* /////////////////////////////////////////////////////////// */

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(30.37f, 20.45f, 128.58f);
	vPoint[1] = _float3(24.66f, 20.45f, 123.12f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(33.20f, 20.45f, 134.51f);
	vPoint[1] = _float3(30.37f, 20.45f, 128.58f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(33.60f, 20.45f, 139.91f);
	vPoint[1] = _float3(33.20f, 20.45f, 134.51f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(23.65f, 20.45f, 149.67f);
	vPoint[1] = _float3(33.60f, 20.45f, 139.91f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(18.95f, 20.45f, 149.57f);
	vPoint[1] = _float3(23.65f, 20.45f, 149.67f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(12.85f, 20.45f, 146.77f);
	vPoint[1] = _float3(18.95f, 20.45f, 149.57f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(6.30f, 20.45f, 140.11f);
	vPoint[1] = _float3(12.85f, 20.45f, 146.77f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(3.60f, 20.45f, 134.15f);
	vPoint[1] = _float3(6.30f, 20.45f, 140.11f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(3.20f, 20.45f, 129.40f);
	vPoint[1] = _float3(3.60f, 20.45f, 134.15f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(13.40f, 20.45f, 119.50f);
	vPoint[1] = _float3(3.20f, 20.45f, 129.40f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(18.00f, 20.45f, 119.70f);
	vPoint[1] = _float3(13.40f, 20.45f, 119.50f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoint, sizeof(_float3) * 3);
	vPoint[0] = _float3(24.66f, 20.45f, 123.12f);
	vPoint[1] = _float3(18.00f, 20.45f, 119.70f);
	vPoint[2] = _float3(19.10f, 20.45f, 135.06f);
	WriteFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating MainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Clear_ImGui();
	CKeyManager::Get_Instance()->Destroy_Instance();
	CCollision_Manager::Get_Instance()->Destroy_Instance();
	CSkill_Manager::Get_Instance()->Destroy_Instance();

	Safe_Release(m_pFrustum);
	Safe_Release(m_pPicking);
	Safe_Release(m_pObjectTool);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);

	CGameInstance::Release_Engine();
}
