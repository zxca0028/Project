#include "pch.h"
#include "Skeleton.h"
#include "GameInstance.h"
#include "KeyManager.h"
#include "Random.h"
#include "Collision_Manager.h"
#include "Player.h"
#include "Skill_Manager.h"
#include "Icon.h"

CSkeleton::CSkeleton(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{}

CSkeleton::CSkeleton(const CSkeleton& rhs)
	: CGameObject(rhs)
{}

HRESULT CSkeleton::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CSkeleton::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	m_pCollision_Manager = GET_INSTANCE(CCollision_Manager);

	RELEASE_INSTANCE(CCollision_Manager);

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

	_float4 vSpawnPos;
	memcpy(&vSpawnPos, pArg, sizeof(_float4));

	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(vSpawnPos));
	m_pTransform->Set_Scale(_float3(0.1f, 0.1f, 0.1f));
	m_pTransform->Rotation_Axis_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(vSpawnPos.w));

	m_dwActionState = BATTLE_IDLE;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CIcon::ICONINFO IconInfo;

	const _matrix* pWorldMatrix = m_pTransform->Get_WorldMatrix();

	IconInfo.WorldMatrix = pWorldMatrix;
	IconInfo.fObjectHP   = &m_fHP;
	IconInfo.bRender     = &m_bActivate;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Icon"), TEXT("Layer_HPGauge"), &IconInfo)))
	{
		return E_FAIL;
	}

	m_pPlayer = (CPlayer*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"));

	RELEASE_INSTANCE(CGameInstance);

	m_dwActionState = CREATE;

	return S_OK;
}

_int CSkeleton::Tick(_double DeltaTime)
{
	__super::Tick(DeltaTime);

	m_vPos = _float4(m_vMyPos,1.f);

	if (m_dwActionState != DIE2 && m_dwActionState != CREATE )
	{
		D3DXPLANE Plane;

		if (false == m_pNavigation->Move_OnNavigation(m_vMyPos, &Plane))
		{
			m_bisIn = false;
		}
		else
		{
			m_bisIn = true;
			m_pTransform->Ride_Navigation(Plane);
		}
	}

	Monster_Behavior(DeltaTime);

	Time_Initailize(DeltaTime);

	m_pCollider->Update_Collider();

	return _int();
}

_int CSkeleton::Late_Tick(_double DeltaTime)
{
	__super::Late_Tick(DeltaTime);

	m_dwPlayerActionState = m_pCollision_Manager->Get_PlayerState();

	m_pMesh->Play_Animation(DeltaTime);

	if (true == m_pFrustum->isIn(m_pTransform->Get_State(CTransform::STATE_POSITION), 5.f))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		{
			return E_FAIL;
		}
	}

	return _int();
}

HRESULT CSkeleton::Render()
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

#ifdef _DEBUG

	if (true == m_CollisionView)
	{
		m_pRenderer->Add_RenderDebug(m_pCollider);
	}

#endif // _DEBUG

	return S_OK;
}

HRESULT CSkeleton::Set_ConstTable()
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
	m_pShader->Set_ConstTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ProjMatrix", &ProjMatrix, sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSkeleton::Add_Components(void* pArg)
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fSpeedPerSec = 2.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader_Mesh"), (CComponent**)&m_pShader))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Frustum"), TEXT("Com_Frustum"), (CComponent**)&m_pFrustum))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Skeleton"), TEXT("Com_Mesh"), (CComponent**)&m_pMesh))) { return E_FAIL; }

	CCollider::COLLIDERDESC ColliderDesc;
	ColliderDesc.pParentMatrix = m_pTransform->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(1.f, 2.5f, 1.f);
	ColliderDesc.vInitPos = _float3(0.f, 1.25f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pCollider, &ColliderDesc))) { return E_FAIL; }

	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentCellIndex = 1;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigation, &NaviDesc))) { return E_FAIL; }

	return S_OK;
}

void CSkeleton::Monster_Behavior(_double DeltaTime)
{
	if (true == CSkill_Manager::Get_Instance()->IsPlayer_Die() && false == m_bMotionLock)
	{
		m_bMotionLock = true;
		m_dwActionState = PLAYER_DIE;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_vPlayerPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
	m_vMyPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	m_vDir = m_vPlayerPos - m_vMyPos;
	m_fDist = D3DXVec3Length(&m_vDir);

	switch (m_dwActionState)
	{
	case CHASE:

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		if (true == Hit_N(DeltaTime)) { break; }

		m_pMesh->Set_AnimationIndex(CHASE);

		m_pTransform->Look_Target(m_vPlayerPos);

		if (true == Hit_Q(DeltaTime)) { break; }
		if (true == Hit_W(DeltaTime)) { break; }
		if (true == Hit_E(DeltaTime)) { break; }
		if (true == Hit_R(DeltaTime)) { break; }
		if (true == Hit_F(DeltaTime)) { break; }

		if (m_fDist > 1.0f)
		{
			m_pTransform->Set_MovingState(true);
			m_pTransform->Chase_Target(m_vPlayerPos, DeltaTime);
			m_dwActionState = CHASE;
		}
		else
		{
			m_pTransform->Set_MovingState(false);

			_int iPattern = m_pRandom->Get_RandomInt(1, 9);

			if (iPattern <= 3)
			{
				m_dwActionState = ATTACK1;
			}
			else if (iPattern <= 6)
			{
				m_dwActionState = ATTACK2;
			}
			else
			{
				m_dwActionState = ATTACK2;
			}
		}

		break;

	case BATTLE_IDLE:

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		m_bAttackLock = false;

		if (true == Hit_N(DeltaTime)) { break; }

		m_pMesh->Set_AnimationIndex(BATTLE_IDLE);

		m_fTimeAcc += DeltaTime;

		m_pTransform->Look_Target(m_vPlayerPos);

		if (true == Hit_Q(DeltaTime)) { break; }
		if (true == Hit_W(DeltaTime)) { break; }
		if (true == Hit_E(DeltaTime)) { break; }
		if (true == Hit_R(DeltaTime)) { break; }
		if (true == Hit_F(DeltaTime)) { break; }

		if (m_fTimeAcc > 1.f)
		{
			m_fTimeAcc = 0.f;

			if (m_fDist > 1.5f)
			{
				m_pTransform->Set_MovingState(true);
				m_pTransform->Chase_Target(m_vPlayerPos, DeltaTime);
				m_dwActionState = CHASE;
			}
			else
			{
				m_pTransform->Set_MovingState(false);

				_int iPattern = m_pRandom->Get_RandomInt(1, 9);

				if (iPattern <= 3)
				{
					m_dwActionState = ATTACK1;
				}
				else if (iPattern <= 6)
				{
					m_dwActionState = ATTACK2;
				}
				else
				{
					m_dwActionState = ATTACK2;
				}
			}
		}

		break;

	case HIT:

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		Hit_N(DeltaTime, true);

		Hit_W(DeltaTime);
		Hit_R(DeltaTime);

		m_pMesh->Set_AnimationIndex(HIT);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 0.15f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = BATTLE_IDLE;
		}

		break;

	case ATTACK1:

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		if (true == Hit_N(DeltaTime)) { break; }


		m_pMesh->Set_AnimationIndex(ATTACK1);

		m_fTimeAcc += DeltaTime;

		Attack_Player(2.f, m_fDamage);

		if (true == Hit_Q(DeltaTime)) { break; }
		if (true == Hit_W(DeltaTime)) { break; }
		if (true == Hit_E(DeltaTime)) { break; }
		if (true == Hit_R(DeltaTime)) { break; }
		if (true == Hit_F(DeltaTime)) { break; }

		if (m_fTimeAcc > 3.8f)
		{
			m_fTimeAcc = 0.f;
			m_bAttackLock = false;
			m_dwActionState = BATTLE_IDLE;
		}

		break;

	case ATTACK2:

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		if (true == Hit_N(DeltaTime)) { break; }

		m_pMesh->Set_AnimationIndex(ATTACK2);

		m_fTimeAcc += DeltaTime;

		Attack_Player(2.f, m_fDamage);

		if (true == Hit_Q(DeltaTime)) { break; }
		if (true == Hit_W(DeltaTime)) { break; }
		if (true == Hit_E(DeltaTime)) { break; }
		if (true == Hit_R(DeltaTime)) { break; }
		if (true == Hit_F(DeltaTime)) { break; }

		if (m_fTimeAcc > 3.5f)
		{
			m_fTimeAcc = 0.f;
			m_bAttackLock = false;
			m_dwActionState = BATTLE_IDLE;
		}

		break;

	case ATTACK3:

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		if (true == Hit_N(DeltaTime)) { break; }

		m_pMesh->Set_AnimationIndex(ATTACK3);

		m_fTimeAcc += DeltaTime;

		Attack_Player(2.f, m_fDamage);

		if (true == Hit_Q(DeltaTime)) { break; }
		if (true == Hit_W(DeltaTime)) { break; }
		if (true == Hit_E(DeltaTime)) { break; }
		if (true == Hit_R(DeltaTime)) { break; }
		if (true == Hit_F(DeltaTime)) { break; }

		if (m_fTimeAcc > 2.4f)
		{
			m_fTimeAcc = 0.f;
			m_bAttackLock = false;
			m_dwActionState = BATTLE_IDLE;
		}

		break;

	case KNOCKBACK:
	{
		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		Hit_N(DeltaTime, false);

		Hit_Q(DeltaTime);
		if (true == Hit_W(DeltaTime)) { break; }
		Hit_R(DeltaTime);
		if (true == Hit_F(DeltaTime)) { break; }

		m_pMesh->Set_AnimationIndex(HIT);

		m_fTimeAcc += DeltaTime;

		static _float fDelta = 0.f;

		if (false == m_bLock_E1)
		{
			_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

			if (abs(D3DXVec3Dot(&vLook, &m_vDir)) < 0.86f)
			{
				_float3 vScale = m_pTransform->Get_Scale();

				_float3 vUp    = _float3(0.f, 1.f, 0.f);
				_float3 vRight = *D3DXVec3Cross(&vRight, &vUp, &m_vDir);
				_float3 vLook  = m_vDir;

				D3DXVec3Normalize(&vUp, &vUp);
				D3DXVec3Normalize(&vRight, &vRight);
				D3DXVec3Normalize(&vLook, &vLook);

				m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
				m_pTransform->Set_State(CTransform::STATE_UP,    vUp* vScale.y);
				m_pTransform->Set_State(CTransform::STATE_LOOK,  vLook* vScale.z);
			}

			m_bLock_E1 = true;
		}

		if (m_fTimeAcc < 1.f)
		{
			fDelta = (powf(512, -m_fTimeAcc))/10.f;

			if (true == m_bisIn)
			{
				m_pTransform->Move_Back(fDelta);
			}
		}

		if (m_fTimeAcc > m_fRandomFloat)
		{
			fDelta = 0.f;
			m_bLock_E1 = false;
			m_fTimeAcc = 0.f;
			m_dwActionState = BATTLE_IDLE;
		}

		break;
	}
	case AIRBORNE1:
	{
		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		Hit_N(DeltaTime, false);

		Hit_Q(DeltaTime);
		if (true == Hit_E(DeltaTime)) { break; }
		Hit_R(DeltaTime);
		if (true == Hit_F(DeltaTime)) { break; }

		m_pMesh->Set_AnimationIndex(AIRBORNE1);

		m_fTimeAcc += DeltaTime;

		if (false == m_bLock_W1)
		{	
			_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

			if (abs(D3DXVec3Dot(&vLook, &m_vDir)) < 0.86f)
			{
				_float3 vScale = m_pTransform->Get_Scale();

				_float3 vUp = _float3(0.f, 1.f, 0.f);
				_float3 vRight = *D3DXVec3Cross(&vRight, &vUp, &m_vDir);
				_float3 vLook = m_vDir;

				D3DXVec3Normalize(&vUp, &vUp);
				D3DXVec3Normalize(&vRight, &vRight);
				D3DXVec3Normalize(&vLook, &vLook);

				m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
				m_pTransform->Set_State(CTransform::STATE_UP, vUp * vScale.y);
				m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
			}
			m_bLock_W1 = true;
		}

		if (true == m_bisIn)
		{
			m_vMyPos.y = (55 * (sinf(D3DXToRadian(45.f)) * m_fTimeAcc * 10) - (0.5f * 9.81f * m_fTimeAcc * 10 * m_fTimeAcc * 10)) / 100 + 9.5f;
			m_pTransform->Set_State(CTransform::STATE_POSITION, m_vMyPos);
			m_pTransform->Move_Back(DeltaTime * 1.2f);
		}

		if (m_fTimeAcc > 0.8f)
		{
			m_fTimeAcc = 0.f;
			m_fRandomFloat = m_pRandom->Get_RandomFloat(0.3f, 2.f);
			m_dwActionState = AIRBORNE2;
		}

		break;
	}
	case AIRBORNE2:
	{
		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		m_pMesh->Set_AnimationIndex(AIRBORNE2);

		Hit_N(DeltaTime, false);

		Hit_Q(DeltaTime);
		if (true == Hit_E(DeltaTime)) { break; }
		Hit_R(DeltaTime);
		if (true == Hit_F(DeltaTime)) { break; }

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > m_fRandomFloat)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = AIRBORNE3;
		}
		break;
	}
	case AIRBORNE3:

		if (m_fHP <= 0.f)
		{
			m_dwActionState = DIE1;
			break;
		}

		m_pMesh->Set_AnimationIndex(AIRBORNE3);

		Hit_N(DeltaTime, false);

		Hit_Q(DeltaTime);
		if (true == Hit_E(DeltaTime)) { break; }
		Hit_R(DeltaTime);
		if (true == Hit_F(DeltaTime)) { break; }

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 1.8f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = BATTLE_IDLE;
		}

		break;

	case DIE1:
		m_pMesh->Set_AnimationIndex(DIE1);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 6.5f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = DIE2;
		}

		break;

	case DIE2:
		m_pMesh->Set_AnimationIndex(DIE2);

		m_fTimeAcc += DeltaTime;

		if (false == m_bCreateLock)
		{
			CreateEffect(TEXT("Prototype_Effect_FinalFire"), 4.f, false, true);

			m_bCreateLock = true;
		}

		if (m_fTimeAcc > 1.f)
		{
			m_pTransform->Move_Down(DeltaTime * 0.5f);
		}
		if (m_fTimeAcc > 3.f)
		{
			m_fTimeAcc = 0.f;

			m_bisDelete = true;

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			pGameInstance->Add_DeleteContainer(LEVEL_GAMEPLAY, TEXT("Layer_Skeleton"), this);

			RELEASE_INSTANCE(CGameInstance);
		}

		break;

	case CREATE:
		m_pMesh->Set_AnimationIndex(BATTLE_IDLE);

		if (true == CSkill_Manager::Get_Instance()->IsMoveStart())
		{
			if (m_vMyPos.y < 9.5f)
			{
				m_pTransform->Move_Up(DeltaTime * 0.6f);
			}
			else
			{
				m_fTimeAcc += DeltaTime;

				if (m_fTimeAcc > 3.f)
				{
					m_fTimeAcc = 0.f;
					m_bActivate = true;
					m_dwActionState = CHASE;
				}
			}
		}

		break;

	case PLAYER_DIE:
		m_pMesh->Set_AnimationIndex(BATTLE_IDLE);
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
}

_bool CSkeleton::Hit_Q(_double DeltaTime)
{
	if (m_fTimeAcc_Q > 0.0f && m_fTimeAcc_Q < 0.25f && true == CSkill_Manager::Get_Instance()->IsActive_Q())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_Q)
			{
				CreateEffect(TEXT("Prototype_Effect_Spark"), 3.f);

				m_bLock_Q = true;
				m_fHP -= m_fDamage_Q;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
				return true;
			}
		}
	}

	return false;
}

_bool CSkeleton::Hit_W(_double DeltaTime)
{
	if (m_fTimeAcc_W > 0.f && m_fTimeAcc_W < 0.1f && true == CSkill_Manager::Get_Instance()->IsActive_W())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_W2)
			{
				CreateEffect(TEXT("Prototype_Effect_Spark"), 2.f);

				m_fHP -= 7.f;
				m_fTimeAcc = 0.f;
				m_bLock_W2 = true;
				m_dwActionState = HIT;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_W > 0.2f && m_fTimeAcc_W < 0.3f && true == CSkill_Manager::Get_Instance()->IsActive_W())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_W3)
			{
				CreateEffect(TEXT("Prototype_Effect_Spark"), 2.f);

				m_fHP -= 7.f;
				m_fTimeAcc = 0.f;
				m_bLock_W3 = true;
				m_dwActionState = HIT;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_W > 0.4f && m_fTimeAcc_W < 0.5f && true == CSkill_Manager::Get_Instance()->IsActive_W())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_W4)
			{
				CreateEffect(TEXT("Prototype_Effect_Spark"), 2.f);

				m_fHP -= 7.f;
				m_fTimeAcc = 0.f;
				m_bLock_W4 = true;
				m_dwActionState = HIT;
				return true;
			}
		}
	}
	else if (m_fTimeAcc_W > 0.7f && m_fTimeAcc_W < 0.8f && true == CSkill_Manager::Get_Instance()->IsActive_W())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_W5)
			{
				m_fHP -= m_fDamage_W;

				CreateEffect(TEXT("Prototype_Effect_Lightning"), 3.f, false, true);

				_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

				if (abs(D3DXVec3Dot(&vLook, &m_vDir)) < 0.86f)
				{
					_float3 vScale = m_pTransform->Get_Scale();

					_float3 vUp = _float3(0.f, 1.f, 0.f);
					_float3 vRight = *D3DXVec3Cross(&vRight, &vUp, &m_vDir);
					_float3 vLook = m_vDir;

					D3DXVec3Normalize(&vUp, &vUp);
					D3DXVec3Normalize(&vRight, &vRight);
					D3DXVec3Normalize(&vLook, &vLook);

					m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
					m_pTransform->Set_State(CTransform::STATE_UP, vUp * vScale.y);
					m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
				}

				m_fTimeAcc = 0.f;
				m_dwActionState = AIRBORNE1;
				return true;
			}
		}
	}

	return false;
}

_bool CSkeleton::Hit_E(_double DeltaTime)
{
	if (m_fTimeAcc_E > 0.4f && m_fTimeAcc_E < 0.5f && true == CSkill_Manager::Get_Instance()->IsActive_E())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_N))
		{
			if (false == m_bLock_E2)
			{
				CreateEffect(TEXT("Prototype_Effect_Explosion"), 2.f);

				m_fHP -= m_fDamage_E;

				_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

				if (abs(D3DXVec3Dot(&vLook, &m_vDir)) < 0.86f)
				{
					_float3 vScale = m_pTransform->Get_Scale();

					_float3 vUp = _float3(0.f, 1.f, 0.f);
					_float3 vRight = *D3DXVec3Cross(&vRight, &vUp, &m_vDir);
					_float3 vLook = m_vDir;

					D3DXVec3Normalize(&vUp, &vUp);
					D3DXVec3Normalize(&vRight, &vRight);
					D3DXVec3Normalize(&vLook, &vLook);

					m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
					m_pTransform->Set_State(CTransform::STATE_UP, vUp * vScale.y);
					m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
				}

				m_fTimeAcc = 0.f;
				m_fRandomFloat = m_pRandom->Get_RandomFloat(1.1f, 2.f);
				m_dwActionState = KNOCKBACK;
				m_bLock_E2 = false;
				return true;
			}
			
		}
	}

	return false;
}

_bool CSkeleton::Hit_R(_double DeltaTime)
{
	if (m_fTimeAcc_R > 0.f && m_fTimeAcc_R < 0.1f && true == CSkill_Manager::Get_Instance()->IsActive_R())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_R1)
			{
				CreateEffect(TEXT("Prototype_Effect_Normal"), 3.f);

				m_bLock_R1 = true;
				m_fHP -= m_fDamage_R;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
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
				CreateEffect(TEXT("Prototype_Effect_Normal"), 3.f);

				m_bLock_R2 = true;
				m_fHP -= m_fDamage_R;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
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
				CreateEffect(TEXT("Prototype_Effect_Explosion"), 3.f);

				m_bLock_R3 = true;
				m_fHP -= m_fDamage_R;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
				return true;
			}
		}
	}

	return false;
}

_bool CSkeleton::Hit_N(_double DeltaTime, _bool bActionChange)
{
	if (true == CSkill_Manager::Get_Instance()->IsActive_N1())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_N))
		{
			if (false == m_bLock_N1)
			{
				m_fHP -= m_fDamage_N1;
				m_bLock_N1 = true;

				if (true == bActionChange)
				{
					m_fTimeAcc = 0.f;
					m_dwActionState = HIT;
				}

				CreateEffect(TEXT("Prototype_Effect_Normal"), 3.f);

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
				m_fHP -= m_fDamage_N2;
				m_bLock_N2 = true;

				if (true == bActionChange)
				{
					m_fTimeAcc = 0.f;
					m_dwActionState = HIT;
				}

				CreateEffect(TEXT("Prototype_Effect_Normal"), 3.f);

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
				m_fHP -= m_fDamage_N3;
				m_bLock_N3 = true;

				if (true == bActionChange)
				{
					m_fTimeAcc = 0.f;
					m_dwActionState = HIT;
				}

				CreateEffect(TEXT("Prototype_Effect_Normal"), 3.f);

				return true;
			}
		}
	}

	return false;
}

_bool CSkeleton::Hit_F(_double DeltaTime)
{
	if (m_fTimeAcc_F > 0.f && m_fTimeAcc_F < 0.1f && true == CSkill_Manager::Get_Instance()->IsActive_F())
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_W))
		{
			if (false == m_bLock_F1)
			{
				CreateEffect(TEXT("Prototype_Effect_Electric"), 2.f);

				m_bLock_F1 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
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
				CreateEffect(TEXT("Prototype_Effect_Electric"), 2.f);

				m_bLock_F2 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
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
				CreateEffect(TEXT("Prototype_Effect_Electric"), 2.f);

				m_bLock_F3 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
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
				CreateEffect(TEXT("Prototype_Effect_Electric"), 2.f);

				m_bLock_F4 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
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
				CreateEffect(TEXT("Prototype_Effect_Electric"), 2.f);

				m_bLock_F5 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
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
				CreateEffect(TEXT("Prototype_Effect_Electric"), 2.f);

				m_bLock_F6 = true;
				m_fHP -= m_fDamage_F;
				m_fTimeAcc = 0.f;
				m_dwActionState = HIT;
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
				CreateEffect(TEXT("Prototype_Effect_Lightning"), 3.f, false, true);

				m_bLock_F7 = true;
				m_fHP -= m_fDamage_F * 2;
				m_fTimeAcc = 0.f;
				m_dwActionState = AIRBORNE1;
				return true;
			}
		}
	}

	return false;
}

void CSkeleton::Time_Initailize(_double DeltaTime)
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
		m_bLock_W2 = false;
		m_bLock_W3 = false;
		m_bLock_W4 = false;
		m_bLock_W5 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_E())
	{
		m_fTimeAcc_E += DeltaTime;
	}
	else
	{
		m_fTimeAcc_E = 0.f;
		m_bLock_E2 = false;
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

void CSkeleton::CreateEffect(const _tchar* pPrototypeTag, _float fScale, _bool isRandom, _bool isPtr)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CRandom* pRandom = GET_INSTANCE(CRandom);

	if (true == isPtr)
	{
		m_vPos = _float4(m_vMyPos, fScale);

		_float4* pPos = &m_vPos;

		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pPrototypeTag, TEXT("Layer_Effect"), &pPos);
	}
	else
	{
		_float4 vPos = _float4(m_vMyPos, fScale);

		if (true == isRandom)
		{
			vPos.y += pRandom->Get_RandomFloat(-1.f, 3.f);
		}

		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pPrototypeTag, TEXT("Layer_Effect"), &vPos);
	}

	RELEASE_INSTANCE(CRandom);
	RELEASE_INSTANCE(CGameInstance);
}

void CSkeleton::Attack_Player(_float fTime, _float fDamage)
{
	if (m_fTimeAcc > fTime && false == m_bAttackLock)
	{
		if (m_pCollider->Collision_OBB(m_pPlayer_D))
		{
			m_pPlayer->Set_PlayerHP(fDamage);
			m_bAttackLock = true;
		}
	}
}

CSkeleton* CSkeleton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkeleton* pInstance = new CSkeleton(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Monster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkeleton::Clone(void* pArg)
{
	CSkeleton* pInstance = new CSkeleton(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Monster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton::Free()
{
	__super::Free();

	Safe_Release(m_pRandom);
	Safe_Release(m_pMesh);
	Safe_Release(m_pShader);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pNavigation);

	Safe_Release(m_pPlayer_W);
	Safe_Release(m_pPlayer_N);
	Safe_Release(m_pPlayer_D);
}
