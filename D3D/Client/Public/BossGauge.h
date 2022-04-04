#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBossGauge final : public CGameObject
{
private:

	explicit CBossGauge(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBossGauge(const CBossGauge& rhs);
	virtual ~CBossGauge() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double TimeDelta)		 override;
	virtual _int    Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render()					 override;

private:

	CShader*        m_pShader   = nullptr;
	CTexture*       m_pTexture  = nullptr;
	CRenderer*      m_pRenderer = nullptr;
	CVIBuffer_Rect* m_pVIBuffer = nullptr;

private:

	_matrix m_WorldMatrix;
	_matrix m_ViewMatrix;
	_matrix	m_ProjMatrix;

	_float  m_fX, m_fY, m_fSizeX, m_fSizeY;

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components();

private:

	float fSX, fSY, fX, fY;

	int iSizeX = 0;
	int iSizeY = 0;
	int X = 0;
	int Y = 0;

	_float m_fTimeAcc = 0.f;
	_float* m_fBossHP = nullptr;

	_float m_fMaxHP = 0.f;

public:

	static  CBossGauge*  Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void		 Free() override;

};

END