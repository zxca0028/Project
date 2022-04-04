#include "pch.h"
#include "Flogas.h"
#include "GameInstance.h"
#include "KeyManager.h"
#include "Random.h"
#include "Collision_Manager.h"
#include "Skill_Manager.h"
#include "Effect_Normal.h"
#include "Trail.h"

CFlogas::CFlogas(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{}

CFlogas::CFlogas(const CFlogas& rhs)
	: CGameObject(rhs)
{}

HRESULT CFlogas::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CFlogas::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	m_pCollision_Manager = GET_INSTANCE(CCollision_Manager);
	m_pRandom = GET_INSTANCE(CRandom);

	RELEASE_INSTANCE(CRandom);
	RELEASE_INSTANCE(CCollision_Manager);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Components(pArg)))
	{
		return E_FAIL;
	}

	m_pPlayer_W = m_pCollision_Manager->Find_Collider(TEXT("Player_Body_OBB_W"));
	m_pPlayer_N = m_pCollision_Manager->Find_Collider(TEXT("Player_Body_OBB_N"));
	m_pPlayer_D = m_pCollision_Manager->Find_Collider(TEXT("Player_Body_OBB_D"));
	Safe_AddRef(m_pPlayer_W);
	Safe_AddRef(m_pPlayer_N);
	Safe_AddRef(m_pPlayer_D);

	m_pTransform->Setup_Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(180.f));
	m_pTransform->Set_Scale(_float3(0.1f, 0.1f, 0.1f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(19.11f, 20.5f, 135.07f));

	

	CTrail::TRAILINFO TrailInfo;

	TrailInfo.pWorldMatrix = m_pOriginalMatrix;
	TrailInfo.iIndex = 2;

	CTrail* pTrail = CTrail::Create(m_pGraphic_Device, &TrailInfo);
	
	if (nullptr == pTrail)
	{
		return E_FAIL;
	}
	
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Trail"), pTrail)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	m_dwActionState = BATTLE_IDLE;

	return S_OK;
}

_int CFlogas::Tick(_double DeltaTime)
{
	__super::Tick(DeltaTime);

	if (false == m_bGaugeCreateLock)
	{
		m_fTimeAcc_GaugeCreate += DeltaTime;

		if (m_fTimeAcc_GaugeCreate > 3.f)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			_float* HP = &m_fHP;

			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_BossGauge"), TEXT("Layer_BossGauge"), &HP)))
			{
				return E_FAIL;
			}

			RELEASE_INSTANCE(CGameInstance);

			m_fTimeAcc_GaugeCreate = 0.f;
			m_bGaugeCreateLock = true;
		}
	}

	if (m_dwPhase == 2)
	{
		if (false == m_bParticleCreateLock)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			_float4 vPos = _float4(m_pTransform->Get_State(CTransform::STATE_POSITION), 1.f);

			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Effect_FlameParticle"), TEXT("Layer_Effect"), &vPos)))
			{
				return E_FAIL;
			}

			RELEASE_INSTANCE(CGameInstance);

			m_bParticleCreateLock = true;
		}

		m_fTimeAcc_Flame += DeltaTime;

		if (m_fTimeAcc_Flame > 2.f)
		{
			CreateEffect(TEXT("Prototype_Effect_Flame"), 10.f);

			m_fTimeAcc_Flame = 0.f;
		}
	}

	Boss_Behavior(DeltaTime);

	Time_Initialize(DeltaTime);

	return _int();
}

_int CFlogas::Late_Tick(_double DeltaTime)
{
	__super::Late_Tick(DeltaTime);

	const _matrix* pWorldMatrix = m_pTransform->Get_WorldMatrix();

	m_OriginalMatrix = (*m_pParentBoneMatrix * *pWorldMatrix);

	_float3 vPos   = *(_float3*)m_OriginalMatrix.m[3];
	_float3 vRight = *(_float3*)m_OriginalMatrix.m[0];
	_float3 vUp    = *(_float3*)m_OriginalMatrix.m[1];

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	_float3 vDownPos = vPos + vUp *  3.f + vRight * 0.5f;
	_float3 vUpPos = vPos;// -vUp * 1.f;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (false == m_bDeleteLock)
	{
		CTrail* pTrail = (CTrail*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Trail"), 2);

		pTrail->AddNewTrail(vDownPos, vUpPos, DeltaTime);
		pTrail->UpdateTrail(DeltaTime);
	}

	RELEASE_INSTANCE(CGameInstance);

	m_pMesh->Play_Animation(DeltaTime);

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
	{
		return E_FAIL;
	}

	return _int();
}

HRESULT CFlogas::Render()
{
	__super::Render();

	if (FAILED(Set_ConstTable()))
	{
		return E_FAIL;
	}

	m_pShader->Begin_Shader(1);

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
			if (FAILED(m_pMesh->Set_TextureOnShader(m_pShader, "g_NormalDiffuse", MESHMATERIALTEXTURE::TYPE_NORMAL, i, j)))
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

HRESULT CFlogas::Set_ConstTable()
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

HRESULT CFlogas::Add_Components(void* pArg)
{
	/* Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec	  = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Transform"),   TEXT("Com_Transform"),   (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader_Mesh"), (CComponent**)&m_pShader)))   { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"),    TEXT("Com_Renderer"),    (CComponent**)&m_pRenderer))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Frustum"),     TEXT("Com_Frustum"),     (CComponent**)&m_pFrustum)))  { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Flogas"), TEXT("Com_Mesh"),		  (CComponent**)&m_pMesh)))		{ return E_FAIL; }

	m_pParentBoneMatrix = m_pMesh->Get_BoneMatrix("BN_Weapon_R");

	const _matrix* pWorldMatrix = m_pTransform->Get_WorldMatrix();

	m_OriginalMatrix = (*m_pParentBoneMatrix * *pWorldMatrix);

	m_pOriginalMatrix = &m_OriginalMatrix;

	CCollider::COLLIDERDESC ColliderDesc;
	ColliderDesc.pParentMatrix = m_pTransform->Get_WorldMatrix();
	ColliderDesc.vScale   = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vInitPos = _float3(0.f, 1.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pCollider, &ColliderDesc))) { return E_FAIL; }


	ColliderDesc.pParentMatrix = m_pOriginalMatrix;
	ColliderDesc.vScale   = _float3(1.f, 1.f, 5.f);
	ColliderDesc.vInitPos = _float3(0.f, 1.f, 0.f);
	ColliderDesc.vRadians = _float3(45.f,0.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_OBB"), TEXT("Com_OBB_Weapon"), (CComponent**)&m_pFlogasWeapon, &ColliderDesc))) { return E_FAIL; }

	if (FAILED(m_pCollision_Manager->Add_Collider(TEXT("Flogas_Body_OBB"), m_pCollider))) { return E_FAIL; }
	if (FAILED(m_pCollision_Manager->Add_Collider(TEXT("Flogas_Weapon_OBB"), m_pFlogasWeapon))) { return E_FAIL; }

	return S_OK;
}

void CFlogas::Get_PlayerDesc()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_vPlayerPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	_float3 vMyPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vLook = m_vPlayerPos - vMyPos;

	m_fDistance = D3DXVec3Length(&vLook);

	m_vCurrentPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	RELEASE_INSTANCE(CGameInstance);
}

void CFlogas::Boss_Behavior(_double DeltaTime)
{
	Get_PlayerDesc();

	if (true == CSkill_Manager::Get_Instance()->IsPlayer_Die() && false == m_bMotionLock)
	{
		m_bMotionLock = true;
		m_dwActionState = PLAYER_DIE1;
	}

	if (m_fHP <= 5000.f && false == m_bPhaseChangeLock)
	{
		m_dwActionState = PHASE_CHANGE;
		m_bPhaseChangeLock = true;
	}

	switch (m_dwActionState)
	{
	case CHASE:

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		m_pTransform->Look_Target(m_vPlayerPos);
		m_pTransform->Set_MovingState(true);
		

		Hit_Q(DeltaTime);
		Hit_W(DeltaTime);
		Hit_E(DeltaTime);
		Hit_R(DeltaTime);
		Hit_F(DeltaTime);
		Hit_N(DeltaTime);

		if (m_dwPhase == 1)
		{
			m_pMesh->Set_AnimationIndex(WALK);
			m_pTransform->Chase_Target(m_vPlayerPos, DeltaTime);

			if (m_fDistance < 4.f)
			{
				m_pTransform->Set_MovingState(false);

				_uint iRandom = m_pRandom->Get_RandomInt(1, 100);
				
				if (iRandom <= 20)
				{
					m_dwActionState = ATTACK1;
				}
				else if (iRandom <= 40)
				{
					m_dwActionState = ATTACK2;
				}
				else if (iRandom <= 70)
				{
					m_dwActionState = ATTACK3;
				}
				else
				{
					m_dwActionState = ATTACK4;
				}
			}
		}
		else if (m_dwPhase == 2)
		{
			m_pMesh->Set_AnimationIndex(RUN);
			m_pTransform->Chase_Target(m_vPlayerPos, DeltaTime * 1.3f);

			if (m_fDistance < 4.f)
			{
				m_pTransform->Set_MovingState(false);

				_uint iRandom = m_pRandom->Get_RandomInt(1, 100);

				if (iRandom <= 20)
				{
					m_dwActionState = ATTACK1;
				}
				else if (iRandom <= 40)
				{
					m_dwActionState = ATTACK2;
				}
				else if (iRandom <= 70)
				{
					m_dwActionState = ATTACK3;
				}
				else
				{
					m_dwActionState = ATTACK4;
				}
			}
		}

		break;

	case BATTLE_IDLE:
		m_pMesh->Set_AnimationIndex(BATTLE_IDLE);

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		Hit_Q(DeltaTime);
		Hit_W(DeltaTime);
		Hit_E(DeltaTime);
		Hit_R(DeltaTime);
		Hit_F(DeltaTime);
		Hit_N(DeltaTime);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 1.5f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = CHASE;
		}

		break;

	case HIT:
		m_pMesh->Set_AnimationIndex(HIT);

		break;

	case ATTACK1:
		m_pMesh->Set_AnimationIndex(ATTACK1);

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 1.f)
		{
			CSkill_Manager::Get_Instance()->Active_ATTACK1();

		}

		Hit_Q(DeltaTime);
		Hit_W(DeltaTime);
		Hit_E(DeltaTime);
		Hit_R(DeltaTime);
		Hit_F(DeltaTime);
		Hit_N(DeltaTime);

		if (m_fTimeAcc > 2.7f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = BATTLE_IDLE;
			CSkill_Manager::Get_Instance()->InActive_ATTACK1();
		}

		break;

	case ATTACK2:
		m_pMesh->Set_AnimationIndex(ATTACK2);

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		CSkill_Manager::Get_Instance()->Active_ATTACK2();

		m_fTimeAcc += DeltaTime;

		Hit_Q(DeltaTime);
		Hit_W(DeltaTime);
		Hit_E(DeltaTime);
		Hit_R(DeltaTime);
		Hit_F(DeltaTime);
		Hit_N(DeltaTime);

		if (m_fTimeAcc > 2.4f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = BATTLE_IDLE;
			CSkill_Manager::Get_Instance()->InActive_ATTACK2();
		}

		break;

	case ATTACK3:
		m_pMesh->Set_AnimationIndex(ATTACK3);

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 1.f)
		{
			CSkill_Manager::Get_Instance()->Active_ATTACK3();
		}

		Hit_Q(DeltaTime);
		Hit_W(DeltaTime);
		Hit_E(DeltaTime);
		Hit_R(DeltaTime);
		Hit_F(DeltaTime);
		Hit_N(DeltaTime);

		if (m_fTimeAcc > 2.8f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = BATTLE_IDLE;
			CSkill_Manager::Get_Instance()->InActive_ATTACK3();
		}

		break;

	case ATTACK4:
		m_pMesh->Set_AnimationIndex(ATTACK4);

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		m_fTimeAcc += DeltaTime;
		
		if (m_fTimeAcc > 1.f)
		{
			CSkill_Manager::Get_Instance()->Active_ATTACK4();

		}

		Hit_Q(DeltaTime);
		Hit_W(DeltaTime);
		Hit_E(DeltaTime);
		Hit_R(DeltaTime);
		Hit_F(DeltaTime);
		Hit_N(DeltaTime);

		if (m_fTimeAcc > 2.8f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = BATTLE_IDLE;
			CSkill_Manager::Get_Instance()->InActive_ATTACK4();
		}

		break;

	//case ATTACK5:
	//	m_pMesh->Set_AnimationIndex(ATTACK5);
	//
	//	if (m_fHP <= 0.f)
	//	{
	//		m_dwActionState = DIE1;
	//		break;
	//	}
	//
	//	m_fTimeAcc += DeltaTime;
	//
	//	if (m_fTimeAcc > 1.f)
	//	{
	//		CSkill_Manager::Get_Instance()->Active_ATTACK5();
	//
	//	}
	//
	//	Hit_Q(DeltaTime);
	//	Hit_W(DeltaTime);
	//	Hit_E(DeltaTime);
	//	Hit_R(DeltaTime);
	//	Hit_F(DeltaTime);
	//	Hit_N(DeltaTime);
	//
	//	if (m_fTimeAcc > 2.7f)
	//	{
	//		m_fTimeAcc = 0.f;
	//		m_dwActionState = BATTLE_IDLE;
	//		CSkill_Manager::Get_Instance()->InActive_ATTACK5();
	//	}
	//
	//	/* Counter */
	//
	//	break;

	case COUNTER:
		m_pMesh->Set_AnimationIndex(BATTLE_IDLE);

		break;

	case DIE1:
		m_pMesh->Set_AnimationIndex(DIE1);

		if (false == m_bDeleteLock)
		{
			m_bDeleteLock = true;
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			CTrail* pTrail = (CTrail*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Trail"), 2);
			
			pGameInstance->Add_DeleteContainer(LEVEL_GAMEPLAY, TEXT("Layer_Trail"), pTrail);

			RELEASE_INSTANCE(CGameInstance);
		}

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 12.f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = DIE2;
		}

		break;

	case DIE2:
		m_pMesh->Set_AnimationIndex(DIE2);

		break;

	case PHASE_CHANGE:

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc < 3.f)
		{
			m_pMesh->Set_AnimationIndex(PHASE_CHANGE);
		}
		else
		{
			m_pMesh->Set_AnimationIndex(BATTLE_IDLE);
		}

		if (m_fTimeAcc > 5.f)
		{
			m_fTimeAcc = 0.f;
			m_dwPhase = 2;
			m_dwActionState = CHASE;
		}
		
		break;

	case CREATE:
		break;

	case PLAYER_DIE1:
		m_pMesh->Set_AnimationIndex(PLAYER_DIE1);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 2.f)
		{
			m_fTimeAcc = 0.;
			m_dwActionState = PLAYER_DIE2;
		}

		break;

	case PLAYER_DIE2:
		m_pMesh->Set_AnimationIndex(BATTLE_IDLE);

		break;
	}
}

void CFlogas::Time_Initialize(_double DeltaTime)
{
	if (true == CSkill_Manager::Get_Instance()->IsActive_Q())
	{
		m_fTimeAcc_Q += DeltaTime;
	}
	else
	{
		m_fTimeAcc_Q = 0.f;
		m_bLock_Q = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_W())
	{
		m_fTimeAcc_W += DeltaTime;
	}
	else
	{
		m_fTimeAcc_W = 0.f;
		m_bLock_W1 = false;
		m_bLock_W2 = false;
		m_bLock_W3 = false;
		m_bLock_W4 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_E())
	{
		m_fTimeAcc_E += DeltaTime;
	}
	else
	{
		m_fTimeAcc_E = 0.f;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_R())
	{
		m_fTimeAcc_R += DeltaTime;
	}
	else
	{
		m_fTimeAcc_R = 0.f;
		m_bLock_R1 = false;
		m_bLock_R2 = false;
		m_bLock_R3 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_F())
	{
		m_fTimeAcc_F += DeltaTime;
	}
	else
	{
		m_fTimeAcc_F = 0.f;
		m_bLock_F1 = false;
		m_bLock_F2 = false;
		m_bLock_F3 = false;
		m_bLock_F4 = false;
		m_bLock_F5 = false;
		m_bLock_F6 = false;
		m_bLock_F7 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_N1())
	{
		m_fTimeAcc_N1 += DeltaTime;
	}
	else
	{
		m_fTimeAcc_N1 = 0.f;
		m_bLock_N1 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_N2())
	{
		m_fTimeAcc_N2 += DeltaTime;
	}
	else
	{
		m_fTimeAcc_N2 = 0.f;
		m_bLock_N2 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_N3())
	{
		m_fTimeAcc_N3 += DeltaTime;
	}
	else
	{
		m_fTimeAcc_N3 = 0.f;
		m_bLock_N3 = false;
	}
}

_bool CFlogas::Hit_N(_double DeltaTime)
{
	if (true == CSkill_Manager::Get_Instance()->IsActive_N1())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_N))
		{
			if (false == m_bLock_N1)
			{
				CreateEffect(TEXT("Prototype_Effect_Normal"), 7.f);

				m_fHP -= m_fDamage_N1;
				m_bLock_N1 = true;
				return true;
			}
		}
	}
	else if (true == CSkill_Manager::Get_Instance()->IsActive_N2())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_N))
		{
			if (false == m_bLock_N2)
			{
				CreateEffect(TEXT("Prototype_Effect_Normal"), 7.f);

				m_fHP -= m_fDamage_N2;
				m_bLock_N2 = true;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_N3 > 0.5f && m_fTimeAcc_N3 < 0.6f && true == CSkill_Manager::Get_Instance()->IsActive_N3())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_N))
		{
			if (false == m_bLock_N3)
			{
				CreateEffect(TEXT("Prototype_Effect_Normal"), 7.f);

				m_fHP -= m_fDamage_N3;
				m_bLock_N3 = true;
				return true;
			}
		}
	}

	return false;
}

_bool CFlogas::Hit_F(_double DeltaTime)
{
	if (m_fTimeAcc_F > 0.f && m_fTimeAcc_F < 0.1f && true == CSkill_Manager::Get_Instance()->IsActive_F())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_F1)
			{
				CreateEffect(TEXT("Prototype_Effect_Electric"), 5.f);

				m_bLock_F1 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_F > 0.3f && m_fTimeAcc_F < 0.4f && true == CSkill_Manager::Get_Instance()->IsActive_F())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_F2)
			{
				CreateEffect(TEXT("Prototype_Effect_Electric"), 5.f);

				m_bLock_F2 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_F > 0.6f && m_fTimeAcc_F < 0.7f && true == CSkill_Manager::Get_Instance()->IsActive_F())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_F3)
			{
				CreateEffect(TEXT("Prototype_Effect_Electric"), 5.f);

				m_bLock_F3 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_F > 0.9f && m_fTimeAcc_F < 1.0f && true == CSkill_Manager::Get_Instance()->IsActive_F())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_F4)
			{
				CreateEffect(TEXT("Prototype_Effect_Electric"), 5.f);

				m_bLock_F4 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_F > 1.2f && m_fTimeAcc_F < 1.3f && true == CSkill_Manager::Get_Instance()->IsActive_F())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_F5)
			{
				CreateEffect(TEXT("Prototype_Effect_Electric"), 5.f);

				m_bLock_F5 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_F > 1.5f && m_fTimeAcc_F < 1.6f && true == CSkill_Manager::Get_Instance()->IsActive_F())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_F6)
			{
				CreateEffect(TEXT("Prototype_Effect_Electric"), 5.f);

				m_bLock_F6 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_F > 2.1f && m_fTimeAcc_F < 2.2f && true == CSkill_Manager::Get_Instance()->IsActive_F())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_F7)
			{
				CreateEffect(TEXT("Prototype_Effect_Lightning"), 6.f, false, true);

				m_bLock_F7 = true;
				m_fHP -= m_fDamage_F * 2;
				m_fTimeAcc = 0.f;
				return true;
			}
		}
	}

	return false;
}

void CFlogas::CreateEffect(const _tchar* pPrototypeTag, _float fScale, _bool isRandom,_bool isPtr)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CRandom* pRandom = GET_INSTANCE(CRandom);

	if (true == isPtr)
	{
		m_vPos = _float4(m_vCurrentPos, fScale);

		_float4* pPos = &m_vPos;

		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pPrototypeTag, TEXT("Layer_Effect"), &pPos);
	}
	else
	{
		_float4 vPos = _float4(m_vCurrentPos, fScale);

		if (true == isRandom)
		{
			vPos.y += pRandom->Get_RandomFloat(-3.f, 5.f);
		}

		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pPrototypeTag, TEXT("Layer_Effect"), &vPos);
	}

	RELEASE_INSTANCE(CRandom);
	RELEASE_INSTANCE(CGameInstance);
}

_bool CFlogas::Hit_Q(_double DeltaTime)
{
	if (m_fTimeAcc_Q > 0.0f && m_fTimeAcc_Q < 0.25f && true == CSkill_Manager::Get_Instance()->IsActive_Q())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_Q)
			{
				CreateEffect(TEXT("Prototype_Effect_Spark"), 5.f);

				m_bLock_Q = true;
				m_fHP -= m_fDamage_Q;
				return true;
			}
		}
	}

	return false;
}

_bool CFlogas::Hit_W(_double DeltaTime)
{
	if (m_fTimeAcc_W > 0.f && m_fTimeAcc_W < 0.1f && true == CSkill_Manager::Get_Instance()->IsActive_W())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_W1)
			{
				CreateEffect(TEXT("Prototype_Effect_Spark"), 5.f);
				
				m_fHP -= m_fDamage_W;
				m_bLock_W1 = true;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_W > 0.2f && m_fTimeAcc_W < 0.3f && true == CSkill_Manager::Get_Instance()->IsActive_W())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_W2)
			{
				CreateEffect(TEXT("Prototype_Effect_Spark"), 5.f);

				m_fHP -= m_fDamage_W;
				m_bLock_W2 = true;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_W > 0.4f && m_fTimeAcc_W < 0.5f && true == CSkill_Manager::Get_Instance()->IsActive_W())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_W3)
			{
				CreateEffect(TEXT("Prototype_Effect_Spark"), 5.f);

				m_fHP -= m_fDamage_W;
				m_bLock_W3 = true;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_W > 0.7f && m_fTimeAcc_W < 0.8f && true == CSkill_Manager::Get_Instance()->IsActive_W())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_W4)
			{
				CreateEffect(TEXT("Prototype_Effect_Lightning"), 6.f, false, true);
				
				m_fHP -= m_fDamage_W * 2;
				m_bLock_W4 = true;
				return true;
			}
		}
	}

	return false;
}

_bool CFlogas::Hit_E(_double DeltaTime)
{
	if (m_fTimeAcc_E > 0.4f && m_fTimeAcc_E < 0.5f && true == CSkill_Manager::Get_Instance()->IsActive_E())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_N))
		{
			CreateEffect(TEXT("Prototype_Effect_Explosion"), 5.f);

			m_fHP -= m_fDamage_E;
			m_fTimeAcc_E = true;
			return true;
		}
	}

	return false;
}

_bool CFlogas::Hit_R(_double DeltaTime)
{
	if (m_fTimeAcc_R > 0.f && m_fTimeAcc_R < 0.1f && true == CSkill_Manager::Get_Instance()->IsActive_R())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_R1)
			{
				CreateEffect(TEXT("Prototype_Effect_Normal"), 5.f);

				m_bLock_R1 = true;
				m_fHP -= m_fDamage_R;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_R > 0.3f && m_fTimeAcc_R < 0.4f && true == CSkill_Manager::Get_Instance()->IsActive_R())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_R2)
			{
				CreateEffect(TEXT("Prototype_Effect_Normal"), 5.f);

				m_bLock_R2 = true;
				m_fHP -= m_fDamage_R;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_R > 0.8f && m_fTimeAcc_R < 0.9f && true == CSkill_Manager::Get_Instance()->IsActive_R())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_R3)
			{
				CreateEffect(TEXT("Prototype_Effect_Explosion"), 5.f);

				m_bLock_R3 = true;
				m_fHP -= m_fDamage_R;
				return true;
			}
		}
	}

	return false;
}

CFlogas* CFlogas::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFlogas* pInstance = new CFlogas(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Flogas");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFlogas::Clone(void* pArg)
{
	CFlogas* pInstance = new CFlogas(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Flogas");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas::Free()
{
	__super::Free();

	Safe_Release(m_pMesh);
	Safe_Release(m_pShader);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);

	Safe_Release(m_pPlayer_W);
	Safe_Release(m_pPlayer_N);
	Safe_Release(m_pPlayer_D);

	Safe_Release(m_pFlogasWeapon);
}
