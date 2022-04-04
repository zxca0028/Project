#include "pch.h"
#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Start.h"
#include "MainFrame.h"

CMainApp::CMainApp()
{
}

HRESULT CMainApp::NativeConstruct()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    CMainFrame*    pMainFrame    = GET_INSTANCE(CMainFrame);

    if (FAILED(pGameInstance->NativeConstruct(g_hWnd, g_iWinCX, g_iWinCY, &m_pGraphic_Device)))
    {
        return E_FAIL;
    }

    if (FAILED(pGameInstance->NativeConstruct((_uint)LEVEL::LEVEL_END, g_hInst, g_hWnd, m_pGraphic_Device)))
    {
        return E_FAIL;
    }
        
    if (FAILED(Add_Component_Prototype()))
    {
        return E_FAIL;
    }

    if (FAILED(pGameInstance->Set_Level(CLevel_Start::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    pMainFrame->NativeConstruct(m_pGraphic_Device);

    RELEASE_INSTANCE(CMainFrame);

    RELEASE_INSTANCE(CGameInstance);
    
    return S_OK;
}

_int CMainApp::Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    CMainFrame*    pMainFrame    = GET_INSTANCE(CMainFrame);

    pMainFrame->Begin_ImGui();

    pMainFrame->Tick();

    pGameInstance->Tick(DeltaTime);

    RELEASE_INSTANCE(CMainFrame);
    RELEASE_INSTANCE(CGameInstance);

    return 0;
}

_int CMainApp::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    pGameInstance->Late_Tick(DeltaTime);

    RELEASE_INSTANCE(CGameInstance);

    return 0;
}

HRESULT CMainApp::Render_MainApp()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    CMainFrame*    pMainFrame    = GET_INSTANCE(CMainFrame);

    pGameInstance->Render_Begin();

    m_pRenderer->Render();

    pGameInstance->Render_Level();

    pMainFrame->Render();

    pGameInstance->Render_End();

    RELEASE_INSTANCE(CMainFrame);
    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CMainApp::Add_Component_Prototype()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    m_pRenderer = CRenderer::Create(m_pGraphic_Device);

    if (nullptr == m_pRenderer)
    {
        return E_FAIL;
    }

    if (FAILED(pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_STATIC, L"Prototype_Renderer", m_pRenderer)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CMainApp* CMainApp::Create()
{
    CMainApp* pInstance = new CMainApp;

    if (FAILED(pInstance->NativeConstruct()))
    {
        MSGBOX("Failed to Create MainApp");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMainApp::Free()
{
    CMainFrame* pMainFrame = GET_INSTANCE(CMainFrame);

    pMainFrame->Release_MainFrame();

    RELEASE_INSTANCE(CMainFrame);

    CMainFrame::Get_Instance()->Destroy_Instance();

    Safe_Release(m_pRenderer);
    Safe_Release(m_pGraphic_Device);

    CGameInstance::Release_Engine();
}
