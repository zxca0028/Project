#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:

	enum class TYPE { NORMAL, OUTLINE, TYPE_END };

protected:

	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:

	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int    Tick(_double DeltaTime);
	virtual _int    Late_Tick(_double DeltaTime);
	virtual HRESULT Render(TYPE eType = TYPE::NORMAL);
	virtual HRESULT Render_Shadow();

public:

	virtual HRESULT Add_Components(void* pArg = nullptr);
	virtual HRESULT Set_ConstTable();

public:

	_bool Render_OutLine() { return m_bRenderOutline; }

protected:

	_bool m_bRenderOutline = false;

protected:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:

	HRESULT		Add_Component(_uint iLevelIndex, wstring pPrototypeTag, wstring pComponentTag, class CComponent** ppOut, void* pArg = nullptr);
	CComponent* Find_Component(wstring pComponentTag);

private:

	typedef unordered_map<wstring, class CComponent*> COMPONENTS;
	COMPONENTS m_Components;

public:

	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END	