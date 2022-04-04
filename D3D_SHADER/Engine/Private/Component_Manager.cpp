#include "Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::NativeConstruct(_uint iNumLevel)
{
	if (nullptr != m_pPrototypes)
	{
		return E_FAIL;
	}

	m_pPrototypes = new PROTOTYPES[iNumLevel];

	m_iNumLevel = iNumLevel;

	return S_OK;
}

HRESULT CComponent_Manager::Add_Component_Prototype(_uint iLevelIndex, wstring pPrototypeTag, CComponent* pPrototype)
{
	if (nullptr == pPrototype || m_iNumLevel <= iLevelIndex || nullptr != Find_Component(iLevelIndex, pPrototypeTag))
	{
		return E_FAIL;
	}

	m_pPrototypes[iLevelIndex].emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

CComponent* CComponent_Manager::Clone_Component(_uint iLevelIndex, wstring pPrototypeTag, void* pArg)
{
	if (m_iNumLevel <= iLevelIndex)
	{
		return nullptr;
	}

	CComponent* pPrototype = Find_Component(iLevelIndex, pPrototypeTag);

	if (nullptr == pPrototype)
	{
		return nullptr;
	}

	CComponent* pComponent = pPrototype->Clone(pArg);

	return pComponent;
}

void CComponent_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pPrototypes[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}

	m_pPrototypes[iLevelIndex].clear();
}

CComponent* CComponent_Manager::Find_Component(_uint iLevelIndex, wstring pPrototypeTag)
{
	if (m_pPrototypes[iLevelIndex].end() == m_pPrototypes[iLevelIndex].find(pPrototypeTag))
	{
		return nullptr;
	}

	return m_pPrototypes[iLevelIndex].find(pPrototypeTag)->second;
}

void CComponent_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
		{
			Safe_Release(Pair.second);
		}

		m_pPrototypes[i].clear();
	}

	Safe_Delete_Array(m_pPrototypes);
}
