#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CGameObject::Tick(_double DeltaTime)
{
	return _int();
}

_int CGameObject::Late_Tick(_double DeltaTime)
{
	return _int();
}

HRESULT CGameObject::Render(TYPE eType)
{
	return S_OK;
}

HRESULT CGameObject::Render_Shadow()
{
	return S_OK;
}

HRESULT CGameObject::Add_Components(void* pArg)
{
	return S_OK;
}

HRESULT CGameObject::Set_ConstTable()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, wstring pPrototypeTag, wstring pComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
	{
		return E_FAIL;
	}

	CComponent* pComponent = CComponent_Manager::Get_Instance()->Clone_Component(iLevelIndex, pPrototypeTag, pArg);

	m_Components.emplace(pComponentTag, pComponent);

	Safe_AddRef(pComponent);

	*ppOut = pComponent;

	return S_OK;
}

CComponent* CGameObject::Find_Component(wstring pComponentTag)
{
	if (m_Components.end() == m_Components.find(pComponentTag))
	{
		return nullptr;
	}

	return m_Components.find(pComponentTag)->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
	{
		Safe_Release(Pair.second);
	}

	m_Components.clear();

	Safe_Release(m_pGraphic_Device);
}
