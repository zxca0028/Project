#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CCollision_Manager final : public CBase
{
	DECLARE_SINGLETON(CCollision_Manager);

private:

	explicit CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:

	HRESULT Add_Collider(const _tchar* pColliderTag ,CCollider* pCollider);
	HRESULT Set_Collider();
	_int    Tick(_double DeltaTime);
	_int    Late_Tick();
	HRESULT Render();

public:

	HRESULT Update_Collider(const _tchar* pColliderTag);
	
private:

	_bool m_bTriggerLock0 = true;

	HRESULT Trigger_CheckPoint11();
	_bool	m_bTriggerLock11 = false;

	HRESULT Trigger_CheckPoint7();
	_bool	m_bTriggerLock7 = false;

private:

	DWORD m_dwPlayerActionState;

public:

	void Set_PlayerState(DWORD dwPlayerActionState) { m_dwPlayerActionState = dwPlayerActionState; }
	DWORD Get_PlayerState() const					{ return m_dwPlayerActionState; }
	void FadeIn_Complete() { m_bFadeInComplete = true; }
	_bool IsFadeFinished() { return m_bFadeInComplete; }

	_bool Restrict_Move() { return m_bRestrict_Move; }
	_bool Release_Controller(){ return m_bRelease_Controller; }
	_bool m_bRestrict_Move = false;
	_bool m_bRelease_Controller = false;
	_bool m_bFadeInComplete = false;
	_bool m_bCreateLock = false;

	_float m_fTimeAcc = 0.f;

private:

	/* Player*/
	CCollider* m_pPlayer_Body_OBB		= nullptr;
	CCollider* m_pPlayer_Body_OBB_W     = nullptr;
	CCollider* m_pPlayer_Body_OBB_N		= nullptr;

	/* Monster */
	CCollider* m_pGhost_Soldier_Body_OBB  = nullptr;

	/* Boss */
	CCollider* m_pFlogas_Body_OBB = nullptr;
	CCollider* m_pFlogas_Weapon_OBB = nullptr;

	/* CheckPoint */
	CCollider* m_pCheckPoint0  = nullptr;
	CCollider* m_pCheckPoint1  = nullptr;
	CCollider* m_pCheckPoint2  = nullptr;
	CCollider* m_pCheckPoint3  = nullptr;
	CCollider* m_pCheckPoint4  = nullptr;
	CCollider* m_pCheckPoint5  = nullptr;
	CCollider* m_pCheckPoint6  = nullptr;
	CCollider* m_pCheckPoint7  = nullptr;
	CCollider* m_pCheckPoint8  = nullptr;
	CCollider* m_pCheckPoint9  = nullptr;
	CCollider* m_pCheckPoint10 = nullptr;
	CCollider* m_pCheckPoint11 = nullptr;

private:

	map<const _tchar*, CCollider*> m_Colliders;

public:

	CCollider* Find_Collider(const _tchar* pColliderTag);

public:

	virtual void Free() override;
};

END