#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:

	explicit CLayer();
	virtual ~CLayer() = default;

public:

	HRESULT		 NativeConstruct();
	HRESULT		 Add_GameObject(CGameObject* pGameObject);
	_int		 Tick(_double DeltaTime);
	_int		 Late_Tick(_double DeltaTime);

public:

	CComponent*  Get_Component(const _tchar* pComponentTag, _uint iIndex);
	CGameObject* Get_GameObject(_uint iIndex);
	HRESULT Delete_GameObject(_uint iIndex);
	HRESULT Delete_GameObject();
	HRESULT Add_DeleteContainer(class CGameObject* pDeleteGameObject);
	_uint   Get_LayerSize();

private:

	list<class CGameObject*> m_Objects;

	_uint m_iTickCount = 0;

private:

	list<class CGameObject*> m_DeleteObjects;

public:

	static CLayer* Create();
	virtual void   Free() override;

};

END