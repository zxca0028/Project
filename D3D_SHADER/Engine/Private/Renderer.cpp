#include "Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Shader.h"
#include "Light_Manager.h"
#include "GameInstance.h"

#include "VIBuffer_Viewport.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent(pGraphic_Device)
    , m_pTarget_Manager(CTarget_Manager::Get_Instance())
{
    Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject)
{
    if (nullptr == pGameObject || RENDERGROUP::RENDER_END <= eGroup)
    {
        return E_FAIL;
    }

    m_RenderObjects[(_uint)eGroup].emplace_back(pGameObject);

    Safe_AddRef(pGameObject);

    return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(CComponent* pComponent)
{
    if (nullptr == pComponent)
    {
        return E_FAIL;
    }

    m_RenderComponent.emplace_back(pComponent);

    Safe_AddRef(pComponent);

    return S_OK;
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
    D3DVIEWPORT9 Viewport;
    m_pGraphic_Device->GetViewport(&Viewport);

    if (FAILED(Set_RenderTarget(Viewport)))
    {
        return E_FAIL;
    }

    m_pShader = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Deferred.hlsl");

    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    m_pVIBuffer = CVIBuffer_Viewport::Create(m_pGraphic_Device, -0.5f, -0.5f, Viewport.Width, Viewport.Height);
    
    if (nullptr == m_pVIBuffer)
    {
        return E_FAIL;
    }

	//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Diffuse"),    0.f,   0.f, 200.f, 200.f))) { return E_FAIL; }
	//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Normal"),     0.f, 200.f, 200.f, 200.f))) { return E_FAIL; }
	//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Specular"),   0.f, 400.f, 200.f, 200.f))) { return E_FAIL; }
    //
	//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Shade"),    200.f,   0.f, 200.f, 200.f))) { return E_FAIL; }
	//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Depth"),    200.f, 200.f, 200.f, 200.f))) { return E_FAIL; }
    //if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Viewport"), 200.f, 400.f, 200.f, 200.f))) { return E_FAIL; }
    //
    //if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_SkyBox"),   400.f,   0.f, 200.f, 200.f))) { return E_FAIL; }
    
    return S_OK;
}

HRESULT CRenderer::Render()
{
    Render_Priority();

    Render_Shadow();

    Render_Outline();

    Render_NonAlpha();

    Render_LightAcc();

    Render_Blend();

    Render_Alpha();

    Render_UI();

    //if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_Deferred")))) { return E_FAIL; }
    //if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_LightAcc")))) { return E_FAIL; }
    ////if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_LightDepth")))) { return E_FAIL; }
    //
    //if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_All")))) { return E_FAIL; }
    //if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_SkyBox")))) { return E_FAIL; }
    //if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_Outline")))) { return E_FAIL; }

    return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
    if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_SkyBox"))) { return E_FAIL; }

    for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_PRIORITY])
    {
        if (FAILED(pGameObject->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[(_uint)RENDERGROUP::RENDER_PRIORITY].clear();
    
    if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_SkyBox"))) { return E_FAIL; }

    return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{
                                 // MRT = Multi Render Target
    if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Deferred"))) { return E_FAIL; }
                                 // Target_Diffuse    ─┐
                                 // Target_Normal      ├ Target Class (LPDIRECT3DSURFACE9)
                                 // Target_Depth      ─┘
                                 // 장치에 순서대로 바인딩

    for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHA])
    {
        if (FAILED(pGameObject->Render(CGameObject::TYPE::NORMAL)))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHA].clear();

    if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Deferred"))) { return E_FAIL; }

    return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
    for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHA])
    {
        if (FAILED(pGameObject->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHA].clear();

    return S_OK;
}

HRESULT CRenderer::Render_UI()
{
    for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_UI])
    {
        if (FAILED(pGameObject->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[(_uint)RENDERGROUP::RENDER_UI].clear();

    return S_OK;
}

HRESULT CRenderer::Render_Shadow()
{
    if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Shadow"))) { return E_FAIL; }

    m_pGraphic_Device->GetDepthStencilSurface(&m_pOriginalSurface);
    m_pGraphic_Device->SetDepthStencilSurface(m_pShadowSurface);

    m_pGraphic_Device->Clear(0, 0, D3DCLEAR_ZBUFFER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 1.f, 0);

    for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_SHADOW])
    {
        if (FAILED(pGameObject->Render_Shadow()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[(_uint)RENDERGROUP::RENDER_SHADOW].clear();

    if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Shadow"))) { return E_FAIL; }

    m_pGraphic_Device->SetDepthStencilSurface(m_pOriginalSurface);
    Safe_Release(m_pOriginalSurface);

    return S_OK;
}

HRESULT CRenderer::Render_Component()
{
    for (auto& pComponent : m_RenderComponent)
    {
        if (FAILED(pComponent->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pComponent);
    }

    m_RenderComponent.clear();

    return S_OK;
}

HRESULT CRenderer::Render_Outline()
{
    if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Outline"))) { return E_FAIL; }

    for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_OUTLINE])
    {
        if (FAILED(pGameObject->Render(CGameObject::TYPE::OUTLINE)))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[(_uint)RENDERGROUP::RENDER_OUTLINE].clear();

    if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Outline"))) { return E_FAIL; }

    return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
    if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_LightAcc"))) { return E_FAIL; }

    CLight_Manager* pLight_Manager = GET_INSTANCE(CLight_Manager);

    pLight_Manager->Render();

    RELEASE_INSTANCE(CLight_Manager);

    if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_LightAcc"))) { return E_FAIL; }

    return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
    if (FAILED(m_pShader->Set_Texture("g_DiffuseTexture",      m_pTarget_Manager->Get_TargetDesc(TEXT("Target_Diffuse")).pTexture)))      { return E_FAIL; }
    if (FAILED(m_pShader->Set_Texture("g_ShadeTexture",        m_pTarget_Manager->Get_TargetDesc(TEXT("Target_Shade")).pTexture)))        { return E_FAIL; }
    if (FAILED(m_pShader->Set_Texture("g_SpecularTexture",     m_pTarget_Manager->Get_TargetDesc(TEXT("Target_Specular")).pTexture)))     { return E_FAIL; }
    if (FAILED(m_pShader->Set_Texture("g_DepthTexture",        m_pTarget_Manager->Get_TargetDesc(TEXT("Target_Depth")).pTexture)))        { return E_FAIL; }
    if (FAILED(m_pShader->Set_Texture("g_SkyBoxTexture",       m_pTarget_Manager->Get_TargetDesc(TEXT("Target_SkyBox")).pTexture)))       { return E_FAIL; }
    if (FAILED(m_pShader->Set_Texture("g_OutlineTexture",      m_pTarget_Manager->Get_TargetDesc(TEXT("Target_Outline")).pTexture)))      { return E_FAIL; }
    if (FAILED(m_pShader->Set_Texture("g_ShadowDepthTexture",  m_pTarget_Manager->Get_TargetDesc(TEXT("Target_ShadowDepth")).pTexture)))  { return E_FAIL; }
    
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix ViewMatrixInv = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrixInv = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);
    D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);

    m_pShader->Set_ConstTable("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_matrix));

    _matrix LightViewMatrix = pGameInstance->Get_LightTransform(D3DTS_VIEW);
    _matrix LightProjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_LightViewMatrix", &LightViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_LightProjMatrix", &LightProjMatrix, sizeof(_matrix));

    RELEASE_INSTANCE(CGameInstance);

    if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Viewport"))) { return E_FAIL; }

    m_pShader->Begin_Shader(1);

    m_pVIBuffer->Render_VIBuffer();

    m_pShader->End_Shader();

    //Render_Component();

    if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Viewport"))) { return E_FAIL; }

    return S_OK;
}

HRESULT CRenderer::Set_RenderTarget(D3DVIEWPORT9 ViewPort)
{
    if (nullptr == m_pTarget_Manager)
    {
        return E_FAIL;
    }

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Diffuse",  ViewPort.Width, ViewPort.Height, D3DFMT_A8R8G8B8,      D3DXCOLOR(0.f, 0.f, 0.f, 0.f)))) { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Normal",   ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f)))) { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Shade",    ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)))) { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)))) { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Depth",    ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)))) { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Viewport", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)))) { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_SkyBox",   ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)))) { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Outline",  ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)))) { return E_FAIL; }

    if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_ShadowDepth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)))) { return E_FAIL; }

    _uint iShadowX = 1920;
    _uint iShadowY = 1080;

    if (FAILED(m_pGraphic_Device->CreateDepthStencilSurface(iShadowX, iShadowY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowSurface, nullptr)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Diffuse"))) { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Normal")))  { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Depth")))   { return E_FAIL; }

    if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Viewport", L"Target_Viewport"))) { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_SkyBox",   L"Target_SkyBox")))   { return E_FAIL; }

    if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Shade")))    { return E_FAIL; }
    if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Specular"))) { return E_FAIL; }

    if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Outline", L"Target_Outline"))) { return E_FAIL; }

    if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Shadow", L"Target_ShadowDepth"))) { return E_FAIL; }

    return S_OK;
}

CRenderer* CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CRenderer* pInstance = new CRenderer(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Create Renderer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CRenderer::Clone(void* pArg)
{
    AddRef();

    return this;
}

void CRenderer::Free()
{
    __super::Free();

    for (_uint i = 0; i < (_uint)RENDERGROUP::RENDER_END; ++i)
    {
        for (auto& pGameObject : m_RenderObjects[i])
        {
            Safe_Release(pGameObject);
        }

        m_RenderObjects[i].clear();
    }

	for (auto& pComponent : m_RenderComponent)
	{
		Safe_Release(pComponent);
	}

	m_RenderComponent.clear();

    Safe_Release(m_pTarget_Manager);
    Safe_Release(m_pShader);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pShadowSurface);
    Safe_Release(m_pOriginalSurface);
}
