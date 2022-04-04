#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:

	explicit CLayer();
	virtual ~CLayer() = default;

public:

	HRESULT NativeConstruct();
	HRESULT Add_GameObject(CGameObject* pGameObject);
	_int    Tick	 (_double DeltaTime);
	_int    Late_Tick(_double DeltaTime);

public:

	class CComponent* Get_ComponentPtr(wstring pComponentTag, _uint iIndex);
	
private:

	list<class CGameObject*> m_Objects;

public:

	static  CLayer* Create();
	virtual void Free() override;

};

END