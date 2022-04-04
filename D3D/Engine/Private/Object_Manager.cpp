#include "Object_Manager.h"
#include "Layer.h"
#include "Component.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{}

HRESULT CObject_Manager::Reserve_Container(_uint iNumLevel)
{
	if (nullptr != m_pLayers)
	{
		return E_FAIL;
	}

	m_pLayers = new LAYERS[iNumLevel];

	m_iNumLevel = iNumLevel;

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == pPrototype || nullptr != Find_Prototype(pPrototypeTag))
	{
		return E_FAIL;
	}

	m_pPrototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg)
{
	if (m_iNumLevel <= iLevelIndex)
	{
		return E_FAIL;
	}

	CGameObject* pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
	{
		return E_FAIL;
	}

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
	{
		return E_FAIL;
	}

	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		if (nullptr == pLayer) { return E_FAIL; }

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
	{
		pLayer->Add_GameObject(pGameObject);
	}

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pGameObject)
{
	if (m_iNumLevel <= iLevelIndex)
	{
		return E_FAIL;
	}

	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		if (nullptr == pLayer) { return E_FAIL; }

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
	{
		pLayer->Add_GameObject(pGameObject);
	}

	return S_OK;
}

_int CObject_Manager::Tick(_double DeltaTime)
{
	for (_uint i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& rPair : m_pLayers[i])
		{
			rPair.second->Tick(DeltaTime);
		}
	}

	return _int();
}

_int CObject_Manager::Late_Tick(_double DeltaTime)
{
	for (_uint i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& rPair : m_pLayers[i])
		{
			rPair.second->Late_Tick(DeltaTime);
		}
	}

	return _int();
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}

	m_pLayers[iLevelIndex].clear();
}

CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer) 
	{
		return nullptr; 
	}

	return pLayer->Get_Component(pComponentTag, iIndex);
}

CGameObject* CObject_Manager::Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		return nullptr;
	}

	return pLayer->Get_GameObject(iIndex);
}

HRESULT CObject_Manager::Delete_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		return E_FAIL;
	}

	return pLayer->Delete_GameObject(iIndex);
}

HRESULT CObject_Manager::Delete_GameObject()
{
	for (_uint i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (FAILED(Pair.second->Delete_GameObject()))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CObject_Manager::Add_DeleteContainer(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pDeleteGameObject)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		return E_FAIL;
	}

	return pLayer->Add_DeleteContainer(pDeleteGameObject);
}

_uint CObject_Manager::Get_LayerSize(_uint iLevelIndex, const _tchar* pLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		return E_FAIL;
	}

	return pLayer->Get_LayerSize();
}

CGameObject* CObject_Manager::Find_Prototype(const _tchar* pPrototypeTag)
{
	auto iter = find_if(m_pPrototypes.begin(), m_pPrototypes.end(), CTagFinder(pPrototypeTag));

	if (iter == m_pPrototypes.end())
	{
		return nullptr;
	}

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (m_iNumLevel <= iLevelIndex)
	{
		return nullptr;
	}

	auto iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
	{
		return nullptr;
	}

	return iter->second;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& rPair : m_pLayers[i])
		{
			Safe_Release(rPair.second);
		}

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& rPair : m_pPrototypes)
	{
		Safe_Release(rPair.second);
	}

	m_pPrototypes.clear();
}