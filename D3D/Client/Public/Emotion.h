#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEmotion final : public CGameObject
{
private:

	explicit CEmotion(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEmotion(const CEmotion& rhs);
	virtual ~CEmotion() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double TimeDelta)		 override;
	virtual _int    Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render()					 override;

private:

	CShader*        m_pShader    = nullptr;
	CTexture*       m_pTexture   = nullptr;
	CRenderer*      m_pRenderer  = nullptr;
	CTransform*		m_pTransform = nullptr;
	CVIBuffer_Rect* m_pVIBuffer  = nullptr;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components(void* pArg = nullptr);

private:

	_bool m_bCreate = false;
	_float m_fTime  = 0.f;
	_float m_fScale = 0.f;
	_float m_fFrame = 0.f;
	_bool  m_bGIF   = false;

private:

	_matrix m_WorldMatrix;

	
public:

	static  CEmotion*    Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void		 Free() override;

};

END