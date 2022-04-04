#include "Layer.h"
#include "GameObject.h"
#include "Component.h"

CLayer::CLayer()
{}

HRESULT CLayer::NativeConstruct()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
	{
		return E_FAIL;
	}

	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double DeltaTime)
{
	for (auto& pGameObject : m_Objects)
	{
		if (false == pGameObject->IsDelete())
		{
			pGameObject->Tick(DeltaTime);
		}
	}

	return _int();
}

_int CLayer::Late_Tick(_double DeltaTime)
{
	for (auto& pGameObject : m_Objects)
	{
		if (false == pGameObject->IsDelete())
		{
			pGameObject->Late_Tick(DeltaTime);
		}
	}

	return _int();
}

CComponent* CLayer::Get_Component(const _tchar* pComponentTag, _uint iIndex)
{
	if (m_Objects.size() <= iIndex)
	{
		return nullptr;
	}

	auto iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	return (*iter)->Find_Component(pComponentTag);
}

CGameObject* CLayer::Get_GameObject(_uint iIndex)
{
	if (m_Objects.size() <= iIndex)
	{
		return nullptr;
	}

	auto iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	return *iter;
}

HRESULT CLayer::Delete_GameObject(_uint iIndex)
{
	if (m_Objects.size() <= iIndex)
	{
		return E_FAIL;
	}

	auto iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	Safe_Release(*iter);
	m_Objects.erase(iter);

	return S_OK;
}

HRESULT CLayer::Delete_GameObject()
{
	if (m_iTickCount <= 10)
	{
		++m_iTickCount;
		return S_OK;
	}

	for (auto& Object : m_DeleteObjects)
	{
		m_Objects.remove(Object);
		Safe_Release(Object);
	}

	m_DeleteObjects.clear();

	m_iTickCount = 0;

	return S_OK;
}

HRESULT CLayer::Add_DeleteContainer(class CGameObject* pDeleteGameObject)
{
	m_DeleteObjects.push_back(pDeleteGameObject);

	return S_OK;
}

_uint CLayer::Get_LayerSize()
{
	return m_Objects.size();
}

CLayer* CLayer::Create()
{
	CLayer* pInstance = new CLayer;

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating Layer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
	{
		Safe_Release(pGameObject);
	}

	m_Objects.clear();
}