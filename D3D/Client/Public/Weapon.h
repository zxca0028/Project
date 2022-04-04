#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CMesh_Static;
END

BEGIN(Client)

class CWeapon final : public CGameObject
{
private:

	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	CShader*      m_pShader       = nullptr;
	CShader*      m_pEffectShader = nullptr;
	CRenderer*    m_pRenderer     = nullptr;
	CTransform*   m_pTransform    = nullptr;
	CMesh_Static* m_pMesh	      = nullptr;

private:

	class CTrail* m_pTrail = nullptr;
	_uint m_iIndex = 0;

private:

	_matrix		   m_OriginMatrix;
	const _matrix* m_pParentBoneMatrix;
	const _matrix* m_pParentWorldMatrix;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components(void* pArg = nullptr);

public:

	static  CWeapon*      Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free()						 override;

};

END