#include "pch.h"
#include "Level_GamePlay.h"
#include "Level_Result.h"
#include "GameInstance.h"
#include "UI.h"
#include "Box.h"
#include "Grass.h"
#include "Block.h"
#include "Mokoko.h"
#include "BackGround.h"
#include "Collider.h"
#include "Transform.h"
#include "UI_Manager.h"

BEGIN(Client)

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GamePlay::NativeConstruct(LEVEL eLevel)
{
    m_eLevelIndex = eLevel;

    if (FAILED(Ready_Layer_BoundingBox()))
    {
        return E_FAIL;
    }
    if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CLevel_GamePlay::Tick(_double DeltaTime)
{
    CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

    if (m_fInterval > 1.f)
    {
        m_fInterval = 2.5f - pUI_Manager->Get_Score() / 15.f;
    }

    RELEASE_INSTANCE(CUI_Manager);

    if (false == m_bIsGameOver)
    {
        m_fTimeAcc += DeltaTime;

        if (m_fTimeAcc >= m_fInterval)
        {
            Add_Object(m_fGap, m_fGround, m_fSpeed);

            m_fTimeAcc = 0.f;
        }
    }
    else
    {
        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

        if (FAILED(pGameInstance->Set_Level(CLevel_Result::Create(m_pGraphic_Device, LEVEL_RESULT))))
        {
            return E_FAIL;
        }

        RELEASE_INSTANCE(CGameInstance);
    }

    return _int();
}

_int CLevel_GamePlay::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CLevel_GamePlay::Render()
{
    _tchar szText[MAX_PATH] = TEXT("MOKOKO");

    SetWindowText(g_hWnd, szText);

    return S_OK;
}

HRESULT CLevel_GamePlay::Add_Object(_float m_fGap, _float m_fGround, _float m_fSpeed)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _float fRandom = pGameInstance->Get_Float(100.f, 450.f);
    
    _float fPos_GND;
    _float fPos_TOP;
    _float fScale_GND;
    _float fScale_TOP;

    m_Desc.fDeletePos = -100.f;
    m_Desc.fSpeed     = m_fSpeed;

    m_Desc.vScale   = _float3(80.f, fScale_GND = fRandom, 0.f);
    m_Desc.vInitPos = _float3(800.f, fPos_GND = (m_fGround + (m_Desc.vScale.y / 2.f)), 0.f);
    m_Desc.iState   = CBlock::POSITION_GND;

    if (FAILED(pGameInstance->Add_GameObject(TEXT("Layer_Block_GND"), CBlock::Create(m_pGraphic_Device, (void*)&m_Desc)))) { return E_FAIL; }

    m_Desc.vScale   = _float3(80.f, fScale_TOP = (1000.f - (fRandom + m_fGround + m_fGap)), 0.f);
    m_Desc.vInitPos = _float3(800.f, fPos_TOP = (1000.f - (m_Desc.vScale.y / 2.f)), 0.f);
    m_Desc.iState   = CBlock::POSITION_TOP;

    if (FAILED(pGameInstance->Add_GameObject(TEXT("Layer_Block_TOP"), CBlock::Create(m_pGraphic_Device, (void*)&m_Desc)))) { return E_FAIL; }

    CMokoko::MOKOKODESC MokokoDesc;
    MokokoDesc.vInitPos = _float3(805.f, ((fPos_TOP - fScale_TOP / 2.f) + (fPos_GND + fScale_GND / 2.f)) / 2.f, 0.f);
    MokokoDesc.fSpeed    = m_fSpeed;

    pGameInstance->Add_GameObject(TEXT("Layer_Mokoko"), CMokoko::Create(m_pGraphic_Device, (void*)&MokokoDesc));

    m_Desc.vScale   = _float3(120.f, 100.f, 0.f);
    m_Desc.vInitPos = _float3(803.f, m_fGround + 20.f, 0.f);

    if (FAILED(pGameInstance->Add_GameObject(TEXT("Layer_Grass"), CGrass::Create(m_pGraphic_Device, (void*)&m_Desc)))) { return E_FAIL; }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(wstring LayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CUI* pUI = nullptr;
    CUI::UIDESC UIDesc;

#pragma region ICON
    UIDesc.vPos   = _float3(35.f, 965.f, 0.f);
    UIDesc.eType  = CUI::TYPE_STATIC;
    UIDesc.eUsage = CUI::USAGE_GAMEPLAY;

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
#pragma endregion

#pragma region  HUNDREDS
    UIDesc.vPos  = _float3(85.f, 965.f, 0.f);
    UIDesc.eType = CUI::TYPE_DYNAMIC;

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_UI(TEXT("GamePlay_Hundreds"), pUI))) { return E_FAIL; }
#pragma endregion

#pragma region TENS
    UIDesc.vPos = _float3(135.f, 965.f, 0.f);

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_UI(TEXT("GamePlay_Tens"), pUI))) { return E_FAIL; }
#pragma endregion

#pragma region UNITS
    UIDesc.vPos = _float3(185.f, 965.f, 0.f);

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_UI(TEXT("GamePlay_Units"), pUI))) { return E_FAIL; }
#pragma endregion

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BoundingBox()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CBox::BOXDESC BoxDesc;

    BoxDesc.vPos   = _float3(350.f, 90.f, 0.f);
    BoxDesc.vScale = _float3(700.f, 180.f, 0.f);

    if (FAILED(pGameInstance->Add_GameObject(TEXT("Layer_Box_GND"), CBox::Create(m_pGraphic_Device, &BoxDesc))))
    {
        return E_FAIL;
    }

    BoxDesc.vPos   = _float3(350.f, 1000.f, 0.f);
    BoxDesc.vScale = _float3(700.f, 1.f, 0.f);

    if (FAILED(pGameInstance->Add_GameObject(TEXT("Layer_Box_TOP"), CBox::Create(m_pGraphic_Device, &BoxDesc))))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
    CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(eLevel)))
    {
        MSGBOX("Failed to Create Level_Main");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_GamePlay::Free()
{
    __super::Free();
}

END