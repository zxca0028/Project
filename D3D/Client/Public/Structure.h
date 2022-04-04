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

class CStructure final : public CGameObject
{
private:

	explicit CStructure(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStructure(const CStructure& rhs);
	virtual ~CStructure() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	CShader*      m_pShaderCom   = nullptr;
	CRenderer*    m_pRendererCom = nullptr;
	CTransform*   m_pTransform   = nullptr;
	CMesh_Static* m_pMeshCom	 = nullptr;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components(void* pArg = nullptr);

public:

	static  CStructure*		 Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void		 Free()						 override;

};

END