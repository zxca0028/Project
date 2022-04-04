#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CMesh_Dynamic;
END

BEGIN(Client)

class CMonster_Test final : public CGameObject
{
private:

	explicit CMonster_Test(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMonster_Test(const CMonster_Test& rhs);
	virtual ~CMonster_Test() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	CShader*       m_pShader     = nullptr;
	CRenderer*     m_pRenderer   = nullptr;
	CTransform*    m_pTransform  = nullptr;
	CMesh_Dynamic* m_pMesh		 = nullptr;

	DWORD m_dwPlayerAction = 0;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components(void* pArg = nullptr);

public:

	static  CMonster_Test*    Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free()						 override;

};

END