#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CPicking;
class CCollider;
class CRenderer;
class CTransform;
class CNavigation;
class CMesh_Dynamic;
class CGameInstance;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:

	enum ACTION 
	{ 
		IDLE        = 38,
		MOVING      = 1,
		ATTACK1		= 15,
		ATTACK2		= 14,
		ATTACK3		= 13,
		AVOID		= 44,

		DASH		= 0,
		SKILL1		= 3,
		SKILL2	    = 7,
		SKILL3_1	= 16,
		SKILL3_2	= 6,
		SKILL4		= 9,

		KNOCKBACK   = 39,

		AIRBORNE1   = 18,
		AIRBORNE2   = 20,
		AIRBORNE3   = 22,
		AIRBORNE4   = 21,

		DIE1		= 41,
		DIE2		= 42,

		AWAKE		= 999,

		ACTION_END 
	};

private:

	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;
	virtual HRESULT Render_ShadowDepth();

private:

	CShader*		m_pShader          = nullptr;
	CPicking*	    m_pPicking         = nullptr;
	CRenderer*		m_pRenderer        = nullptr;
	CTransform*		m_pTransform       = nullptr;
	CNavigation*    m_pNavigation	   = nullptr;
	CMesh_Dynamic*  m_pMesh            = nullptr;

private:

	CGameInstance*			  pGameInstance		   = nullptr;
	class CKeyManager*		  pKey				   = nullptr;
	class CCollision_Manager* m_pCollision_Manager = nullptr;

private:

	_float3 m_vTargetPos;
	DWORD	m_dwActionState = 0;

	_bool  m_bisIn         = true;

	_bool m_bEmotionCreate  = false;
	_float m_EmotionTimeAcc = 0.f;

private:

	void Ride_Navigation();
	HRESULT Create_Emotion(_double DeltaTime);

	void Player_Behavior(_double DeltaTime);
	_float m_fTimeAcc = 0.f;
	_uint m_iAttackCount = 0;
	_float3 m_vCurrentPos;
	_bool m_bIntro = false;
	_bool m_bNaviLock = false;

	void Look_Cursor(DWORD dwActionState, _bool bMoveState = false, _bool bClick = false);


	_bool m_bCreateLock = false;

private:

	_float m_fDamage_ATTACK1 = 0.f; /* ÆÈ ³»·ÁÂï±â */
	_float m_fDamage_ATTACK2 = 1000.f; /* Ä®·Î Âî¸£±â */
	_float m_fDamage_ATTACK3 = 3500.f; /* Ä® ÈÖµÎ¸£±â */
	_float m_fDamage_ATTACK4 = 4000.f; /* Ä® ÈÖµÎ¸£±â */
	_float m_fDamage_ATTACK5 = 500.f; /* ¹ß·Î ³»·ÁÂï±â */

	_float m_fTimeAcc_ATTACK1 = 0.f;
	_float m_fTimeAcc_ATTACK2 = 0.f;
	_float m_fTimeAcc_ATTACK3 = 0.f;
	_float m_fTimeAcc_ATTACK4 = 0.f;
	_float m_fTimeAcc_ATTACK5 = 0.f;

	_bool m_bLock_ATTACK1 = false;
	_bool m_bLock_ATTACK2 = false;
	_bool m_bLock_ATTACK3 = false;
	_bool m_bLock_ATTACK4 = false;
	_bool m_bLock_ATTACK5 = false;

	CCollider* m_pFlogasCollider = nullptr;
	CCollider* m_pCollider_D     = nullptr;

	_bool Hit_Boss();
	_bool Player_Die();

	void Time_Initialize(_double DeltaTime);

public:

	_float m_fHP = 10000.f;

	void Set_PlayerHP(_float fHPDelta) { m_fHP -= fHPDelta; }

private:

	void	Ride_Terrain();
	void	Sync_Camera();

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components(void* pArg = nullptr);

public:

	static  CPlayer*	 Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free()						 override;

};

END