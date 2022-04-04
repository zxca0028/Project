#pragma once

#include "Base.h"

BEGIN(Client)

class CGameObject abstract : public CBase
{
protected:

	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CGameObject() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int    Tick	 (_double DeltaTime);
	virtual _int    Late_Tick(_double DeltaTime);
	virtual HRESULT Render();

public:

	virtual HRESULT Add_Components(void* pArg = nullptr);
	virtual HRESULT Set_ConstTable();

public:

	HRESULT Add_Component(wstring ComponentTag, class CComponent* pComponent);

public:

	_bool   IsValid()					  { return m_bIsValid; }
	void    OnNotify(_bool bIsGameOver)   { m_bIsGameOver   = bIsGameOver; }
	void	Set_LayerTag(wstring LayerTag){ m_strMyLayerTag = LayerTag; }
	void	OnNotify(LEVEL eLevel)		  { m_eLevelIndex	= eLevel; }

public:

	class CComponent* Find_Component(wstring ComponentTag);

private:

	unordered_map<wstring, class CComponent*> m_Components;

protected:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

	_bool m_bIsValid    = true;
	_bool m_bIsGameOver = false;

	wstring m_strMyLayerTag;
	LEVEL   m_eLevelIndex = LEVEL_END;

protected:

	HRESULT Add_DeleteQueue(wstring LayerTag, CGameObject* pGameObject);

public:

	virtual void Free() override;

};

END