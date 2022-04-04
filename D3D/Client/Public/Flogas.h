#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CFrustum;
class CRenderer;
class CCollider;
class CTransform;
class CMesh_Dynamic;
END

BEGIN(Client)

class CFlogas final : public CGameObject
{
private:

	enum ACTION
	{
		/* Walk = 10 | RUN = 11 */
		CHASE = 99,
		WALK = 10,
		RUN = 11,

		BATTLE_IDLE = 15,

		HIT = 16,

		ATTACK1 = 1,
		ATTACK2 = 2,
		ATTACK3 = 6,
		ATTACK4 = 7,
		ATTACK5 = 4,

		/* 4 -> 12 -> 15*/
		COUNTER = 98,

		DIE1 = 18,
		DIE2 = 0,

		PATTERN1 = 8,

		PHASE_CHANGE = 9,

		CREATE = 97,

		PLAYER_DIE1 = 13,
		PLAYER_DIE2 = 133,

		ACTION_END
	};

private:

	explicit CFlogas(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFlogas(const CFlogas& rhs);
	virtual ~CFlogas() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	CShader*       m_pShader    = nullptr;
	CFrustum*      m_pFrustum   = nullptr;
	CRenderer*     m_pRenderer  = nullptr;
	CCollider*	   m_pCollider  = nullptr;
	CTransform*    m_pTransform = nullptr;
	CMesh_Dynamic* m_pMesh		= nullptr;

private:

	class CCollision_Manager* m_pCollision_Manager = nullptr;
	class CRandom* m_pRandom = nullptr;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components(void* pArg = nullptr);

private:

	void Get_PlayerDesc();
	
	_float3 m_vCurrentPos;
	_float3 m_vPlayerPos;
	_float  m_fDistance;

	CCollider* m_pFlogasWeapon = nullptr;

	CCollider* m_pPlayer_W = nullptr;
	CCollider* m_pPlayer_N = nullptr;
	CCollider* m_pPlayer_D = nullptr;

private:

	void Boss_Behavior(_double DeltaTime);

	void Time_Initialize(_double DeltaTime);

	_bool m_bPatternLock1 = false;
	_bool m_bPatternLock2 = false;
	_bool m_bPatternLock3 = false;

	_uint m_iPatternCount = 0;

	DWORD  m_dwPhase = 1;
	DWORD  m_dwActionState;
	_float m_fTimeAcc = 0.f;
	_float m_fHP = 10000.f;
	_float m_fDamage_N1 = 10.f;
	_float m_fDamage_N2 = 20.f;
	_float m_fDamage_N3 = 30.f;

	_float m_fDamage_Q = 100.f;
	_float m_fDamage_W = 50.f;
	_float m_fDamage_E = 200.f;
	_float m_fDamage_R = 200.f;
	_float m_fDamage_F = 100.f;
	_float m_fTimeAcc_Q = 0.f;
	_float m_fTimeAcc_W = 0.f;
	_float m_fTimeAcc_E = 0.f;
	_float m_fTimeAcc_R = 0.f;
	_bool m_bLock_Q = false;
	_bool m_bLock_W1 = false;
	_bool m_bLock_W2 = false;
	_bool m_bLock_W3 = false;
	_bool m_bLock_W4 = false;
	_bool m_bLock_E = false;
	_bool m_bLock_R = false;
	_bool m_bLock_R1 = false;
	_bool m_bLock_R2 = false;
	_bool m_bLock_R3 = false;

	_float m_fTimeAcc_N1 = 0.f;
	_float m_fTimeAcc_N2 = 0.f;
	_float m_fTimeAcc_N3 = 0.f;
	_bool m_bLock_N1 = false;
	_bool m_bLock_N2 = false;
	_bool m_bLock_N3 = false;

	_bool Hit_Q(_double DeltaTime);
	_bool Hit_W(_double DeltaTime);
	_bool Hit_E(_double DeltaTime);
	_bool Hit_R(_double DeltaTime);
	_bool Hit_N(_double DeltaTime);
	_bool Hit_F(_double DeltaTime);

	_float m_fTimeAcc_F = 0.f;
	_bool m_bLock_F1 = false;
	_bool m_bLock_F2 = false;
	_bool m_bLock_F3 = false;
	_bool m_bLock_F4 = false;
	_bool m_bLock_F5 = false;
	_bool m_bLock_F6 = false;
	_bool m_bLock_F7 = false;

	_float m_fTimeAcc_Flame = 0.f;
	_bool m_bDeleteLock = false;

	_bool m_bMotionLock = false;

	void CreateEffect(const _tchar* pPrototypeTag, _float fScale, _bool isRandom = true, _bool isPtr = false);

	_float4 m_vPos;

	_bool m_bParticleCreateLock = false;
	_bool m_bPhaseChangeLock = false;
	_bool m_bGaugeCreateLock = false;
	_float m_fTimeAcc_GaugeCreate = 0.f;

private:

	class CTrail* m_pTrail = nullptr;

	_matrix* m_pOriginalMatrix = nullptr;
	_matrix m_OriginalMatrix;
	const _matrix* m_pWeaponMatrix = nullptr;
	const _matrix* m_pParentBoneMatrix = nullptr;

public:

	static  CFlogas*     Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free()						 override;

};

END