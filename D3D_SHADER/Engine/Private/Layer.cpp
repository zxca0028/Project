#include "Layer.h"
#include "GameObject.h"
#include "Component.h"

CLayer::CLayer()
{
}

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

	m_Objects.emplace_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double DeltaTime)
{
	for (auto& pGameObject : m_Objects)
	{
		pGameObject->Tick(DeltaTime);
	}

	return _int();
}

_int CLayer::Late_Tick(_double DeltaTime)
{
	for (auto& pGameObject : m_Objects)
	{
		pGameObject->Late_Tick(DeltaTime);
	}

	return _int();
}

CComponent* CLayer::Get_ComponentPtr(wstring pComponentTag, _uint iIndex)
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

CLayer* CLayer::Create()
{
	CLayer* pInstance = new CLayer;

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Create Layer");
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
