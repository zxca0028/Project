#include "GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
    : m_pPipeLine(CPipeLine::Get_Instance())
    , m_pKey_Manager(CKey_Manager::Get_Instance())
    , m_pTimer_Manager(CTimer_Manager::Get_Instance())
    , m_pLevel_Manager(CLevel_Manager::Get_Instance())
    , m_pLight_Manager(CLight_Manager::Get_Instance())
    , m_pGraphic_Device(CGraphic_Device::Get_Instance())
    , m_pTarget_Manager(CTarget_Manager::Get_Instance())
    , m_pComponent_Manager(CComponent_Manager::Get_Instance())
    , m_pGameObject_Manager(CGameObject_Manager::Get_Instance())
{
    Safe_AddRef(m_pPipeLine);
    Safe_AddRef(m_pKey_Manager);
    Safe_AddRef(m_pTimer_Manager);
    Safe_AddRef(m_pLevel_Manager);
    Safe_AddRef(m_pLight_Manager);
    Safe_AddRef(m_pTarget_Manager);
    Safe_AddRef(m_pGraphic_Device);
    Safe_AddRef(m_pComponent_Manager);
    Safe_AddRef(m_pGameObject_Manager);
}

#pragma region GAMEINSATNCE
HRESULT CGameInstance::NativeConstruct(_uint iNumLevel, HINSTANCE hInst, HWND hWnd, LPDIRECT3DDEVICE9 pGraphic_Device)
{
    if (nullptr == m_pComponent_Manager)
    {
        return E_FAIL;
    }

    if (FAILED(m_pComponent_Manager->NativeConstruct(iNumLevel)))
    {
        return E_FAIL;
    }

    if (nullptr == m_pGameObject_Manager)
    {
        return E_FAIL;
    }

    if (FAILED(m_pGameObject_Manager->NativeConstruct(iNumLevel)))
    {
        return E_FAIL;
    }

    if (nullptr == m_pKey_Manager)
    {
        return E_FAIL;
    }

    if (FAILED(m_pKey_Manager->NativeConstruct(hInst, hWnd)))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CGameInstance::Tick(_double DeltaTime)
{
    if (nullptr == m_pKey_Manager)
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

    m_pKey_Manager->Key_Update();

    m_pLevel_Manager->Tick(DeltaTime);

    m_pGameObject_Manager->Tick(DeltaTime);

    return _int();
}

_int CGameInstance::Late_Tick(_double DeltaTime)
{
    if (nullptr == m_pLevel_Manager)
    {
        return E_FAIL;
    }

    if (nullptr == m_pGameObject_Manager)
    {
        return E_FAIL;
    }

    m_pLevel_Manager->Late_Tick(DeltaTime);

    m_pGameObject_Manager->Late_Tick(DeltaTime);

    return _int();
}

void CGameInstance::Clear(_uint iLevelIndex)
{
    if (nullptr == m_pComponent_Manager)
    {
        return;
    }

    if (nullptr == m_pGameObject_Manager)
    {
        return;
    }

    m_pComponent_Manager->Clear(iLevelIndex);
    m_pGameObject_Manager->Clear(iLevelIndex);
}

void CGameInstance::Release_Engine()
{

    if (0 != CGameInstance::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release GameInstance");
    }
    if (0 != CLevel_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Level_Manager");
    }
    if (0 != CGameObject_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release GameObject_Manager");
    }
    if (0 != CComponent_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Component_Manager");
    }
    if (0 != CTimer_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Timer_Manager");
    }
    if (0 != CTarget_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Target_Manager");
    }
    if (0 != CLight_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Light_Manager");
    }
    if (0 != CKey_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Key_Manager");
    }
    if (0 != CPipeLine::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release PipeLine");
    }
    if (0 != CGraphic_Device::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Graphic_Device");
    }
}
#pragma endregion

#pragma region GRAPHIC_DEVICE
HRESULT CGameInstance::NativeConstruct(HWND hWnd, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device)
{
    if (nullptr == m_pGraphic_Device)
    {
        return E_FAIL;
    }

    return m_pGraphic_Device->NativeConstruct(hWnd, iWinCX, iWinCY, ppGraphic_Device);
}

void CGameInstance::Render_Begin()
{
    if (nullptr == m_pGraphic_Device)
    {
        return;
    }

    m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWnd)
{
    if (nullptr == m_pGraphic_Device)
    {
        return;
    }

    m_pGraphic_Device->Render_End(hWnd);
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
        return 0.f;
    }

    return m_pTimer_Manager->Compute_DeltaTime(pTimerTag);
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
HRESULT CGameInstance::Render_Level()
{
    if (nullptr == m_pLevel_Manager)
    {
        return E_FAIL;
    }

    return m_pLevel_Manager->Render();
}
#pragma endregion

#pragma region COMPONENT_MANAGER
HRESULT CGameInstance::Add_Component_Prototype(_uint iLevelIndex, wstring pPrototypeTag, CComponent* pPrototype)
{
    if (nullptr == m_pComponent_Manager)
    {
        return E_FAIL;
    }

    return m_pComponent_Manager->Add_Component_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}
#pragma endregion

#pragma region GAMEOBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject_Prototype(wstring pPrototypeTag, CGameObject* pPrototype)
{
    if (nullptr == m_pGameObject_Manager)
    {
        return E_FAIL;
    }

    return m_pGameObject_Manager->Add_GameObject_Prototype(pPrototypeTag, pPrototype);
}
HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, wstring pPrototypeTag, wstring pLayerTag, void* pArg)
{
    if (nullptr == m_pGameObject_Manager)
    {
        return E_FAIL;
    }

    return m_pGameObject_Manager->Add_GameObject(iLevelIndex, pPrototypeTag, pLayerTag, pArg);
}
CComponent* CGameInstance::Get_ComponentPtr(_uint iLevelIndex, wstring pLayerTag, wstring pComponentTag, _uint iIndex)
{
    if (nullptr == m_pGameObject_Manager)
    {
        return nullptr;
    }

    return m_pGameObject_Manager->Get_ComponentPtr(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}
#pragma endregion

#pragma region PIPELINE
_matrix CGameInstance::Get_Transform(D3DTRANSFORMSTATETYPE eStateType)
{
    if (nullptr == m_pPipeLine)
    {
        _matrix IdentityMatrix;
        D3DXMatrixIdentity(&IdentityMatrix);

        return IdentityMatrix;
    }

    return m_pPipeLine->Get_Transform(eStateType);
}
_float3 CGameInstance::Get_CamPos()
{
    if (nullptr == m_pPipeLine)
    {
        return _float3();
    }

    return m_pPipeLine->Get_CamPos();
}
#pragma endregion

#pragma region KEY_MANAGER
_bool CGameInstance::Key_Up(DWORD dwKey)
{
    if (nullptr == m_pKey_Manager)
    {
        return _bool();
    }

    return m_pKey_Manager->Key_Up(dwKey);
}
_bool CGameInstance::Key_Down(DWORD dwKey)
{
    if (nullptr == m_pKey_Manager)
    {
        return _bool();
    }

    return m_pKey_Manager->Key_Down(dwKey);
}
_bool CGameInstance::Key_Pressing(DWORD dwKey)
{
    if (nullptr == m_pKey_Manager)
    {
        return _bool();
    }

    return m_pKey_Manager->Key_Pressing(dwKey);
}
_long CGameInstance::Mouse_Move(CKey_Manager::MOUSE eMouseState)
{
    if (nullptr == m_pKey_Manager)
    {
        return _long();
    }

    return m_pKey_Manager->Mouse_Move(eMouseState);
}
#pragma endregion

#pragma region LIGHT_MANAGER
HRESULT CGameInstance::Add_Right(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9_DERIVED& LightDesc)
{
    if (nullptr == m_pLight_Manager)
    {
        return E_FAIL;
    }
    
	return m_pLight_Manager->Add_Light(pGraphic_Device, LightDesc);
}
_matrix CGameInstance::Get_LightTransform(D3DTRANSFORMSTATETYPE eStateType)
{
    if (nullptr == m_pPipeLine)
    {
        _matrix IdentityMatrix;
        D3DXMatrixIdentity(&IdentityMatrix);

        return IdentityMatrix;
    }

    return m_pLight_Manager->Get_LightTransform(eStateType);
}
#pragma endregion

#pragma region TARGET_MANAGER
TARGETDESC CGameInstance::Get_TargetDesc(wstring pTargetTag)
{
    if (nullptr == m_pTarget_Manager)
    {
        return TARGETDESC();
    }

    return m_pTarget_Manager->Get_TargetDesc(pTargetTag);
}
#pragma endregion

void CGameInstance::Free()
{
    Safe_Release(m_pPipeLine);
    Safe_Release(m_pKey_Manager);
    Safe_Release(m_pLevel_Manager);
    Safe_Release(m_pLight_Manager);
    Safe_Release(m_pTimer_Manager);
    Safe_Release(m_pTarget_Manager);
    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pComponent_Manager);
    Safe_Release(m_pGameObject_Manager);
}
