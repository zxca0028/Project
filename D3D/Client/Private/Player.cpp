#include "pch.h"
#include "Player.h"
#include "GameInstance.h"
#include "KeyManager.h"
#include "Emotion.h"
#include "Collision_Manager.h"
#include "Skill_Manager.h"
#include "Sound_Manager.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	m_pCollision_Manager = GET_INSTANCE(CCollision_Manager);
	pGameInstance		 = GET_INSTANCE(CGameInstance);
	pKey				 = GET_INSTANCE(CKeyManager);

	RELEASE_INSTANCE(CCollision_Manager);
	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CKeyManager);

	if (FAILED(Add_Components(pArg)))
	{
		return E_FAIL;
	}

	m_pTransform->Set_Scale(_float3(0.1f, 0.1f, 0.1f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(88.33f, 9.5f, 64.13f));
	//m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(26.62f, 20.45f, 128.01f));
	//m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(34.74f, 18.30f, 115.12f));
	//m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(8.2f, 0.f, 8.55f));

	m_dwActionState = IDLE;

	_float* pHP = &m_fHP;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_PlayerGauge"), TEXT("Layer_PlayerHP"), &pHP)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CPlayer::Tick(_double DeltaTime)
{
	__super::Tick(DeltaTime);

	Time_Initialize(DeltaTime);

	m_vCurrentPos  = m_pTransform->Get_State(CTransform::STATE_POSITION);

	if (pKey->Key_Down(KEY_1))
	{
		m_fHP += 10000.f;
	}
	
	if (m_dwActionState != IDLE)
	{
		CSkill_Manager::Get_Instance()->MoveStart();
	}

	pKey->Key_Update();

	Create_Emotion(DeltaTime);
	Ride_Navigation();
	
	if (m_PlayerControl == true)
	{
		Player_Behavior(DeltaTime);
	}
	else
	{
		m_pMesh->Set_AnimationIndex(IDLE);
	}

	m_pCollision_Manager->Set_PlayerState(m_dwActionState);

	if (pKey->Key_Down(KEY_RB))
	{
		_float3* pPickingPos = m_pPicking->Compute_PickingPoint(m_pNavigation, m_vCurrentPos);
	
		if (nullptr != pPickingPos)
		{
			m_vTargetPos = *pPickingPos;
			
			_float4 vPos = _float4(m_vTargetPos, 0.5f);

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Effect_Arcane"), TEXT("Layer_Arcane"), &vPos);
		}
	}

	return _int();
}

_int CPlayer::Late_Tick(_double DeltaTime)
{
	__super::Late_Tick(DeltaTime);

	m_pMesh->Play_Animation(DeltaTime);

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
	{
		return E_FAIL;
	}

	return _int();
}

HRESULT CPlayer::Render()
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
	if (true == m_NaviView)
	{
		_matrix Identity;
		//m_pNavigation->Render(D3DXMatrixIdentity(&Identity));
		m_pRenderer->Add_RenderDebug(m_pNavigation);
	}

	#endif

	return S_OK;
}

HRESULT CPlayer::Render_ShadowDepth()
{
	__super::Render();

	_matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
	
	_float3 vLightEye = _float3(-5.f, 15.f, -5.f);
	_float3 vLightAt  = _float3(60.f,  0.f, 60.f);
	_float3 vLightUp  = _float3( 0.f,  1.f,  0.f);

	_matrix LightViewMatrix = *D3DXMatrixLookAtLH(&LightViewMatrix, &vLightEye, &vLightAt, &vLightUp);

	_matrix LightProjMatrix = *D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(120.f), (_float)g_iWinCX / (_float)g_iWinCY, 0.2f, 1000.f);

	m_pShader->Set_ConstTable("g_WorldMatrix",     &WorldMatrix,	 sizeof(_matrix));
	m_pShader->Set_ConstTable("g_LightViewMatrix", &LightViewMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_LightProjMatrix", &LightProjMatrix, sizeof(_matrix));

	m_pShader->Begin_Shader(3);

	_uint iNumMeshContainers = m_pMesh->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		_uint iNumMaterials = m_pMesh->Get_NumMaterials(i);

		m_pMesh->Update_SkinnedMesh(i);

		for (_uint j = 0; j < iNumMaterials; ++j)
		{
			m_pShader->Commit();

			m_pMesh->Render(i, j);
		}
	}

	m_pShader->End_Shader();

	return S_OK;
}

HRESULT CPlayer::Set_ConstTable()
{
	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}

	_matrix WrldMatrix = *m_pTransform->Get_WorldMatrix();
	_matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	_matrix ProjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

	m_pShader->Set_ConstTable("g_WorldMatrix", &WrldMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix, sizeof(_matrix));

	return S_OK;
}

HRESULT CPlayer::Add_Components(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec    = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Transform"),     TEXT("Com_Transform"),   (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_Mesh"),   TEXT("Com_Shader_Mesh"), (CComponent**)&m_pShader)))   { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"),	   TEXT("Com_Renderer"),    (CComponent**)&m_pRenderer))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Picking"),       TEXT("Com_Picking"),     (CComponent**)&m_pPicking)))  { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Player"),   TEXT("Com_Mesh"),	    (CComponent**)&m_pMesh)))     { return E_FAIL; }

	CCollider* pCollider = nullptr;
	CCollider* pCollider_W = nullptr;
	CCollider* pCollider_N = nullptr;
	CCollider* pCollider_D = nullptr;

	CCollider::COLLIDERDESC ColliderDesc;
	ColliderDesc.pParentMatrix = m_pTransform->Get_WorldMatrix();
	ColliderDesc.vScale		   = _float3(0.f, 1.f, 0.f);
	ColliderDesc.vInitPos	   = _float3(0.f, 0.5f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_OBB"),       TEXT("Com_OBB"), (CComponent**)&pCollider, &ColliderDesc))) { return E_FAIL; }
	
	ColliderDesc.vScale = _float3(3.5f, 2.f, 3.5f);
	ColliderDesc.vInitPos = _float3(0.f, 1.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_OBB"), TEXT("Com_OBB_W"), (CComponent**)&pCollider_W, &ColliderDesc))) { return E_FAIL; }

	ColliderDesc.vScale = _float3(2.5f, 3.f, 1.f);
	ColliderDesc.vInitPos = _float3(0.f, 1.f, 1.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_OBB"), TEXT("Com_OBB_N"), (CComponent**)&pCollider_N, &ColliderDesc))) { return E_FAIL; }

	ColliderDesc.vScale = _float3(2.f, 4.f, 2.f);
	ColliderDesc.vInitPos = _float3(0.f, 2.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_OBB"), TEXT("Com_OBB_D"), (CComponent**)&pCollider_D, &ColliderDesc))) { return E_FAIL; }

	m_pCollider_D = pCollider_D;

	if (FAILED(m_pCollision_Manager->Add_Collider(TEXT("Player_Body_OBB"), pCollider))) { return E_FAIL; }
	if (FAILED(m_pCollision_Manager->Add_Collider(TEXT("Player_Body_OBB_W"), pCollider_W))) { return E_FAIL; }
	if (FAILED(m_pCollision_Manager->Add_Collider(TEXT("Player_Body_OBB_N"), pCollider_N))) { return E_FAIL; }
	if (FAILED(m_pCollision_Manager->Add_Collider(TEXT("Player_Body_OBB_D"), pCollider_D))) { return E_FAIL; }
	Safe_Release(pCollider);
	Safe_Release(pCollider_W);
	Safe_Release(pCollider_N);
	Safe_Release(pCollider_D);

	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentCellIndex = 0;
	//NaviDesc.iCurrentCellIndex = 620;
	//NaviDesc.iCurrentCellIndex = 416;
	//NaviDesc.iCurrentCellIndex = 0;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigation, &NaviDesc))) { return E_FAIL; }

return S_OK;
}

void CPlayer::Ride_Navigation()
{
	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	D3DXPLANE Plane;

	if (false == m_pNavigation->Move_OnNavigation(vPos, &Plane))
	{
		m_pTransform->Set_MovingState(false);
		m_bisIn = false;
	}
	else
	{
		m_bisIn = true;
		m_pTransform->Ride_Navigation(Plane);
	}
}

HRESULT CPlayer::Create_Emotion(_double DeltaTime)
{
	if (false == m_bEmotionCreate)
	{
		if (pKey->Key_Down(KEY_F1))
		{
			int i = 0;
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Emotion"), TEXT("Layer_Emotion"), &i))) { return E_FAIL; }
			m_bEmotionCreate = true;
		}
		if (pKey->Key_Down(KEY_F2))
		{
			int i = 1;
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Emotion"), TEXT("Layer_Emotion"), &i))) { return E_FAIL; }
			m_bEmotionCreate = true;
		}
		if (pKey->Key_Down(KEY_F3))
		{
			int i = 2;
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Emotion"), TEXT("Layer_Emotion"), &i))) { return E_FAIL; }
			m_bEmotionCreate = true;
		}
		if (pKey->Key_Down(KEY_F4))
		{
			int i = 3;
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Emotion"), TEXT("Layer_Emotion"), &i))) { return E_FAIL; }
			m_bEmotionCreate = true;
		}
		if (pKey->Key_Down(KEY_F5))
		{
			int i = 4;
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Emotion"), TEXT("Layer_Emotion"), &i))) { return E_FAIL; }
			m_bEmotionCreate = true;
		}
	}
	if (true == m_bEmotionCreate)
	{
		m_EmotionTimeAcc += DeltaTime;
	}
	if (m_EmotionTimeAcc > 2.5f)
	{
		m_bEmotionCreate = false;
		m_EmotionTimeAcc = 0.f;

		pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Emotion"), 0);
	}

	return S_OK;
}

void CPlayer::Player_Behavior(_double DeltaTime)
{
	m_pFlogasCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Flogas"), TEXT("Com_OBB_Weapon"));

	switch (m_dwActionState)
	{
	case IDLE:

		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(IDLE);

		if (pKey->Key_Pressing(KEY_RB))
		{
			Look_Cursor(MOVING, true, true);
		}
		if (pKey->Key_Down(KEY_LB))
		{
			Look_Cursor(ATTACK1);
			break;
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_Q())
		{
			if (pKey->Key_Down(KEY_Q))
			{
				Look_Cursor(DASH);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_W())
		{
			if (pKey->Key_Down(KEY_W))
			{
				Look_Cursor(SKILL1);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_E())
		{
			if (pKey->Key_Down(KEY_E))
			{
				Look_Cursor(SKILL2);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_R())
		{
			if (pKey->Key_Down(KEY_R))
			{
				Look_Cursor(SKILL3_1);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_F())
		{
			if (pKey->Key_Down(KEY_F))
			{
				Look_Cursor(SKILL4);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_Space())
		{
			if (pKey->Key_Pressing(KEY_SPACE))
			{
				m_dwActionState = AVOID;
			}
		}

		break;

	case MOVING:

		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(MOVING);

		if (true == m_pCollision_Manager->Restrict_Move())
		{
			m_pTransform->Move_Front(-DeltaTime);
		}

		if (pKey->Key_Pressing(KEY_RB))
		{
			Look_Cursor(MOVING, true, true);
		}
		if (pKey->Key_Down(KEY_LB))
		{
			CSkill_Manager::Get_Instance()->Active_N1();
			Look_Cursor(ATTACK1);
			break;
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_Q())
		{
			if (pKey->Key_Down(KEY_Q))
			{
				Look_Cursor(DASH);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_W())
		{
			if (pKey->Key_Down(KEY_W))
			{
				Look_Cursor(SKILL1);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_E())
		{
			if (pKey->Key_Down(KEY_E))
			{
				Look_Cursor(SKILL2);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_R())
		{
			if (pKey->Key_Down(KEY_R))
			{
				Look_Cursor(SKILL3_1);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_F())
		{
			if (pKey->Key_Down(KEY_F))
			{
				Look_Cursor(SKILL4);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_Space())
		{
			if (pKey->Key_Pressing(KEY_SPACE))
			{
				m_dwActionState = AVOID;
			}
		}

		if (false == m_pTransform->Chase_Target(m_vTargetPos, DeltaTime))
		{
			m_dwActionState = IDLE;
		}

		break;

	case ATTACK1:

		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(ATTACK1);

		CSkill_Manager::Get_Instance()->Active_N1();

		m_fTimeAcc += DeltaTime;

		if (pKey->Key_Down(KEY_LB))
		{
			++m_iAttackCount;
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_Q())
		{
			if (pKey->Key_Down(KEY_Q))
			{
				CSkill_Manager::Get_Instance()->InActive_N1();
				Look_Cursor(DASH);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_W())
		{
			if (pKey->Key_Down(KEY_W))
			{
				CSkill_Manager::Get_Instance()->InActive_N1();
				Look_Cursor(SKILL1);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_E())
		{
			if (pKey->Key_Down(KEY_E))
			{
				CSkill_Manager::Get_Instance()->InActive_N1();
				Look_Cursor(SKILL2);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_R())
		{
			if (pKey->Key_Down(KEY_R))
			{
				CSkill_Manager::Get_Instance()->InActive_N1();
				Look_Cursor(SKILL3_1);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_F())
		{
			if (pKey->Key_Down(KEY_F))
			{
				CSkill_Manager::Get_Instance()->InActive_N1();
				Look_Cursor(SKILL4);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_Space())
		{
			if (pKey->Key_Pressing(KEY_SPACE))
			{
				CSkill_Manager::Get_Instance()->InActive_N1();
				m_fTimeAcc = 0.f;
				m_iAttackCount = 0;
				m_dwActionState = AVOID;
				break;
			}
		}

		if (m_fTimeAcc > 0.5f)
		{
			if (pKey->Key_Pressing(KEY_RB))
			{
				CSkill_Manager::Get_Instance()->InActive_N1();
				m_fTimeAcc = 0.f;
				m_iAttackCount = 0;
				Look_Cursor(IDLE, false, true);

				break;
			}

			if (m_iAttackCount > 0)
			{
				m_fTimeAcc = 0.f;
				CSkill_Manager::Get_Instance()->InActive_N1();
				m_dwActionState = ATTACK2;
			}
			else
			{
				CSkill_Manager::Get_Instance()->InActive_N1();
				m_fTimeAcc = 0.f;
				m_iAttackCount = 0;
				m_dwActionState = IDLE;
			}
		}

		break;

	case ATTACK2:

		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(DASH);

		CSkill_Manager::Get_Instance()->Active_N2();

		m_fTimeAcc += DeltaTime;

		if (pKey->Key_Down(KEY_LB))
		{
			++m_iAttackCount;
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_Q())
		{
			if (pKey->Key_Down(KEY_Q))
			{
				CSkill_Manager::Get_Instance()->InActive_N2();
				Look_Cursor(DASH);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_W())
		{
			if (pKey->Key_Down(KEY_W))
			{
				CSkill_Manager::Get_Instance()->InActive_N2();
				Look_Cursor(SKILL1);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_E())
		{
			if (pKey->Key_Down(KEY_E))
			{
				CSkill_Manager::Get_Instance()->InActive_N2();
				Look_Cursor(SKILL2);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_R())
		{
			if (pKey->Key_Down(KEY_R))
			{
				CSkill_Manager::Get_Instance()->InActive_N2();
				Look_Cursor(SKILL3_1);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_F())
		{
			if (pKey->Key_Down(KEY_F))
			{
				CSkill_Manager::Get_Instance()->InActive_N2();
				Look_Cursor(SKILL4);
				break;
			}
		}
		if (false == CSkill_Manager::Get_Instance()->IsActive_Space())
		{
			if (pKey->Key_Pressing(KEY_SPACE))
			{
				CSkill_Manager::Get_Instance()->InActive_N2();
				m_fTimeAcc = 0.f;
				m_iAttackCount = 0;
				m_dwActionState = AVOID;
				break;
			}
		}

		if (m_fTimeAcc > 0.6f)
		{
			if (pKey->Key_Pressing(KEY_RB))
			{
				CSkill_Manager::Get_Instance()->InActive_N2();
				m_fTimeAcc = 0.f;
				m_iAttackCount = 0;
				Look_Cursor(IDLE, true, true);
				break;
			}

			if (m_iAttackCount > 2)
			{
				m_fTimeAcc = 0.f;
				CSkill_Manager::Get_Instance()->InActive_N2();
				m_dwActionState = ATTACK3;
			}
			else
			{
				CSkill_Manager::Get_Instance()->InActive_N2();
				m_fTimeAcc = 0.f;
				m_iAttackCount = 0;
				m_dwActionState = IDLE;
			}
		}

		break;

	case ATTACK3:

		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(5);

		CSkill_Manager::Get_Instance()->Active_N3();

		m_fTimeAcc += DeltaTime;

		if (pKey->Key_Pressing(KEY_SPACE))
		{
			CSkill_Manager::Get_Instance()->InActive_N3();
			m_fTimeAcc = 0.f;
			m_iAttackCount = 0;
			m_dwActionState = AVOID;
			break;
		}

		if (m_fTimeAcc > 0.8f)
		{
			CSkill_Manager::Get_Instance()->InActive_N3();
			m_fTimeAcc = 0.f;
			m_iAttackCount = 0;
			m_dwActionState = IDLE;
		}

		break;

	case AVOID:
	{
		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(AVOID);

		CSkill_Manager::Get_Instance()->Active_Space();

		static _bool bAvoidLock = false;

		m_fTimeAcc += DeltaTime;

		if (true == m_bisIn && false == bAvoidLock)
		{
			m_pTransform->Move_Back(DeltaTime * 1.3f);
		}

		if (true == m_pCollision_Manager->Restrict_Move())
		{
			m_pTransform->Move_Back(DeltaTime * -1.3f);
			bAvoidLock = true;
		}

		if (m_fTimeAcc > 0.6f)
		{
			bAvoidLock = false;
			m_fTimeAcc = 0.f;
			m_dwActionState = IDLE;
		}
		break;
	}
	case DASH:
	{
		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(DASH);

		CSkill_Manager::Get_Instance()->Active_Q();

		m_fTimeAcc += DeltaTime;

		static _bool bDashLock = false;
		static float fDelta = 3.f;

		fDelta -= DeltaTime * 1.5f;

		if (true == m_bisIn && false == bDashLock)
		{
			if (m_fTimeAcc > 0.2f && m_fTimeAcc < 0.7f)
			{
				m_pTransform->Move_Front(DeltaTime * fDelta);
			}
		}

		if (true == m_pCollision_Manager->Restrict_Move())
		{
			if (m_fTimeAcc > 0.2f && m_fTimeAcc < 0.7f)
			{
				m_pTransform->Move_Front(DeltaTime * -fDelta);
			}
			bDashLock = true;
		}

		if (m_fTimeAcc > 0.8f)
		{
			bDashLock = false;
			fDelta = 3.f;
			m_fTimeAcc = 0.f;
			m_dwActionState = IDLE;
		}

		break;
	}
	case SKILL1:

		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(SKILL1);

		CSkill_Manager::Get_Instance()->Active_W();

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 0.8f && false == m_bCreateLock)
		{
			_float4 vPos = _float4(m_vCurrentPos, 5.f);

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Effect_Crack"), TEXT("Layer_Effect"), &vPos);
			m_bCreateLock = true;
		}

		if (m_fTimeAcc > 1.3f)
		{
			m_bCreateLock = false;
			m_fTimeAcc = 0.f;
			m_dwActionState = IDLE;
		}

		break;

	case SKILL2:

		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(SKILL2);

		CSkill_Manager::Get_Instance()->Active_E();

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 1.3f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = IDLE;
		}

		break;

	case SKILL3_1:

		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(SKILL3_1);

		CSkill_Manager::Get_Instance()->Active_R();
		CSound_Manager::Get_Instance()->PlaySound(L"../Bin/Sound/Elf_3.mp3", CSound_Manager::CHANNELID::EFFECT);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 0.4f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = SKILL3_2;
		}

		break;

	case SKILL3_2:

		if (true == Player_Die()) { break; }
		if (true == Hit_Boss()) { break; }

		m_pMesh->Set_AnimationIndex(SKILL3_2);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 0.7f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = IDLE;
		}

		break;

	case SKILL4:

		m_pMesh->Set_AnimationIndex(SKILL4);

		CSkill_Manager::Get_Instance()->Active_F();

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 2.1f && false == m_bCreateLock)
		{
			_float4 vPos = _float4(m_vCurrentPos, 5.f);

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Effect_Crack"), TEXT("Layer_Effect"), &vPos);
			m_bCreateLock = true;
		}

		if (m_fTimeAcc > 2.4f)
		{
			m_fTimeAcc = 0.f;
			m_bCreateLock = false;
			m_dwActionState = IDLE;
		}

		break;

	case KNOCKBACK:

		if (true == Player_Die()) { break; }

		m_pMesh->Set_AnimationIndex(KNOCKBACK);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc < 0.8f && true == m_bisIn)
		{
			m_pTransform->Move_Back((powf(512, -m_fTimeAcc)) / 8.f);
		}

		if (m_fTimeAcc > 1.0f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = IDLE;
		}

		break;

	case AIRBORNE1:

		if (true == Player_Die()) { break; }

		m_pMesh->Set_AnimationIndex(AIRBORNE1);

		m_fTimeAcc += DeltaTime;

		if (true == m_bisIn)
		{
			m_vCurrentPos.y = (55 * (sinf(D3DXToRadian(45.f)) * m_fTimeAcc * 10) - (0.5f * 9.81f * m_fTimeAcc * 10 * m_fTimeAcc * 10)) / 100 + 20.45f;
			m_pTransform->Set_State(CTransform::STATE_POSITION, m_vCurrentPos);
			m_pTransform->Move_Back(DeltaTime * 1.5f);
		}

		if (m_fTimeAcc > 0.8f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = AIRBORNE2;
		}

		break;

	case AIRBORNE2:
		m_pMesh->Set_AnimationIndex(AIRBORNE2);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 0.2f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = AIRBORNE3;
		}

		break;

	case AIRBORNE3:
		m_pMesh->Set_AnimationIndex(AIRBORNE3);

		m_fTimeAcc += DeltaTime;

		if (false == CSkill_Manager::Get_Instance()->IsActive_D())
		{
			if (pKey->Key_Down(KEY_D))
			{
				Look_Cursor(AWAKE);
				m_fTimeAcc = 0.f;
				CSkill_Manager::Get_Instance()->Active_D();
				break;
			}
		}

		if (m_fTimeAcc > 1.f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = AIRBORNE4;
		}

		break;

	case AIRBORNE4:
		m_pMesh->Set_AnimationIndex(AIRBORNE4);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc > 0.4f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = IDLE;
		}

		break;

	case DIE1:
		m_pMesh->Set_AnimationIndex(DIE1);

		m_fTimeAcc += DeltaTime;

		if (m_fTimeAcc < 0.8f && true == m_bisIn)
		{
			m_pTransform->Move_Back((powf(512, -m_fTimeAcc)) / 8.f);
		}

		if (m_fTimeAcc > 1.f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = DIE2;
		}

		break;

	case DIE2:
		m_pMesh->Set_AnimationIndex(DIE2);
		break;

	case AWAKE:
		m_pMesh->Set_AnimationIndex(AVOID);

		m_fTimeAcc += DeltaTime;

		if (true == m_bisIn)
		{
			m_pTransform->Move_Back(DeltaTime * 1.5f);
		}

		if (m_fTimeAcc > 0.6f)
		{
			m_fTimeAcc = 0.f;
			m_dwActionState = IDLE;
		}

		break;
	}

	m_pCollision_Manager->Set_PlayerState(m_dwActionState);
}

void CPlayer::Look_Cursor(DWORD dwActionState, _bool bMoveState, _bool bClick)
{
	_float3* pPickingPos = m_pPicking->Compute_PickingPoint(m_pNavigation, m_vCurrentPos);

	if (nullptr != pPickingPos)
	{
		m_vTargetPos = *pPickingPos;

		if (true == bMoveState)
		{
			m_pTransform->Set_MovingState(true);
		}
		m_pTransform->Look_Target(*pPickingPos);
		m_dwActionState = dwActionState;
	}
}

void CPlayer::Sync_Camera()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform"));

	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_float3 vUp = m_pTransform->Get_State(CTransform::STATE_UP);
	_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	_float3 vCamRight = pCameraTransform->Get_State(CTransform::STATE_RIGHT);
	_float3 vCamUp = pCameraTransform->Get_State(CTransform::STATE_UP);
	_float3 vCamLook = pCameraTransform->Get_State(CTransform::STATE_LOOK);

	D3DXVec3Normalize(&vCamRight, &vCamRight);
	D3DXVec3Normalize(&vCamUp, &vCamUp);
	D3DXVec3Normalize(&vCamLook, &vCamLook);

	_float3 vNewRight;
	D3DXVec3Cross(&vNewRight, &vUp, &vCamLook);
	D3DXVec3Normalize(&vNewRight, &vNewRight);

	_float3 vNewLook;
	D3DXVec3Cross(&vNewLook, &vNewRight, &vUp);
	D3DXVec3Normalize(&vNewLook, &vNewLook);

	_float3 vNewUp;
	D3DXVec3Cross(&vNewUp, &vNewLook, &vNewRight);
	D3DXVec3Normalize(&vNewUp, &vNewUp);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vNewRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vNewUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vNewLook);
	m_pTransform->Set_Scale(_float3(0.1f, 0.1f, 0.1f));

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Ride_Terrain()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CVIBuffer_Terrain* pVIBuffer  = (CVIBuffer_Terrain*)(pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer_Terrain")));
	CTransform*		   pTransform = (CTransform*)(pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Com_Transform")));
	const _matrix* pTerrainWorld  = pTransform->Get_WorldMatrix();

	m_pTransform->Ride_Terrain(pVIBuffer, pTerrainWorld);

	RELEASE_INSTANCE(CGameInstance);
}

_bool CPlayer::Hit_Boss()
{
	if (CSkill_Manager::Get_Instance()->IsActive_ATTACK2() && false == m_bLock_ATTACK2)
	{
		if (m_fTimeAcc_ATTACK2 > 0.7f && m_fTimeAcc_ATTACK2 < 0.9f && m_pCollider_D->Collision_OBB(m_pFlogasCollider))
		{
			m_fHP -= m_fDamage_ATTACK2;
			m_fTimeAcc = 0.f;
			m_dwActionState = KNOCKBACK;
			m_bLock_ATTACK2 = true;
			return true;
		}
	}
	else if (CSkill_Manager::Get_Instance()->IsActive_ATTACK3() && false == m_bLock_ATTACK3)
	{
		if (m_fTimeAcc_ATTACK3 > 0.4f && m_fTimeAcc_ATTACK3 < 0.8f && m_pCollider_D->Collision_OBB(m_pFlogasCollider))
		{
			m_fHP -= m_fDamage_ATTACK3;
			m_fTimeAcc = 0.f;
			m_dwActionState = AIRBORNE1;
			m_bLock_ATTACK3 = true;
			return true;
		}
	}
	else if (CSkill_Manager::Get_Instance()->IsActive_ATTACK4() && false == m_bLock_ATTACK4)
	{
		if (m_fTimeAcc_ATTACK4 > 0.4f && m_fTimeAcc_ATTACK4 < 0.8f && m_pCollider_D->Collision_OBB(m_pFlogasCollider))
		{
			m_fHP -= m_fDamage_ATTACK4;
			m_fTimeAcc = 0.f;
			m_dwActionState = AIRBORNE1;
			m_bLock_ATTACK4 = true;
			return true;
		}
	}
	else if (CSkill_Manager::Get_Instance()->IsActive_ATTACK5() && false == m_bLock_ATTACK5)
	{
		if (m_pCollider_D->Collision_OBB(m_pFlogasCollider))
		{
			m_fHP -= m_fDamage_ATTACK5;
			m_fTimeAcc = 0.f;
			m_dwActionState = KNOCKBACK;
			m_bLock_ATTACK5 = true;
			return true;
		}
	}

	return false;
}

void CPlayer::Time_Initialize(_double DeltaTime)
{
	if (true == CSkill_Manager::Get_Instance()->IsActive_ATTACK1())
	{
		m_fTimeAcc_ATTACK1 += DeltaTime;
	}
	else
	{
		m_fTimeAcc_ATTACK1 = 0.f;
		m_bLock_ATTACK1 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_ATTACK2())
	{
		m_fTimeAcc_ATTACK2 += DeltaTime;
	}
	else
	{
		m_fTimeAcc_ATTACK2 = 0.f;
		m_bLock_ATTACK2 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_ATTACK3())
	{
		m_fTimeAcc_ATTACK3 += DeltaTime;
	}
	else
	{
		m_fTimeAcc_ATTACK3 = 0.f;
		m_bLock_ATTACK3 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_ATTACK4())
	{
		m_fTimeAcc_ATTACK4 += DeltaTime;
	}
	else
	{
		m_fTimeAcc_ATTACK4 = 0.f;
		m_bLock_ATTACK4 = false;
	}
	if (true == CSkill_Manager::Get_Instance()->IsActive_ATTACK5())
	{
		m_fTimeAcc_ATTACK5 += DeltaTime;
	}
	else
	{
		m_fTimeAcc_ATTACK5 = 0.f;;
		m_bLock_ATTACK5 = false;
	}
}

_bool CPlayer::Player_Die()
{
	if (m_fHP <= 0.f)
	{
		m_fHP = 0.f;
		m_dwActionState = DIE1;
		CSkill_Manager::Get_Instance()->Player_Die();

		return true;
	}

	return false;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(pKey);
	Safe_Release(m_pMesh);
	Safe_Release(m_pShader);
	Safe_Release(m_pPicking);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);	
	Safe_Release(m_pNavigation);
	Safe_Release(pGameInstance);
	//Safe_Release(m_pCollision_Manager);
}