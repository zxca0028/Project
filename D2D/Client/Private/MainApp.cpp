#include "pch.h"
#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Main.h"
#include "BackGround.h"
#include "Sound_Manager.h"

BEGIN(Client)

CMainApp::CMainApp()
{
}

HRESULT CMainApp::NativeConstruct()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->NativeConstruct(&m_pGraphic_Device)))
    {
        RELEASE_INSTANCE(CGameInstance);

        return E_FAIL;
    }

    if (FAILED(pGameInstance->Set_Level(CLevel_Main::Create(m_pGraphic_Device, LEVEL_MAIN))))
    {
        RELEASE_INSTANCE(CGameInstance);

        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    CSound_Manager::Get_Instance()->Initialize();
    CSound_Manager::Get_Instance()->PlayBGM(TEXT("BGM.mp3"));

#ifdef _DEBUG

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(m_pGraphic_Device);

#endif

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CMainApp::Tick(_double DeltaTime)
{
#ifdef _DEBUG

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiDockNodeFlags DockSpace_Flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiWindowFlags   Window_Flags = /*ImGuiWindowFlags_MenuBar |*/ ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* Viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(Viewport->WorkPos);
    ImGui::SetNextWindowSize(Viewport->WorkSize);
    ImGui::SetNextWindowViewport(Viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    Window_Flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    Window_Flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    Window_Flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("MainFrame", nullptr, Window_Flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID DockSpace_ID = ImGui::GetID("Main DockSpace");
        ImGui::DockSpace(DockSpace_ID, ImVec2(0.0f, 0.0f), DockSpace_Flags);
    }

    ImGui::End();

#endif

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    pGameInstance->Tick(DeltaTime);

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CMainApp::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    pGameInstance->Late_Tick(DeltaTime);

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

HRESULT CMainApp::Render()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    
    pGameInstance->Render_Begin();

#ifdef _DEBUG

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

#endif

    pGameInstance->Render_End();

    pGameInstance->Delete_GameObject();

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CMainApp::Ready_Layer_BackGround(wstring LayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CBackGround::BGDESC BGDesc;

    BGDesc.fInitPosX = 800.f;
    BGDesc.fSpeed    = 250.f;

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, CBackGround::Create(m_pGraphic_Device, (void*)&BGDesc))))
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
#ifdef _DEBUG

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

#endif

    Safe_Release(m_pGraphic_Device);

    CGameInstance::Release_Instances();

    CSound_Manager::Get_Instance()->Destroy_Instance();
}

END