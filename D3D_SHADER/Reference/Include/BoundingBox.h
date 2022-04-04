#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBoundingBox final : public CComponent
{
public:

	typedef struct tagBoxDesc
	{
		const _matrix* pParentMatrix;

		_float3 vScale = _float3(1.f, 1.f, 1.f);

	}BOXDESC;

private:

	explicit CBoundingBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBoundingBox(const CBoundingBox& rhs);
	virtual ~CBoundingBox() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()	 override;
	virtual HRESULT NativeConstruct(void* pArg)	 override;
	
public:

	void Update_Box();

private:

	BOXDESC m_BoxDesc;

	_matrix m_OriginalMatrix;
	_matrix m_ParentMatrix;

	_float3 m_vMin;
	_float3 m_vMax;

private:

	class CTransform* m_pTransform;

public:

	static  CBoundingBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END