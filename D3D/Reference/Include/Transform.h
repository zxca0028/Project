#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:

	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

	typedef struct tagTransformDesc
	{
		_float fSpeedPerSec;
		_float fRotationPerSec;

	}TRANSFORMDESC;

private:

	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

private:

	_matrix		  m_WorldMatrix;
	TRANSFORMDESC m_TransformState;

public:

	void Set_MovingState(_bool IsMoving)
	{
		m_bIsMoving = IsMoving;
	}

private:

	_bool m_bIsMoving = false;

public:

	void Set_WorldMatrix(const _matrix& WorldMatrix)
	{
		m_WorldMatrix = WorldMatrix;
	}

	const _matrix* Get_WorldMatrix()	    const;
	const _matrix  Get_WorldMatrixInverse() const;
	const _float3  Get_State(STATE eState)  const;
	const _float3  Get_Scale()				const;

	void Set_State(STATE eState, _float3 vState);
	void Set_Scale(_float3 vScale);

public:

	void Move_Front(_double DeltaTime);
	void Move_Back (_double DeltaTime);
	void Move_Left (_double DeltaTime);
	void Move_Right(_double DeltaTime);
	void Move_Up   (_double DeltaTime);
	void Move_Down (_double DeltaTime);

	void Rotation_Axis		 (_float3 vAxis, _double DeltaTime);
	void Rotation_Axis_Radian(_float3 vAxis, _float vRadian);
	void Setup_Rotation      (_float3 vAxis, _float fRadian);
	void Setup_CameraAngle   (_float3 vAxis, _float fRadian);

	void Chase_Target(const CTransform* pTargetTransform, _double DeltaTime);
	void Look_Target(const CTransform* pTargetTransform);

	bool Chase_Target(_float3 vTargetPos, _double DeltaTime);
	void Look_Target (_float3 vTargetPos);

	void Ride_Terrain(class CVIBuffer_Terrain* pVIBuffer, const _matrix* pTerrainWorldMatrix);
	void Ride_Navigation(D3DXPLANE Plane);

	void Remove_Rotation();

public:	

	static CTransform*  Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void		Free() override;

};

END