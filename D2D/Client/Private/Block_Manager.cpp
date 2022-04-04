#include "pch.h"
#include "Block_Manager.h"
#include "GameInstance.h"
#include "Block.h"
#include "Grass.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(CBlock_Manager);

CBlock_Manager::CBlock_Manager()
{
}

HRESULT CBlock_Manager::NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;

	Safe_AddRef(m_pGraphic_Device);

	return S_OK;
}

HRESULT CBlock_Manager::Add_Block()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float fRandom = pGameInstance->Get_Float(150.f, 400.f);

	m_Desc.vScale     = _float3(100.f, fRandom, 0.f);
	m_Desc.vInitPos   = _float3(800.f, 180.f + (m_Desc.vScale.y / 2.f), 0.f);
	m_Desc.fDeletePos = 100.f;

	m_Desc.iState = CBlock::POSITION_GND;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Layer_Block"), CBlock::Create(m_pGraphic_Device, (void*)&m_Desc)))) { return E_FAIL; }


	//m_Desc.iState = CBlock::POSITION_TOP;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Layer_Block"), CBlock::Create(m_pGraphic_Device, (void*)&m_Desc)))) { return E_FAIL; }

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Layer_Block"), CGrass::Create(m_pGraphic_Device, (void*)&m_Desc)))) { return E_FAIL; }

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CBlock_Manager::Free()
{
	Safe_Release(m_pGraphic_Device);
}

END