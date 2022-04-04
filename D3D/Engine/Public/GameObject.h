#pragma once

#include "Base.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:

	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:

	HRESULT		Add_Component(_uint iLevelIndexm, const _tchar* pPrototypeTag, const _tchar * pComponentTag, class CComponent** ppOut, void* pArg = nullptr);

public:

	virtual HRESULT     NativeConstruct_Prototype();
	virtual HRESULT     NativeConstruct(void* pArg);
	virtual _int	    Tick(_double DeltaTime);
	virtual _int	    Late_Tick(_double DeltaTime);
	virtual HRESULT     Render();
	virtual HRESULT		Render_ShadowDepth() { return S_OK; }

public:

	_bool IsDelete() { return m_bisDelete; }

protected:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

	_bool m_bisDelete = false;

public:

	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void		 Free();


public:

	CComponent* Find_Component(const _tchar * pComponentTag);

protected:

	typedef map<const _tchar*, class CComponent*> COMPONENTS;
	COMPONENTS  m_Components;

};

END