#include "pch.h"
#include "Loader.h"
#include "GameInstance.h"

#pragma region OBJECT
#include "SkyBox.h"
#include "Camera_Free.h"
#include "Terrain.h"
#include "Player.h"
#include "Sphere.h"
#pragma endregion

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
    :m_pGraphic_Device(pGraphic_Device)
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLoader::NativeConstruct(LEVEL eLevel)
{
    m_eNextLevel = eLevel;

    InitializeCriticalSection(&m_CS);

    m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

    if (0 == m_hThread)
    {
        return E_FAIL;
    }

    return S_OK;
}

unsigned __stdcall CLoader::Thread_Main(void* pArg)
{
    CLoader* pLoading = (CLoader*)pArg;

    EnterCriticalSection(&pLoading->m_CS);

    HRESULT hr = 0;

    switch (pLoading->m_eNextLevel)
    {
    case LEVEL::LEVEL_MAIN:

        hr = pLoading->Ready_Level_Main();

        break;
    }

    if (FAILED(hr))
    {
        return E_FAIL;
    }

    return 0;
}

HRESULT CLoader::Ready_Level_Main()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region OBJECT

	if (FAILED(pGameInstance->Add_GameObject_Prototype(L"Prototype_SkyBox",      CSkyBox::Create(m_pGraphic_Device))))      { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(L"Prototype_Camera_Free", CCamera_Free::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(L"Prototype_Terrain",     CTerrain::Create(m_pGraphic_Device))))     { return E_FAIL; }
    if (FAILED(pGameInstance->Add_GameObject_Prototype(L"Prototype_Player",      CPlayer::Create(m_pGraphic_Device))))      { return E_FAIL; }
    if (FAILED(pGameInstance->Add_GameObject_Prototype(L"Prototype_Sphere",      CSphere::Create(m_pGraphic_Device))))      { return E_FAIL; }

#pragma endregion

#pragma region TEXTURE

	if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Texture_SkyBox",    CTexture::Create(m_pGraphic_Device, CTexture::TYPE::TYPE_CUBE, L"../Bin/Resource/SkyBox/SkyBox.dds")))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Texture_Terrain_D", CTexture::Create(m_pGraphic_Device, CTexture::TYPE::TYPE_2D, L"../Bin/Resource/Terrain/Texture_Grass_Diffuse.png")))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Texture_Terrain_N", CTexture::Create(m_pGraphic_Device, CTexture::TYPE::TYPE_2D, L"../Bin/Resource/Terrain/Texture_Grass_Normal.png"))))  { return E_FAIL; }

#pragma endregion

#pragma region COMPONENT

	if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Transform",        CTransform::Create(m_pGraphic_Device))))                { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_VIBuffer_Cube",    CVIBuffer_Cube::Create(m_pGraphic_Device))))            { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pGraphic_Device, 17, 17)))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Collider",         CCollider::Create(m_pGraphic_Device))))                 { return E_FAIL; }

#pragma endregion

#pragma region MESH

	if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Mesh_Test", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resource/Model/Lumine/", L"Lumine.X")))) { return E_FAIL; }

#pragma endregion

#pragma region SHADER

	if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Shader_SkyBox",     CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_SkyBox.hlsl"))))     { return E_FAIL; }
    if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Shader_Terrain",    CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Terrain.hlsl"))))    { return E_FAIL; }
    if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Shader_Mesh",       CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Mesh.hlsl"))))       { return E_FAIL; }
    if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Shader_Mesh(TEST)", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Mesh(TEST).hlsl")))) { return E_FAIL; }

#pragma endregion

    RELEASE_INSTANCE(CGameInstance);

    m_bIsFinish = true;

    return S_OK;
}

CLoader* CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
    CLoader* pInstance = new CLoader(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(eNextLevel)))
    {
        MSGBOX("Failed to Create Loader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLoader::Free()
{
    WaitForSingleObject(m_hThread, INFINITE);

    DeleteCriticalSection(&m_CS);

    CloseHandle(m_hThread);

    Safe_Release(m_pGraphic_Device);
}
