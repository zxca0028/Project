#include "pch.h"
#include "Monster_Test.h"
#include "GameInstance.h"
#include "KeyManager.h"
#include "Random.h"
#include "Player.h"

CMonster_Test::CMonster_Test(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{}

CMonster_Test::CMonster_Test(const CMonster_Test& rhs)
	: CGameObject(rhs)
{}

HRESULT CMonster_Test::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CMonster_Test::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components(pArg)))
	{
		return E_FAIL;
	}

	m_pTransform->Set_Scale(_float3(0.1f, 0.1f, 0.1f));
	m_pTransform->Setup_Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));

	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(10.f, 0.f, 10.f));

	m_dwPlayerAction = 0;

	return S_OK;
}

_int CMonster_Test::Tick(_double DeltaTime)
{
	__super::Tick(DeltaTime);

	static int AniSet = 0;

	ImGui::Begin("Animation");
	ImGui::InputInt("AnimationSet", &AniSet);
	ImGui::End();

	m_pMesh->Set_AnimationIndex(AniSet);

	return _int();
}

_int CMonster_Test::Late_Tick(_double DeltaTime)
{
	__super::Late_Tick(DeltaTime);

	m_pMesh->Play_Animation(DeltaTime);

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
	{
		return E_FAIL;
	}

	return _int();
}

HRESULT CMonster_Test::Render()
{
	__super::Render();

	if (FAILED(Set_ConstTable()))
	{
		return E_FAIL;
	}

	m_pShader->Begin_Shader(0);

	_uint iNumMeshContainers = m_pMesh->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		_uint iNumMaterials = m_pMesh->Get_NumMaterials(i);

		m_pMesh->Update_SkinnedMesh(i);

		for (_uint j = 0; j < iNumMaterials; ++j)
		{
			if (FAILED(m_pMesh->Set_TextureOnShader(m_pShader, "g_tMtrlDiffuse", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i, j)))
			{
				return E_FAIL;
			}

			m_pShader->Commit();

			m_pMesh->Render(i, j);
		}
	}

	m_pShader->End_Shader();

	return S_OK;
}

HRESULT CMonster_Test::Set_ConstTable()
{
	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WrldMatrix = *m_pTransform->Get_WorldMatrix();
	_matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	_matrix ProjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

	m_pShader->Set_ConstTable("g_WorldMatrix", &WrldMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix, sizeof(_matrix));

	D3DLIGHT9 LightDesc = pGameInstance->Get_LightDesc(0);
	_float4   LightDir = _float4(LightDesc.Direction, 0.f);

	m_pShader->Set_ConstTable("g_vLightDir",      &LightDir,		   sizeof(_float4));
	m_pShader->Set_ConstTable("g_vLightDiffuse",  &LightDesc.Diffuse,  sizeof(_float4));
	m_pShader->Set_ConstTable("g_vLightSpecular", &LightDesc.Specular, sizeof(_float4));
	m_pShader->Set_ConstTable("g_vLightAmbient",  &LightDesc.Ambient,  sizeof(_float4));

	_float4 vCamPos = _float4(pGameInstance->Get_CamPosition(), 1.f);

	m_pShader->Set_ConstTable("g_vCamPos", &vCamPos, sizeof(_float4));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Test::Add_Components(void* pArg)
{ 
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fSpeedPerSec    = 2.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Transform"),         TEXT("Com_Transform"),   (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_Mesh"),       TEXT("Com_Shader_Mesh"), (CComponent**)&m_pShader)))   { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"),          TEXT("Com_Renderer"),    (CComponent**)&m_pRenderer))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Test"),		   TEXT("Com_Mesh"),        (CComponent**)&m_pMesh)))     { return E_FAIL; }

	return S_OK;
}

CMonster_Test* CMonster_Test::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster_Test* pInstance = new CMonster_Test(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Monster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Test::Clone(void* pArg)
{
	CMonster_Test* pInstance = new CMonster_Test(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Monster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Test::Free()
{
	__super::Free();

	Safe_Release(m_pMesh);
	Safe_Release(m_pShader);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
}
