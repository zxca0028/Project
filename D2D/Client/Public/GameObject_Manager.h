#pragma once

#include "Base.h"

BEGIN(Client)

class CGameObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CGameObject_Manager);

private:

	explicit CGameObject_Manager();
	virtual ~CGameObject_Manager() = default;

public:

	HRESULT Add_GameObject(wstring LayerTag, class CGameObject* pGameObject);
	HRESULT Add_DeleteQueue(wstring LayerTag, class CGameObject* pGameObject);
	HRESULT Delete_GameObject();
	_int    Tick	 (_double DeltaTime);
	_int    Late_Tick(_double DeltaTime);

public:

	class CComponent* Get_ComponentPtr(wstring LayerTag, wstring ComponentTag, _uint iIndex);

public:

	void  OnNotify(_bool bIsPlayerAlive);
	void  OnNotify(LEVEL eLevel);

private:

	unordered_map<wstring, class CLayer*> m_Layers;

private:

	class CLayer* Find_Layer(wstring LayerTag);

public:

	virtual void Free() override;

};

END