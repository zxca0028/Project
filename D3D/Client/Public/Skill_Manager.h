#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CSkill_Manager final : public CBase
{
	DECLARE_SINGLETON(CSkill_Manager)

private:

	explicit CSkill_Manager();
	virtual ~CSkill_Manager() = default;

public:

	void Player_Die() { m_bIsDead = true; }
	_bool IsPlayer_Die() { return m_bIsDead; }

	_bool m_bIsDead = false;


public:

	void Active_N1() { m_bActive_N1 = true; }
	void Active_N2() { m_bActive_N2 = true; }
	void Active_N3() { m_bActive_N3 = true; }
	void Active_Q() { m_bActive_Q = true; }
	void Active_W() { m_bActive_W = true; }
	void Active_E() { m_bActive_E = true; }
	void Active_R() { m_bActive_R = true; }
	void Active_D() { m_bActive_D = true; }
	void Active_F() { m_bActive_F = true; }
	void Active_Space() { m_bActive_Space = true; }

	void InActive_N1() { m_bActive_N1 = false; }
	void InActive_N2() { m_bActive_N2 = false; }
	void InActive_N3() { m_bActive_N3 = false; }
	void InActive_Q() { m_bActive_Q = false; }
	void InActive_W() { m_bActive_W = false; }
	void InActive_E() { m_bActive_E = false; }
	void InActive_R() { m_bActive_R = false; }
	void InActive_D() { m_bActive_D = false; }
	void InActive_F() { m_bActive_F = false; }
	void InActive_Space() { m_bActive_Space = false; }

	_bool IsActive_N1() { return m_bActive_N1; }
	_bool IsActive_N2() { return m_bActive_N2; }
	_bool IsActive_N3() { return m_bActive_N3; }
	_bool IsActive_Q() { return m_bActive_Q; }
	_bool IsActive_W() { return m_bActive_W; }
	_bool IsActive_E() { return m_bActive_E; }
	_bool IsActive_R() { return m_bActive_R; }
	_bool IsActive_D() { return m_bActive_D; }
	_bool IsActive_F() { return m_bActive_F; }
	_bool IsActive_Space() { return m_bActive_Space; }

private:

	_bool m_bActive_N1 = false;
	_bool m_bActive_N2 = false;
	_bool m_bActive_N3 = false;
	_bool m_bActive_Q = false;
	_bool m_bActive_W = false;
	_bool m_bActive_E = false;
	_bool m_bActive_R = false;
	_bool m_bActive_D = false;
	_bool m_bActive_F = false;
	_bool m_bActive_Space = false;

public:
	
	void Active_ATTACK1() { m_bATTACK1 = true; }
	void Active_ATTACK2() { m_bATTACK2 = true; }
	void Active_ATTACK3() { m_bATTACK3 = true; }
	void Active_ATTACK4() { m_bATTACK4 = true; }
	void Active_ATTACK5() { m_bATTACK5 = true; }
	void InActive_ATTACK1() { m_bATTACK1 = false; }
	void InActive_ATTACK2() { m_bATTACK2 = false; }
	void InActive_ATTACK3() { m_bATTACK3 = false; }
	void InActive_ATTACK4() { m_bATTACK4 = false; }
	void InActive_ATTACK5() { m_bATTACK5 = false; }
	_bool IsActive_ATTACK1() { return m_bATTACK1; }
	_bool IsActive_ATTACK2() { return m_bATTACK2; }
	_bool IsActive_ATTACK3() { return m_bATTACK3; }
	_bool IsActive_ATTACK4() { return m_bATTACK4; }
	_bool IsActive_ATTACK5() { return m_bATTACK5; }

private:

	_bool m_bATTACK1 = false;
	_bool m_bATTACK2 = false;
	_bool m_bATTACK3 = false;
	_bool m_bATTACK4 = false;
	_bool m_bATTACK5 = false;

public:

	void MoveStart() { m_bisMove = true; }
	_bool IsMoveStart() { return m_bisMove; }
	_bool m_bisMove = false;

public:

	void MonsterAttack() { m_bMonsterAttack = true; }
	_bool IsMonsterAttack() { return m_bMonsterAttack; }
	_bool m_bMonsterAttack = false;

public:

	virtual void Free() override;

};

END