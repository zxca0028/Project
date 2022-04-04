#include "pch.h"
#include "Level_Logo.h"
#include "GameInstance.h"
#include "BG_Logo.h"
#include "Level_Loading.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{}

HRESULT CLevel_Logo::NativeConstruct()
{
	__super::NativeConstruct();

	if (FAILED(Ready_Layer_BG_Logo(TEXT("Layer_BG_Logo"))))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CLevel_Logo::Tick(_double DeltaTime)
{
	__super::Tick(DeltaTime);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		if (SUCCEEDED(pGameInstance->Set_Level(CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
		{
			pGameInstance->Clear(LEVEL_LOGO);
			goto succeeded;
		}
	}

	RELEASE_INSTANCE(CGameInstance);
	return -1;

succeeded:

	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

HRESULT CLevel_Logo::Render()
{
	__super::Render();

	SetWindowText(g_hWnd, TEXT("LEVEL_LOGO"));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BG_Logo(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_BG_Logo"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating Level_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
