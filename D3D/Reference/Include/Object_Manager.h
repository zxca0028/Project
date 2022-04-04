#pragma once

#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)

private:

	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;

public:

	HRESULT Reserve_Container(_uint iNumLevel);
	HRESULT Add_GameObject_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, class CGameObject* pGameObject);
	_int	Tick(_double DeltaTime);
	_int	Late_Tick(_double DeltaTime);
	void    Clear(_uint iLevelIndex);

	class CComponent*  Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);
	HRESULT Delete_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);
	HRESULT Delete_GameObject();
	HRESULT Add_DeleteContainer(_uint iLevelIndex, const _tchar* pLayerTag, class CGameObject* pDeleteGameObject);
	_uint   Get_LayerSize(_uint iLevelIndex, const _tchar* pLayerTag);

private:

	typedef map<const _tchar*, class CLayer*>	   LAYERS;
	typedef map<const _tchar*, class CGameObject*> PROTOTYPES;

	LAYERS*		m_pLayers = nullptr;
	PROTOTYPES  m_pPrototypes;

	_uint		m_iNumLevel	  = 0;

public: /* private */

	CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	CLayer*		 Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:

	virtual void Free() override;

};

END