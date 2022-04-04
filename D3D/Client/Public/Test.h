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

class CTest final : public CGameObject
{
private:

	explicit CTest(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTest(const CTest& rhs);
	virtual ~CTest() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	CShader*      m_pShaderCom    = nullptr;
	CRenderer*    m_pRendererCom  = nullptr;
	CTransform*   m_pTransform    = nullptr;
	CMesh_Static* m_pMeshCom	  = nullptr;

private:

	_matrix		   m_OriginMatrix;
	const _matrix* m_pParentBoneMatrix;
	const _matrix* m_pParentWorldMatrix;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components(void* pArg = nullptr);
	void    ImGui_Test();
	void    Controller(_double DeltaTime);

public:

	static  CTest* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free()						 override;

};

END