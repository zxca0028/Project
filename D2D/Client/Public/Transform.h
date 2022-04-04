#pragma once

#include "Component.h"

BEGIN(Client)

class CTransform final : public CComponent
{
public:

	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:

	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTransform() = default;

public:

	HRESULT NativeConstruct(void* pArg);
	
public:

	const _matrix* Get_WorldMatrix()    const;
	const _matrix  Get_WorldMatrixInv() const;

	void Set_WorldMatrix(_matrix WorldMatrix);

	const _float3 Get_State(STATE eState) const ;
	const _float3 Get_Scale()			  const ;

	void Set_State(STATE eState, _float3 vState);
	void Set_Scale(_float3 vScale);

public:

	void Move_Up   (_double DeltaTime);
	void Move_Down (_double DeltaTime);
	void Move_Left (_double DeltaTime);
	void Move_Right(_double DeltaTime);

	void Rotation(_float fDeg);
	void Remove_Rotation();

	void Jumping(_double DeltaTime, _float fGravity, _float fPower);

private:

	_matrix m_WorldMatrix		 = {};

	_float  m_TransformPerSecond = 0.f;

public:

	static  CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual void Free() override;

};

END