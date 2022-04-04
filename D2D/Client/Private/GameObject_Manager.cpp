#include "pch.h"
#include "GameObject_Manager.h"
#include "Layer.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(CGameObject_Manager);

CGameObject_Manager::CGameObject_Manager()
{
}

HRESULT CGameObject_Manager::Add_GameObject(wstring LayerTag, class CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
	{
		return E_FAIL;
	}
	
	CLayer* pLayer = Find_Layer(LayerTag);
	
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
	
		if (nullptr == pLayer)
		{
			return E_FAIL;
		}
	
		pLayer->Add_GameObject(pGameObject);
	
		m_Layers.emplace(LayerTag, pLayer);
	}
	else
	{
		pLayer->Add_GameObject(pGameObject);
	}

	pGameObject->Set_LayerTag(LayerTag);

	return S_OK;
}

_int CGameObject_Manager::Tick(_double DeltaTime)
{
   	for (auto& Pair : m_Layers)
	{
		Pair.second->Tick(DeltaTime);
	}

	return _int();
}

_int CGameObject_Manager::Late_Tick(_double DeltaTime)
{
	for (auto& Pair : m_Layers)
	{
		Pair.second->Late_Tick(DeltaTime);
	}

	return _int();
}

CComponent* CGameObject_Manager::Get_ComponentPtr(wstring LayerTag, wstring ComponentTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(LayerTag);

	if (nullptr == pLayer)
	{
		return nullptr;
	}

	return pLayer->Get_ComponentPtr(ComponentTag, iIndex);
}

HRESULT CGameObject_Manager::Add_DeleteQueue(wstring LayerTag, CGameObject* pGameObject)
{
	CLayer* pLayer = Find_Layer(LayerTag);

	if (nullptr == pLayer)
	{
		return E_FAIL;
	}

	return pLayer->Add_DeleteQueue(pGameObject);
}

HRESULT CGameObject_Manager::Delete_GameObject()
{
	for (auto& Pair : m_Layers)
	{
		Pair.second->Delete_GameObject();
	}

	return S_OK;
}

void CGameObject_Manager::OnNotify(_bool bIsGameOver)
{
	for (auto& Pair : m_Layers)
	{
		Pair.second->OnNotify(bIsGameOver);
	}
}

void CGameObject_Manager::OnNotify(LEVEL eLevel)
{
	for (auto& Pair : m_Layers)
	{
		Pair.second->OnNotify(eLevel);
	}
}

CLayer* CGameObject_Manager::Find_Layer(wstring LayerTag)
{
	auto iter = m_Layers.find(LayerTag);
	
	if (iter == m_Layers.end())
	{
		return nullptr;
	}
	
	return iter->second;
}

void CGameObject_Manager::Free()
{
	for (auto& Pair : m_Layers)
	{
		Safe_Release(Pair.second);
	}
	
	m_Layers.clear();
}

END