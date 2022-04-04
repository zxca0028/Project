#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CSkyBox final : public CGameObject
{
private:

	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()	      override;
	virtual HRESULT NativeConstruct(void* pArg)	      override;
	virtual _int    Tick	 (_double DeltaTime)      override;
	virtual _int    Late_Tick(_double DeltaTime)      override;
	virtual HRESULT Render(TYPE eType = TYPE::NORMAL) override;

public:

	virtual HRESULT Add_Components(void* pArg=nullptr) override;
	virtual HRESULT Set_ConstTable()				   override;

private:

	CShader*		m_pShader    = nullptr;
	CTexture*		m_pTexture   = nullptr;
	CRenderer*		m_pRenderer  = nullptr;
	CTransform*		m_pTransform = nullptr;
	CVIBuffer_Cube* m_pVIBuffer  = nullptr;

public:

	static  CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END