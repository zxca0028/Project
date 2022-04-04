#include "pch.h"
#include "MainFrame.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CMainFrame)

//CMainFrame::CMainFrame(LPDIRECT3DDEVICE9 pGraphic_Device)
//	: m_pGraphic_Device(pGraphic_Device)
//{
//	Safe_AddRef(m_pGraphic_Device);
//}

CMainFrame::CMainFrame()
{

}

HRESULT CMainFrame::NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;

	Safe_AddRef(m_pGraphic_Device);

	m_pGameInstance = GET_INSTANCE(CGameInstance);

	m_pViewportTexture = m_pGameInstance->Get_TargetDesc(L"Target_Viewport").pTexture;

	m_TargetDesc.emplace("Target_Viewport",    m_pGameInstance->Get_TargetDesc(L"Target_Viewport"));
	m_TargetDesc.emplace("Target_Specular",    m_pGameInstance->Get_TargetDesc(L"Target_Specular"));
	m_TargetDesc.emplace("Target_Depth",       m_pGameInstance->Get_TargetDesc(L"Target_Depth"));
	m_TargetDesc.emplace("Target_Diffuse",     m_pGameInstance->Get_TargetDesc(L"Target_Diffuse"));
	m_TargetDesc.emplace("Target_Normal",      m_pGameInstance->Get_TargetDesc(L"Target_Normal"));
	m_TargetDesc.emplace("Target_Shade",       m_pGameInstance->Get_TargetDesc(L"Target_Shade"));
	m_TargetDesc.emplace("Target_Outline",     m_pGameInstance->Get_TargetDesc(L"Target_Outline"));
	m_TargetDesc.emplace("Target_ShadowDepth", m_pGameInstance->Get_TargetDesc(L"Target_ShadowDepth"));

	for (auto& Pair : m_TargetDesc)
	{
		Safe_AddRef(Pair.second.pTexture);
	}

	m_pPicking = CPicking::Create(m_pGraphic_Device);

	if (FAILED(m_pGameInstance->Add_Component_Prototype((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Picking", m_pPicking)))
	{
		return E_FAIL;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	return S_OK;
}

_int CMainFrame::Tick()
{
	MainFrame();

	Viewport();

	if (true == m_bIsViewportFocused)
	{
		m_pPicking->Compute_MousePos(Fixed_MousePos, m_vTextureSize);
	}

	RenderTarget();

	Navigator();

	return _int();
}

HRESULT CMainFrame::Render()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	return S_OK;
}

void CMainFrame::Release_MainFrame()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

HRESULT CMainFrame::Begin_ImGui()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return S_OK;
}

HRESULT CMainFrame::MainFrame()
{
	ImGuiDockNodeFlags DockSpace_Flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags   Window_Flags    = /*ImGuiWindowFlags_MenuBar |*/ ImGuiWindowFlags_NoDocking;

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
		
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Menu"))
	{
		if (ImGui::MenuItem("Viewport"))     { m_bViewport    = true; }
		if (ImGui::MenuItem("Diffuse"))      { m_bDiffuse     = true; }
		if (ImGui::MenuItem("Normal"))       { m_bNormal      = true; }
		if (ImGui::MenuItem("Specular"))     { m_bSpecular    = true; }
		if (ImGui::MenuItem("Shade"))        { m_bShade       = true; }
		if (ImGui::MenuItem("Depth"))        { m_bDepth       = true; }
		if (ImGui::MenuItem("Outline"))      { m_bOutline     = true; }
		if (ImGui::MenuItem("ShadowDepth"))  { m_bShadowDepth = true; }

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	ImGui::End();

	return S_OK;
}

HRESULT CMainFrame::Viewport()
{
	static const char* MenuName = "Main";

	if (true == m_bViewport)
	{
		if (false == ImGui::Begin("Viewport", &m_bViewport, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar))
		{
			ImGui::End();
		}
		else
		{
			m_bIsViewportFocused = ImGui::IsWindowFocused();
			
			ImGui::BeginMenuBar();
			if (ImGui::BeginMenu(MenuName))
			{
				if (ImGui::MenuItem("Main"))         { m_pViewportTexture = m_TargetDesc.find("Target_Viewport")->second.pTexture;     MenuName = "Main";        }
				if (ImGui::MenuItem("Diffuse"))      { m_pViewportTexture = m_TargetDesc.find("Target_Diffuse")->second.pTexture;      MenuName = "Diffuse";     }
				if (ImGui::MenuItem("Normal"))       { m_pViewportTexture = m_TargetDesc.find("Target_Normal")->second.pTexture;       MenuName = "Normal";      }
				if (ImGui::MenuItem("Specular"))     { m_pViewportTexture = m_TargetDesc.find("Target_Specular")->second.pTexture;     MenuName = "Specular";    }
				if (ImGui::MenuItem("Shade"))        { m_pViewportTexture = m_TargetDesc.find("Target_Shade")->second.pTexture;        MenuName = "Shade";       }
				if (ImGui::MenuItem("Depth"))        { m_pViewportTexture = m_TargetDesc.find("Target_Depth")->second.pTexture;        MenuName = "Depth";       }
				if (ImGui::MenuItem("Outline"))      { m_pViewportTexture = m_TargetDesc.find("Target_Outline")->second.pTexture;      MenuName = "Outline";     }
				if (ImGui::MenuItem("ShadowDepth"))  { m_pViewportTexture = m_TargetDesc.find("Target_ShadowDepth")->second.pTexture;  MenuName = "ShadowDepth"; }

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();

			m_vViewportSize.x = ImGui::GetWindowSize().x - 15.f;
			m_vViewportSize.y = ImGui::GetWindowSize().y - 53.f;

			ImGui::Image((void*)m_pViewportTexture, ImVec2(m_vViewportSize.x, m_vViewportSize.y));

			Screen_MousePos = _float2(ImGui::GetMousePos().x,    ImGui::GetMousePos().y);
			Viewport_LT_Pos = _float2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y);

			Viewport_MousePos = Screen_MousePos - Viewport_LT_Pos;

			m_vTextureSize.x = m_TargetDesc.find("Target_Viewport")->second.iWitdh;
			m_vTextureSize.y = m_TargetDesc.find("Target_Viewport")->second.iHeight;

			Fixed_MousePos.x = (Viewport_MousePos.x / m_vViewportSize.x) * m_vTextureSize.x;
			Fixed_MousePos.y = (Viewport_MousePos.y / m_vViewportSize.y) * m_vTextureSize.y;

			ImGui::End();
		}
	}

	return S_OK;
}

HRESULT CMainFrame::RenderTarget()
{
	CGameInstance* m_pGameInstance = GET_INSTANCE(CGameInstance);

	LPDIRECT3DTEXTURE9 pDiffuse = m_pGameInstance->Get_TargetDesc(L"Target_Diffuse").pTexture;

	if (true == m_bDiffuse)
	{
		if (false == ImGui::Begin("DiffuseTarget", &m_bDiffuse, ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Image((void*)pDiffuse, ImGui::GetWindowSize());
			ImGui::End();
		}
	}

	LPDIRECT3DTEXTURE9 pNormal = m_pGameInstance->Get_TargetDesc(L"Target_Normal").pTexture;

	if (true == m_bNormal)
	{
		if (false == ImGui::Begin("NormalTarget", &m_bNormal, ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Image((void*)pNormal, ImGui::GetWindowSize());
			ImGui::End();
		}
	}

	LPDIRECT3DTEXTURE9 pSpecular = m_pGameInstance->Get_TargetDesc(L"Target_Specular").pTexture;

	if (true == m_bSpecular)
	{
		if (false == ImGui::Begin("SpecularTarget", &m_bSpecular, ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Image((void*)pSpecular, ImGui::GetWindowSize());
			ImGui::End();
		}
	}

	LPDIRECT3DTEXTURE9 pShade = m_pGameInstance->Get_TargetDesc(L"Target_Shade").pTexture;

	if (true == m_bShade)
	{
		if (false == ImGui::Begin("ShadeTarget", &m_bShade, ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Image((void*)pShade, ImGui::GetWindowSize());
			ImGui::End();
		}
	}

	LPDIRECT3DTEXTURE9 pDepth = m_pGameInstance->Get_TargetDesc(L"Target_Depth").pTexture;

	if (true == m_bDepth)
	{
		if (false == ImGui::Begin("DepthTarget", &m_bDepth, ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Image((void*)pDepth, ImGui::GetWindowSize());
			ImGui::End();
		}
	}

	LPDIRECT3DTEXTURE9 pOutline = m_pGameInstance->Get_TargetDesc(L"Target_Outline").pTexture;

	if (true == m_bOutline)
	{
		if (false == ImGui::Begin("OutlineTarget", &m_bOutline, ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Image((void*)pOutline, ImGui::GetWindowSize());
			ImGui::End();
		}
	}

	LPDIRECT3DTEXTURE9 pShadowDepth = m_pGameInstance->Get_TargetDesc(L"Target_ShadowDepth").pTexture;

	if (true == m_bShadowDepth)
	{
		if (false == ImGui::Begin("ShadowDepthTarget", &m_bShadowDepth, ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Image((void*)pShadowDepth, ImGui::GetWindowSize());
			ImGui::End();
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMainFrame::Navigator()
{
	ImGui::Begin("Test1");
	ImGui::End();
	
	ImGui::Begin("Navigator");

	ImGui::Bullet(); ImGui::SameLine();
	ImGui::Text("MousePos : X = %.f, Y = %.f", Fixed_MousePos.x, Fixed_MousePos.y);

	ImGui::End();

	return S_OK;
}

void CMainFrame::Free()
{
	for (auto& Pair : m_TargetDesc)
	{
		Safe_Release(Pair.second.pTexture);
	}

	m_TargetDesc.clear();

	Safe_Release(m_pPicking);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
