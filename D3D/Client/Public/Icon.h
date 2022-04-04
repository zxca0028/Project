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

class CIcon final : public CGameObject
{
public:

	typedef struct tagIconInfo
	{
		const _matrix* WorldMatrix = nullptr;
		_float*		   fObjectHP   = nullptr;
		_bool*		   bRender	   = nullptr;

	}ICONINFO;

private:

	explicit CIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CIcon(const CIcon& rhs);
	virtual ~CIcon() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double TimeDelta)		 override;
	virtual _int    Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render()					 override;

private:

	CShader* m_pShader = nullptr;
	CTexture* m_pTexture = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTransform* m_pTransform = nullptr;
	CVIBuffer_Rect* m_pVIBuffer = nullptr;

private:

	_matrix m_WorldMatrix;
	_matrix m_ViewMatrix;
	_matrix	m_ProjMatrix;

	_float  m_fX, m_fY, m_fSizeX, m_fSizeY;


	_float m_fTest = 0.5f;

	ICONINFO m_IconInfo;
	_float m_fObjectMaxHP = 0.f;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components();

public:

	static  CIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void		 Free() override;

};

END