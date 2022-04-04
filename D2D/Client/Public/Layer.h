#pragma once

#include "GameObject.h"

BEGIN(Client)

class CLayer final : public CBase
{
private:

	explicit CLayer();
	virtual ~CLayer() = default;
	
public:

	HRESULT NativeConstruct();
	_int    Tick	  (_double DeltaTime);
	_int    Late_Tick(_double DeltaTime);

public:

	HRESULT Add_GameObject(CGameObject* pGameObject);
	HRESULT Add_DeleteQueue(CGameObject* pGameObject);
	HRESULT Delete_GameObject();

public:

	class CComponent* Get_ComponentPtr(wstring ComponentTag, _uint iIndex);

public:

	void OnNotify(_bool bIsPlayerAlive);
	void OnNotify(LEVEL eLevel);

private:

	list<CGameObject*>  m_GameObjects;
	queue<CGameObject*> m_InvalidGameObjects;

private:

	_uint m_iTickCount = 0;

public:

	static  CLayer* Create();
	virtual void Free() override;

};

END