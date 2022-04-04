#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CFrustum;
class CRenderer;
class CCollider;
class CTransform;
class CNavigation;
class CMesh_Dynamic;
END

BEGIN(Client)

class CSkeleton final : public CGameObject
{
public:

	enum ACTION
	{
		CHASE = 13,

		BATTLE_IDLE = 14,

		HIT = 15,

		ATTACK1 = 0,
		ATTACK2 = 12,
		ATTACK3 = 19,

		KNOCKBACK = 99,

		AIRBORNE1 = 9,
		AIRBORNE2 = 7,
		AIRBORNE3 = 6,

		DIE1 = 17,
		DIE2 = 18,

		CREATE = 98,

		PLAYER_DIE = 1111,

		ACTION_END
	};

private:

	explicit CSkeleton(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSkeleton(const CSkeleton& rhs);
	virtual ~CSkeleton() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	class CRandom* m_pRandom = nullptr;
	CShader* m_pShader = nullptr;
	CFrustum* m_pFrustum = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CCollider* m_pCollider = nullptr;
	CCollider* m_pPlayer_W = nullptr;
	CCollider* m_pPlayer_N = nullptr;
	CCollider* m_pPlayer_D = nullptr;
	CTransform* m_pTransform = nullptr;
	CNavigation* m_pNavigation = nullptr;
	CMesh_Dynamic* m_pMesh = nullptr;

private:

	class CCollision_Manager* m_pCollision_Manager = nullptr;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components(void* pArg = nullptr);

private:

	void Monster_Behavior(_double DeltaTime);

private:

	DWORD m_dwActionState;
	DWORD m_dwPlayerActionState;
	_float3 m_vPlayerPos;
	_float3 m_vMyPos;
	_float3 m_vDir;
	_float m_fDist;
	_float m_fTimeAcc = 0.f;
	_bool m_bisIn = false;
	_bool Hit_Q(_double DeltaTime);
	_bool Hit_W(_double DeltaTime);
	_bool Hit_E(_double DeltaTime);
	_bool Hit_R(_double DeltaTime);
	_float m_fTimeAcc_Q = 0.f;
	_float m_fTimeAcc_W = 0.f;
	_float m_fTimeAcc_E = 0.f;
	_float m_fTimeAcc_R = 0.f;
	_uint  m_dwTimeAcc_N = 0;
	_float m_fRandomFloat = 0.f;

	_bool Hit_N(_double DeltaTime, _bool bActionChange = true);

	_float m_fHP = 100.f;

	_bool m_bDamageChecker = false;

	_bool m_bLock_Q = false;
	_bool m_bLock_W1 = false;
	_bool m_bLock_W2 = false;
	_bool m_bLock_W3 = false;
	_bool m_bLock_W4 = false;
	_bool m_bLock_W5 = false;
	_bool m_bLock_E1 = false;
	_bool m_bLock_E2 = false;
	_bool m_bLock_R1 = false;
	_bool m_bLock_R2 = false;
	_bool m_bLock_R3 = false;

	_bool m_bLock_N1 = false;
	_bool m_bLock_N2 = false;
	_bool m_bLock_N3 = false;

	_float m_fTimeAcc_N1 = 0.f;
	_float m_fTimeAcc_N2 = 0.f;
	_float m_fTimeAcc_N3 = 0.f;

	_float m_fDamage_N1 = 5.f;
	_float m_fDamage_N2 = 8.f;
	_float m_fDamage_N3 = 10.f;
	_float m_fDamage_Q = 30.f;
	_float m_fDamage_W = 60.f;
	_float m_fDamage_E = 50.f;
	_float m_fDamage_R = 25.f;

	_bool m_bActivate = false;

	_bool m_bCreateLock = false;
	
	_bool Hit_F(_double DeltaTime);
	_bool m_bLock_F1 = false;
	_bool m_bLock_F2 = false;
	_bool m_bLock_F3 = false;
	_bool m_bLock_F4 = false;
	_bool m_bLock_F5 = false;
	_bool m_bLock_F6 = false;
	_bool m_bLock_F7 = false;
	_float m_fTimeAcc_F = 0.f;
	_float m_fDamage_F = 20.f;
	void Time_Initailize(_double DeltaTime);

	void CreateEffect(const _tchar* pPrototypeTag, _float fScale, _bool isRandom = true, _bool isPtr = false);

	_float4 m_vPos;

	class CPlayer* m_pPlayer = nullptr;

	_bool m_bAttackLock = false;
	void Attack_Player(_float fTime, _float fDamage);
	_float m_fDamage = 100.f;

	_bool m_bMotionLock = false;

public:

	static  CSkeleton* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free()						 override;

};

END