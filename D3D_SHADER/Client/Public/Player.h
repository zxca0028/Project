#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CPicking;
class CRenderer;
class CCollider;
class CTransform;
class CMesh_Static;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
private:

	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()	      override;
	virtual HRESULT NativeConstruct(void* pArg)	      override;
	virtual _int    Tick	 (_double DeltaTime)      override;
	virtual _int    Late_Tick(_double DeltaTime)      override;
	virtual HRESULT Render(TYPE eType = TYPE::NORMAL) override;
	virtual HRESULT Render_Shadow() override;

private:

	virtual HRESULT Add_Components(void* pArg) override;
	virtual HRESULT Set_ConstTable()		   override;

private:

	CShader*          m_pShader    = nullptr;
	CPicking*		  m_pPicking   = nullptr;
	CRenderer*        m_pRenderer  = nullptr;
	CCollider*	      m_pCollider  = nullptr;
	CTransform*       m_pTransform = nullptr;
	CMesh_Static*     m_pMesh      = nullptr;

public:

	static  CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END