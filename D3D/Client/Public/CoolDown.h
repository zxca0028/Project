#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CCoolDown final : public CGameObject
{
private:

	explicit CCoolDown(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCoolDown(const CCoolDown& rhs);
	virtual ~CCoolDown() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	CShader* m_pShader = nullptr;
	CTexture* m_pTexture = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CVIBuffer_Rect* m_pVIBuffer = nullptr;

private:

	_matrix m_WorldMatrix;
	_matrix m_ViewMatrix;
	_matrix	m_ProjMatrix;

	_float  m_fX, m_fY, m_fSizeX, m_fSizeY;

private:

	_float m_fAngle = 0.f;
	_bool m_bLock = false;
	_int m_iToF = 0;
	_uint m_iIndex = 0;

private:

	_float m_fTimeAcc_Q = 0.f;
	_float m_fTimeAcc_W = 0.f;
	_float m_fTimeAcc_E = 0.f;
	_float m_fTimeAcc_R = 0.f;
	_float m_fTimeAcc_D = 0.f;
	_float m_fTimeAcc_F = 0.f;
	_float m_fTimeAcc_Space = 0.f;

private:

	void CoolTime(_float fCoolTime, _double DeltaTime);

private:

	HRESULT Set_ConstTable();
	HRESULT Add_Components();
public:

	static  CCoolDown* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void		 Free() override;

};

END