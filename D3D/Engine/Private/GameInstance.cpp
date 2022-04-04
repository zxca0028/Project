#include "GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
    : m_pGraphic_Device   (CGraphic_Device::Get_Instance())
    , m_pInput            (CInput::Get_Instance())
    , m_pPipeLine         (CPipeLine::Get_Instance())
    , m_pTime_Manager     (CTime_Manager::Get_Instance())
    , m_pLevel_Manager    (CLevel_Manager::Get_Instance())
    , m_pLight_Manager    (CLight_Manager::Get_Instance())
    , m_pObject_Manager   (CObject_Manager::Get_Instance())
    , m_pComponent_Manager(CComponent_Manager::Get_Instance())
{
    Safe_AddRef(m_pGraphic_Device);
    Safe_AddRef(m_pInput);
    Safe_AddRef(m_pPipeLine);
    Safe_AddRef(m_pTime_Manager);
    Safe_AddRef(m_pLight_Manager);
    Safe_AddRef(m_pLevel_Manager);
    Safe_AddRef(m_pObject_Manager);
    Safe_AddRef(m_pComponent_Manager);
}
//////////////////////////////////////////////////////////////////////
#pragma region PUBLIC

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevel, HINSTANCE hInst, HWND hWnd)
{
    if (nullptr == m_pObject_Manager)
    {
        return E_FAIL;
    }
    if (nullptr == m_pComponent_Manager)
    {
        return E_FAIL;
    }
    if (nullptr == m_pInput)
    {
        return E_FAIL;
    }

    if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevel)))
    {
        return E_FAIL;
    }
    if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevel)))
    {
        return E_FAIL;
    }
    if (FAILED(m_pInput->NativeConstruct(hInst, hWnd)))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CGameInstance::Tick(_double DeltaTime)
{
    if (nullptr == m_pInput)
    {
        return -1;
    }
    if (nullptr == m_pLevel_Manager || nullptr == m_pObject_Manager)
    {
        return -1;
    }

    m_pInput->Update_Input();

     m_pLevel_Manager->Tick(DeltaTime);
    m_pObject_Manager->Tick(DeltaTime);

    return _int();
}

_int CGameInstance::Late_Tick(_double DeltaTime)
{
    if (nullptr == m_pLevel_Manager || nullptr == m_pObject_Manager)
    {
        return -1;
    }

     m_pLevel_Manager->Late_Tick(DeltaTime);
    m_pObject_Manager->Late_Tick(DeltaTime);

    return _int();
}

void CGameInstance::Clear(_uint iLevelIndex)
{
    if (nullptr != m_pObject_Manager)
    {
        m_pObject_Manager->Clear(iLevelIndex);
    }
    if (nullptr != m_pComponent_Manager)
    {
        m_pComponent_Manager->Clear(iLevelIndex);
    }
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
    if (0 != CObject_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Object_Manager");
    }
    if (0 != CComponent_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Component_Manager");
    }
    if (0 != CLight_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Light_Manager");
    }
    if (0 != CPipeLine::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release PipeLine");
    }
    if (0 != CTime_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Time_Manager");
    }
    if (0 != CInput::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Input");
    }
    if (0 != CTarget_Manager::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Target_Manager");
    }
    if (0 != CGraphic_Device::Get_Instance()->Destroy_Instance())
    {
        MSGBOX("Failed to Release Graphic_Device");
    }
}

#pragma endregion
//////////////////////////////////////////////////////////////////////
#pragma region GRAPHIC_DEVICE

HRESULT CGameInstance::Ready_Graphic_Device(HWND hWnd, CGraphic_Device::WINMODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device)
{
    if (nullptr == m_pGraphic_Device)
    {
        return E_FAIL;
    }
    else
    {
        return m_pGraphic_Device->Ready_Graphic_Device(hWnd, eMode, iWinCX, iWinCY, ppGraphic_Device);
    }
}

void CGameInstance::Render_Begin()
{
    if (nullptr != m_pGraphic_Device)
    {
        m_pGraphic_Device->Render_Begin();
    }
}

void CGameInstance::Render_End(HWND hWND)
{
    if (nullptr != m_pGraphic_Device)
    {
        m_pGraphic_Device->Render_End(hWND);
    }
}

#pragma endregion
//////////////////////////////////////////////////////////////////////
#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Set_Level(CLevel* pLevel)
{
    if (nullptr == m_pLevel_Manager)
    {
        return E_FAIL;
    }
    else
    {
        return m_pLevel_Manager->Set_Level(pLevel);
    }
}

HRESULT CGameInstance::Render_Level()
{
    if (nullptr == m_pLevel_Manager)
    {
        return E_FAIL;
    }
    else
    {
        return m_pLevel_Manager->Render();
    }
}

#pragma endregion
//////////////////////////////////////////////////////////////////////
#pragma region OBJECT_MANAGER

HRESULT CGameInstance::Add_GameObject_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
    if (nullptr == m_pObject_Manager)
    {
        return E_FAIL;
    }
    else
    {
        return m_pObject_Manager->Add_GameObject_Prototype(pPrototypeTag, pPrototype);
    }
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg)
{
    if (nullptr == m_pObject_Manager)
    {
        return E_FAIL;
    }
    else
    {
        return m_pObject_Manager->Add_GameObject(iLevelIndex, pPrototypeTag, pLayerTag, pArg);
    }
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pGameObject)
{
    if (nullptr == m_pObject_Manager)
    {
        return E_FAIL;
    }
    else
    {
        return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pGameObject);
    }
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex)
{
    if (nullptr == m_pObject_Manager)
    {
        return nullptr;
    }
    else
    {
        return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
    }
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex)
{
    if (nullptr == m_pObject_Manager)
    {
        return nullptr;
    }

    return m_pObject_Manager->Get_GameObject(iLevelIndex, pLayerTag, iIndex);
}

HRESULT CGameInstance::Delete_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex)
{
    if (nullptr == m_pObject_Manager)
    {
        return E_FAIL;
    }

    return m_pObject_Manager->Delete_GameObject(iLevelIndex, pLayerTag, iIndex);
}

HRESULT CGameInstance::Delete_GameObject()
{
    if (nullptr == m_pObject_Manager)
    {
        return E_FAIL;
    }

    return m_pObject_Manager->Delete_GameObject();
}

HRESULT CGameInstance::Add_DeleteContainer(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pDeleteGameObject)
{
    if (nullptr == m_pObject_Manager)
    {
        return E_FAIL;
    }

    return m_pObject_Manager->Add_DeleteContainer(iLevelIndex, pLayerTag, pDeleteGameObject);
}

_uint CGameInstance::Get_LayerSize(_uint iLevelIndex, const _tchar* pLayerTag)
{
    if (nullptr == m_pObject_Manager)
    {
        return E_FAIL;
    }

    return m_pObject_Manager->Get_LayerSize(iLevelIndex, pLayerTag);
}

#pragma endregion
//////////////////////////////////////////////////////////////////////
#pragma region COMPONENT_MANAGER

HRESULT CGameInstance::Add_Component_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype)
{
    if (nullptr == m_pComponent_Manager)
    {
        return E_FAIL;
    }
    else
    {
        return m_pComponent_Manager->Add_Component_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
    }
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg)
{
    if (nullptr == m_pComponent_Manager)
    {
        return nullptr;
    }
    else
    {
        return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
    }
}

#pragma endregion
//////////////////////////////////////////////////////////////////////
#pragma region TIME_MANAGER

HRESULT CGameInstance::Add_Timer(string pTimerTag)//const _tchar* pTimerTag)
{
    if (nullptr == m_pTime_Manager)
    {
        return E_FAIL;
    }
    else
    {
        return m_pTime_Manager->Add_Timer(pTimerTag);
    }
}

_float CGameInstance::Compute_DeltaTime(string pTimerTag)//const _tchar* pTimerTag)
{
    if (nullptr == m_pTime_Manager)
    {
        return 0.f;
    }
    else
    {
        return m_pTime_Manager->Compute_DeltaTime(pTimerTag);
    }
}

#pragma endregion
//////////////////////////////////////////////////////////////////////
#pragma region PIPELINE

_matrix CGameInstance::Get_Transform(D3DTRANSFORMSTATETYPE eStateType)
{
    if (nullptr == m_pPipeLine)
    {
        _matrix IdenMtx;
        D3DXMatrixIdentity(&IdenMtx);

        return IdenMtx;
    }
    else
    {
        return m_pPipeLine->Get_Transform(eStateType);
    }    
}

_float3 CGameInstance::Get_CamPosition()
{
    if (nullptr == m_pPipeLine)
    {
        return _float3();
    }

    return m_pPipeLine->Get_CamPosition();
}

#pragma endregion
//////////////////////////////////////////////////////////////////////
#pragma region INPUT

_byte CGameInstance::Get_KeyState(_byte bKey)
{
    if (nullptr == m_pInput)
    {
        return 0;
    }
    else
    {
        return m_pInput->Get_KeyState(bKey);
    }
}

_byte CGameInstance::Get_MouseClickState(CInput::MOUSE_CLICK eClick)
{
    if (nullptr == m_pInput)
    {
        return 0;
    }
    else
    {
        return m_pInput->Get_MouseClickState(eClick);
    }
}

_long CGameInstance::Get_MouseMoveState(CInput::MOUSE_MOVE eMove)
{
    if (nullptr == m_pInput)
    {
        return 0;
    }
    else
    {
        return m_pInput->Get_MouseMoveState(eMove);
    }
}
#pragma endregion
//////////////////////////////////////////////////////////////////////
#pragma region LIGHT_MANAGER

D3DLIGHT9 CGameInstance::Get_LightDesc(_uint iIndex)
{
    if (nullptr == m_pLight_Manager)
    {
        return D3DLIGHT9();
    }

    return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightDesc)
{
    if (nullptr == m_pLight_Manager)
    {
        return E_FAIL;
    }

    return m_pLight_Manager->Add_Light(pGraphic_Device, LightDesc);
}

#pragma endregion
//////////////////////////////////////////////////////////////////////

void CGameInstance::Free()
{
    Safe_Release(m_pInput);
    Safe_Release(m_pPipeLine);
    Safe_Release(m_pTime_Manager);
    Safe_Release(m_pLevel_Manager);
    Safe_Release(m_pLight_Manager);
    Safe_Release(m_pObject_Manager);
    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pComponent_Manager);
}
