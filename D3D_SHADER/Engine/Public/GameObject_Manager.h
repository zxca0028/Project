#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CGameObject_Manager)

private:

	explicit CGameObject_Manager();
	virtual ~CGameObject_Manager() = default;

public:

	HRESULT NativeConstruct(_uint iNumLevel);
	HRESULT Add_GameObject_Prototype(wstring pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, wstring pPrototypeTag, wstring pLayerTag, void* pArg = nullptr);
	_int    Tick(_double DeltaTime);
	_int    Late_Tick(_double DeltaTime);
	void    Clear(_uint iLevelIndex);

private:

	typedef unordered_map<wstring, class CGameObject*> PROTOTYPES;
	PROTOTYPES m_Prototypes;

	typedef unordered_map<wstring, class CLayer*> LAYERS;
	LAYERS* m_pLayers = nullptr;

private:

	_uint	m_iNumLevel = 0;

public:

	class CComponent* Get_ComponentPtr(_uint iLevelIndex, wstring pLayerTag, wstring pComponentTag, _uint iIndex);

private:

	CGameObject* Find_Prototype(wstring pPrototypeTag);
	CLayer*		 Find_Layer(_uint iLevelIndex, wstring pLayerTag);

public:

	virtual void Free() override;

};

END