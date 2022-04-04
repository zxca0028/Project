#include "pch.h"
#include "GameInstance.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pRandom(CRandom::Get_Instance())
	, m_pPipeline(CPipeline::Get_Instance())
	, m_pRenderer(CRenderer::Get_Instance())
	, m_pKey_Manager(CKey_Manager::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pUI_Manager(CUI_Manager::Get_Instance())
	, m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pGameObject_Manager(CGameObject_Manager::Get_Instance())
{
	Safe_AddRef(m_pRandom);
	Safe_AddRef(m_pPipeline);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pKey_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pUI_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameObject_Manager);
}

#pragma region GAMEINSTANCE
HRESULT CGameInstance::NativeConstruct(LPDIRECT3DDEVICE9* ppGraphic_Device)
{
	if (nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	if (nullptr == m_pPipeline)
	{
		return E_FAIL;
	}

	if (nullptr == m_pUI_Manager)
	{
		return E_FAIL;
	}

	if (FAILED(m_pGraphic_Device->NativeConstruct(g_hWnd, g_iWinCX, g_iWinCY, ppGraphic_Device)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pPipeline->NativeConstruct()))
	{
		return E_FAIL;
	}

	if (FAILED(m_pUI_Manager->Load_Score()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CGameInstance::Tick(_double DeltaTime)
{
	if (nullptr == m_pKey_Manager)
	{
		return -1;
	}
	if (nullptr == m_pLevel_Manager)
	{
		return -1;
	}
	if (nullptr == m_pGameObject_Manager)
	{
		return -1;
	}

	m_pKey_Manager->Key_Update();

	m_pLevel_Manager->Tick(DeltaTime);

	m_pGameObject_Manager->Tick(DeltaTime);

	return _int();
}

_int CGameInstance::Late_Tick(_double DeltaTime)
{
	if (nullptr == m_pLevel_Manager)
	{
		return -1;
	}
	if (nullptr == m_pGameObject_Manager)
	{
		return -1;
	}

	m_pLevel_Manager->Late_Tick(DeltaTime);

	m_pGameObject_Manager->Late_Tick(DeltaTime);

	return _int();
}

HRESULT CGameInstance::Render_Begin()
{
	if (nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}
	if (nullptr == m_pLevel_Manager)
	{
		return E_FAIL;
	}
	if (nullptr == m_pGameObject_Manager)
	{
		return E_FAIL;
	}

	m_pGraphic_Device->Render_Begin();

	m_pLevel_Manager->Render();

	m_pRenderer->Render();

	return S_OK;
}
HRESULT CGameInstance::Render_End()
{
	if (nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	m_pGraphic_Device->Render_End();

	return S_OK;
}
void CGameInstance::Release_Instances()
{
	if (0 != CGameInstance::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release GameInstance");
	}
	if (0 != CRenderer::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release Renderer");
	}
	if (0 != CLevel_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release Level_Manager");
	}
	if (0 != CGameObject_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release GameObject_Manager");
	}
	if (0 != CTimer_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release Timer_Manager");
	}
	if (0 != CKey_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release Key_Manager");
	}
	if (0 != CPipeline::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release Pipeline");
	}
	if (0 != CRandom::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release Random");
	}
	if (0 != CUI_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release Score_Manager");
	}
	if (0 != CGraphic_Device::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release Graphic_Device");
	}
}
#pragma endregion



#pragma region RENDERER
HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERGROUP eGroup, CGameObject* pGameObject)
{
	if (nullptr == m_pRenderer)
	{
		return E_FAIL;
	}

	return m_pRenderer->Add_RenderObject(eGroup, pGameObject);
}

#ifdef _DEBUG

HRESULT CGameInstance::Add_RenderComponent(CComponent* pComponent)
{
	if (nullptr == m_pRenderer)
	{
		return E_FAIL;
	}

	return m_pRenderer->Add_RenderComponent(pComponent);
}

#endif

#pragma endregion



#pragma region KEY_MANAGER
_bool CGameInstance::Key_Up(DWORD dwKey)
{
	if (nullptr == m_pKey_Manager)
	{
		return false;
	}

	return m_pKey_Manager->Key_Up(dwKey);
}

_bool CGameInstance::Key_Down(DWORD dwKey)
{
	if (nullptr == m_pKey_Manager)
	{
		return false;
	}

	return m_pKey_Manager->Key_Down(dwKey);
}

_bool CGameInstance::Key_Pressing(DWORD dwKey)
{
	if (nullptr == m_pKey_Manager)
	{
		return false;
	}

	return m_pKey_Manager->Key_Pressing(dwKey);
}
#pragma endregion



#pragma region LEVEL_MANAGER
HRESULT CGameInstance::Set_Level(CLevel* pLevel)
{
	if (nullptr == m_pLevel_Manager)
	{
		return E_FAIL;
	}

	return m_pLevel_Manager->Set_Level(pLevel);
}
#pragma endregion



#pragma region TIMER_MANAGER
HRESULT CGameInstance::Add_Timer(wstring pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
	{
		return E_FAIL;
	}

	return m_pTimer_Manager->Add_Timer(pTimerTag);
}
_double CGameInstance::Compute_DeltaTime(wstring pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
	{
		return 0;
	}

	return m_pTimer_Manager->Compute_DeltaTime(pTimerTag);
}
#pragma endregion



#pragma region GAMEOBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject(wstring LayerTag, CGameObject* pGameObject)
{
	if (nullptr == m_pGameObject_Manager)
	{
		return E_FAIL;
	}

	return m_pGameObject_Manager->Add_GameObject(LayerTag, pGameObject);
}
HRESULT CGameInstance::Add_DeleteQueue(wstring LayerTag, CGameObject* pGameObject)
{
	if (nullptr == m_pGameObject_Manager)
	{
		return E_FAIL;
	}

	return m_pGameObject_Manager->Add_DeleteQueue(LayerTag, pGameObject);
}
HRESULT CGameInstance::Delete_GameObject()
{
	if (nullptr == m_pGameObject_Manager)
	{
		return E_FAIL;
	}

	return m_pGameObject_Manager->Delete_GameObject();
}
void CGameInstance::OnNotify(_bool bIsGameOver)
{
	if (nullptr == m_pGameObject_Manager)
	{
		return;
	}

	m_pLevel_Manager->OnNotify(bIsGameOver);
	m_pGameObject_Manager->OnNotify(bIsGameOver);
}
void CGameInstance::OnNotify(LEVEL eLevel)
{
	if (nullptr == m_pGameObject_Manager)
	{
		return;
	}

	m_pGameObject_Manager->OnNotify(eLevel);
}
CComponent* CGameInstance::Get_ComponentPtr(wstring LayerTag, wstring ComponentTag, _uint iIndex)
{
	if (nullptr == m_pGameObject_Manager)
	{
		return nullptr;
	}

	return m_pGameObject_Manager->Get_ComponentPtr(LayerTag, ComponentTag, iIndex);
}
#pragma endregion



#pragma region PIPELINE
HRESULT CGameInstance::Set_Transform(_matrix TransformMatrix, D3DTRANSFORMSTATETYPE eType)
{
	if (nullptr == m_pPipeline)
	{
		return E_FAIL;
	}
	
	return m_pPipeline->Set_Transform(TransformMatrix, eType);
}

_matrix CGameInstance::Get_Transform(D3DTRANSFORMSTATETYPE eType)
{
	if (nullptr == m_pPipeline)
	{
		_matrix IdentityMatrix;

		D3DXMatrixIdentity(&IdentityMatrix);

		return IdentityMatrix;
	}

	return m_pPipeline->Get_Transform(eType);
}
_float CGameInstance::Get_Float(_float fMin, _float fMax)
{
	if (nullptr == m_pRandom)
	{
		return _float();
	}

	return m_pRandom->Get_Float(fMin, fMax);
}
#pragma endregion



#pragma region SCORE_MANAGER
HRESULT CGameInstance::Add_UI(wstring UITag, CUI* pUI)
{
	if (nullptr == m_pUI_Manager)
	{
		return E_FAIL;
	}

	return m_pUI_Manager->Add_UI(UITag, pUI);
}
HRESULT CGameInstance::Plus_Score()
{
	if (nullptr == m_pUI_Manager)
	{
		return E_FAIL;
	}

	return m_pUI_Manager->Plus_Score();
}
HRESULT CGameInstance::Set_Score()
{
	if (nullptr == m_pUI_Manager)
	{
		return E_FAIL;
	}

	return m_pUI_Manager->Set_Score();
}
void CGameInstance::Clear_UI()
{
	return m_pUI_Manager->Clear_UI();
}
#pragma endregion


void CGameInstance::Free()
{
	Safe_Release(m_pRandom);
	Safe_Release(m_pPipeline);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pUI_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameObject_Manager);
}

END