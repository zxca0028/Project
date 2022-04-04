#include "pch.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Player.h"
#include "Terrain.h"
#include "Camera_Free.h"
#include "Camera_3D.h"
#include "Camera_QV.h"
#include "Sky.h"
#include "UI.h"
#include "Weapon.h"
#include "Test.h"
#include "Flogas.h"
#include "Ghost_Soldier.h"
#include "Monster_Test.h"
#include "Structure.h"
#include "Emotion.h"
#include "CheckPoint.h"
#include "Fade.h"
#include "CoolDown.h"
#include "Icon.h"
#include "Skeleton.h"
#include "BossGauge.h"
#include "PlayerGauge.h"

#include "Effect_Normal.h"
#include "Effect_Lightning.h"
#include "Effect_Flame.h"
#include "Effect_FlameParticle.h"
#include "Effect_FireExplosion.h"
#include "Effect_Arcane.h"
#include "Effect_FinalFire.h"
#include "Effect_Crack.h"
#include "Effect_Spark.h"
#include "Effect_Electric.h"
#include "Effect_Explosion.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

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
	CLoader* pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->m_CS);

	HRESULT hr = 0;

	switch (pLoader->m_eNextLevel)
	{
	case LEVEL_GAMEPLAY:
		hr = pLoader->Ready_Level_GamePlay();
		break;
	}

	if (FAILED(hr))
	{
		LeaveCriticalSection(&pLoader->m_CS);
		return E_FAIL;
	}

	LeaveCriticalSection(&pLoader->m_CS);

	return 0;
}

HRESULT CLoader::Ready_Level_GamePlay()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_OBJECT
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Player"),        CPlayer::Create(m_pGraphic_Device))))        { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Terrain"),       CTerrain::Create(m_pGraphic_Device))))       { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_SkyBox"),        CSky::Create(m_pGraphic_Device))))           { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Camera_Free"),   CCamera_Free::Create(m_pGraphic_Device))))   { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Camera_QV"),     CCamera_QV::Create(m_pGraphic_Device))))     { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Weapon"),        CWeapon::Create(m_pGraphic_Device))))        { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Flogas"),	    CFlogas::Create(m_pGraphic_Device))))        { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Ghost_Soldier"), CGhost_Soldier::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Skeleton"),	    CSkeleton::Create(m_pGraphic_Device))))		 { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Structure"),     CStructure::Create(m_pGraphic_Device))))     { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_UI"),		    CUI::Create(m_pGraphic_Device))))		     { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_CoolDown"),	    CCoolDown::Create(m_pGraphic_Device))))	     { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Emotion"),       CEmotion::Create(m_pGraphic_Device))))	     { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_CheckPoint"),    CCheckPoint::Create(m_pGraphic_Device))))    { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Fade"),	        CFade::Create(m_pGraphic_Device))))			 { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Icon"),		    CIcon::Create(m_pGraphic_Device))))			 { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Test"),			CTest::Create(m_pGraphic_Device))))			 { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_BossGauge"),		CBossGauge::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_PlayerGauge"),	CPlayerGauge::Create(m_pGraphic_Device))))     { return E_FAIL; }
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Monster_Test"),  CMonster_Test::Create(m_pGraphic_Device)))) { return E_FAIL; }

#pragma endregion

#pragma region EFFECT_OBJECT
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Trail"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/trail%d.dds"), 2)))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_Normal"), CEffect_Normal::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Normal"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/Hit/%d.png"), 16)))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_Lightning"), CEffect_Lightning::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Lightning"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/FX_Lightning_001_TEX_KKJ/%d.png"), 16)))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_Flame"), CEffect_Flame::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Flame"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/Flame/%d.png"), 32)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_FlameFilter"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/Flame/Filter0.tga"))))) { return E_FAIL; }
	
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_FlameParticle"), CEffect_FlameParticle::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_FlameParticle"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/Flame_Particle/%d.png"), 61)))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_FireExplosion"), CEffect_FireExplosion::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_FireExplosion"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/FireExplosion/%d.tga"), 58)))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_Arcane"), CEffect_Arcane::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Arcane"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/Arcane/Arcane_%d.png"), 42)))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_FinalFire"), CEffect_FinalFire::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_FinalFire"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/FX_fumefire_003_TEX_KJS/%d.png"), 36)))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_Crack"), CEffect_Crack::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Crack"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Crack.png"))))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_Spark"), CEffect_Spark::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Spark"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/Spark/Spark%d.png"), 4)))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_Electric"), CEffect_Electric::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Electric"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/Lightning_B2/lightning_%d.tga"), 8)))) { return E_FAIL; }

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Effect_Explosion"), CEffect_Explosion::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Explosion"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Effect/Texture/Explosion/Explosion_R_%d.tga"), 16)))) { return E_FAIL; }

#pragma endregion

#pragma region PROTOTYPE_COMPONENT
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_VIBuffer_Cube"),    CVIBuffer_Cube::Create(m_pGraphic_Device)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Terrain/Height512.bmp"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_OBB"),	  CCollider::Create(m_pGraphic_Device, CCollider::TYPE_OBB))))  { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_AABB"),	  CCollider::Create(m_pGraphic_Device, CCollider::TYPE_AABB)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_Sphere"),  CCollider::Create(m_pGraphic_Device, CCollider::TYPE_SPHERE)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Navigation"),		  CNavigation::Create(m_pGraphic_Device, TEXT("../Bin/DataFiles/Navigation.dat"))))) { return E_FAIL; }
#pragma endregion

#pragma region DYNAMIC_MESH
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Player"),  CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/DynamicMesh/Unit/Elf_00/"),		TEXT("Elf_00.X")))))     { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Flogas"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/DynamicMesh/Unit/Flogas_00/"), TEXT("Flogas_00.X"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_GhostSoldier"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/DynamicMesh/Unit/Ghost_Soldier_00/"), TEXT("Ghost_Soldier_00.X"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Skeleton"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/DynamicMesh/Unit/Skeleton_00/"), TEXT("Skeleton_00.X"))))) { return E_FAIL; }
#pragma endregion

#pragma region STATIC_MESH
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Weapon"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Item/DGR_00/"), TEXT("DGR_00.X"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon16"),  CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon16.X")))))   { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon17"),  CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon17.X")))))   { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon18"),  CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon18.X")))))   { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon53"),  CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon53.X")))))   { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon54"),  CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon54.X")))))   { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon56"),  CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon56.X"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon73"),  CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon73.X")))))   { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon134"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon134.X")))))  { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon136"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon136.X")))))  { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon141"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon141.X")))))  { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon142"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon142.X")))))  { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon187"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon187.X")))))  { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon216"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), TEXT("Hieracon216.X"))))) { return E_FAIL; }
#pragma endregion

#pragma region PROTOTYPE_TEXTURE
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Terrain"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resource/Bless/Map/Tile/Tile_D_4.tga"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Terrain_N"),  CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D,    TEXT("../Bin/Resource/Bless/Map/Tile/Tile_N_4.tga")))))   { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_³îÀÚ¿¡¿ä"),  CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D,   TEXT("../Bin/Resource/11.png")))))         { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_¸Ó¾³ÇØ¿ä"),  CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D,   TEXT("../Bin/Resource/22.png")))))         { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_¿ôÇÁ³×¿ä"),  CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D,   TEXT("../Bin/Resource/33.png")))))         { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_ÃßÃµÀÌ¿ä"),  CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D,   TEXT("../Bin/Resource/44.png"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_¸ô?·ç"),     CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D,   TEXT("../Bin/Resource/MolRu/Frame%d.png"), 14))))         { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_UI"),        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_UI,   TEXT("../Bin/Resource/UI.png")))))	   { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Icon"),      CTexture::Create(m_pGraphic_Device, CTexture::TYPE_UI,   TEXT("../Bin/Resource/QuestIcon.png"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Avoid"),     CTexture::Create(m_pGraphic_Device, CTexture::TYPE_UI,   TEXT("../Bin/Resource/Avoid.png"))))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_HPGauge"),   CTexture::Create(m_pGraphic_Device, CTexture::TYPE_UI,   TEXT("../Bin/Resource/Bless/UI/HPGauge/HPGauge_%d.png"), 5)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_BossGauge"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_UI, TEXT("../Bin/Resource/Bless/UI/BossGauge/BossGauge_%d.png"), 4)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_PlayerGauge"),   CTexture::Create(m_pGraphic_Device, CTexture::TYPE_UI,   TEXT("../Bin/Resource/Bless/UI/StatusGauge/StatusGauge_%d.png"), 8)))) { return E_FAIL; }
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Skybox"),    CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resource/Skybox_0.dds"))))) { return E_FAIL; }
#pragma endregion																																			

	

	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Shader_BossGauge"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_BossGauge.hlsl"))))) { return E_FAIL; }

	m_isFinish = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLoader* CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Creating CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pGraphic_Device);

	CloseHandle(m_hThread);
}
