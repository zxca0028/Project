#pragma once

#include "GameObject.h"

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:

	enum STATE { STATE_IDLE, STATE_JUMP, STATE_END };

private:

	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayer() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

public:

	virtual HRESULT Add_Components(void* pArg = nullptr) override;
	virtual HRESULT Set_ConstTable() override;

private:

	HRESULT Player_Behavior(_double DeltaTime);
	_bool   OnCollision();

private:

	class CShader*        m_pShader    = nullptr;
	class CTexture*       m_pTexture   = nullptr;
	class CCollider*	  m_pCollider  = nullptr;
	class CTransform*     m_pTransform = nullptr;
	class CVIBuffer_Rect* m_pVIBuffer  = nullptr;

private:

	_uint  m_iState   = 0;
	_float m_fFrame   = 0.f;
	_float m_fTimeAcc = 0.f;

	_float3 m_DebugScale = _float3(1.f, 1.f, 0.f);
	_float3 m_DebugPos   = _float3(0.f, 0.f, 0.f);

private:

	_uint m_iTOP = 0;
	_uint m_iGND = 0;

public:

	static  CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual void Free() override;

};

END