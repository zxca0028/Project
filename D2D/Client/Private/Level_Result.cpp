#include "pch.h"
#include "Level_Result.h"
#include "Level_Main.h"
#include "GameInstance.h"
#include "Result.h"
#include "UI.h"
#include "Sound_Manager.h"

BEGIN(Client)

CLevel_Result::CLevel_Result(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Result::NativeConstruct(LEVEL eLevel)
{
    m_eLevelIndex = eLevel;

    if (FAILED(Ready_Layer_Result(TEXT("Layer_Result"))))
    {
        return E_FAIL;
    }

    if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    pGameInstance->OnNotify(LEVEL_RESULT);
    pGameInstance->Set_Score();

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

_int CLevel_Result::Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    m_fTimeAcc += DeltaTime;

    if (m_fTimeAcc > 0.3f)
    {
        if (pGameInstance->Key_Down(KEY_SPACE))
        {
            CSound_Manager::Get_Instance()->PlaySound(TEXT("Open.mp3"), CSound_Manager::CHANNELID::INPUT);

            if (FAILED(pGameInstance->Set_Level(CLevel_Main::Create(m_pGraphic_Device, LEVEL_MAIN))))
            {
                return E_FAIL;
            }
        }
    }

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CLevel_Result::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CLevel_Result::Render()
{
    _tchar szText[MAX_PATH] = TEXT("MOKOKO");

    SetWindowText(g_hWnd, szText);

    return S_OK;
}

HRESULT CLevel_Result::Ready_Layer_Result(wstring LayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, CResult::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_Result::Ready_Layer_UI(wstring LayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CUI* pUI = nullptr;
    CUI::UIDESC UIDesc;

    UIDesc.vPos   = _float3(450.f, 705.f, 0.f);
    UIDesc.eType  = CUI::TYPE_DYNAMIC;
    UIDesc.eUsage = CUI::USAGE_RESULT;

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_UI(TEXT("Result_Score_Hundreds"), pUI))) { return E_FAIL; }

    UIDesc.vPos = _float3(500.f, 705.f, 0.f);

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_UI(TEXT("Result_Score_Tens"), pUI))) { return E_FAIL; }

    UIDesc.vPos = _float3(550.f, 705.f, 0.f);

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_UI(TEXT("Result_Score_Units"), pUI))) { return E_FAIL; }

    UIDesc.vPos = _float3(450.f, 505.f, 0.f);

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_UI(TEXT("Result_HighScore_Hundreds"), pUI))) { return E_FAIL; }

    UIDesc.vPos = _float3(500.f, 505.f, 0.f);

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_UI(TEXT("Result_HighScore_Tens"), pUI))) { return E_FAIL; }

    UIDesc.vPos = _float3(550.f, 505.f, 0.f);

    pUI = CUI::Create(m_pGraphic_Device, &UIDesc);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, pUI))) { return E_FAIL; }
    if (FAILED(pGameInstance->Add_UI(TEXT("Result_HighScore_Units"), pUI))) { return E_FAIL; }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CLevel_Result* CLevel_Result::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
    CLevel_Result* pInstance = new CLevel_Result(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(eLevel)))
    {
        MSGBOX("Failed to Create Level_Result");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Result::Free()
{
    __super::Free();
}

END