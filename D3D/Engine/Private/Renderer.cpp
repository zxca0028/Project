#include "Renderer.h"
#include "GameObject.h"
#include "Blender.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Shader.h"
#include "VIBuffer_Rect_Viewport.h"
#include "GameInstance.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
{
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	D3DVIEWPORT9 Viewport;
	m_pGraphic_Device->GetViewport(&Viewport);

	if (FAILED(Set_RenderTaget(Viewport)))
	{
		return E_FAIL;
	}

	m_pVIBuffer = CVIBuffer_Rect_Viewport::Create(m_pGraphic_Device, -0.5f, -0.5f, Viewport.Width, Viewport.Height);
	if (nullptr == m_pVIBuffer)
	{
		return E_FAIL;
	}

	m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"));
	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}

#ifdef _DEBUG

	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Diffuse"),       0.f,   0.f, 200.f, 200.f))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Normal"),        0.f, 200.f, 200.f, 200.f))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Specular"),      0.f, 400.f, 200.f, 200.f))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Shade"),       200.f,   0.f, 200.f, 200.f))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Depth"),       200.f, 200.f, 200.f, 200.f))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_ShadowDepth"), 200.f, 400.f, 200.f, 200.f))) { return E_FAIL; }

#endif

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER_GROUP eGroup, CGameObject* pGameObject)
{
	if (nullptr == pGameObject || RENDER_END <= eGroup)
	{
		return E_FAIL;
	}

	m_RenderObjects[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Add_RenderDebug(CComponent* pGameObject)
{
	m_DebugObject.push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

#endif

HRESULT CRenderer::Render_GameObject(_bool bRenderDebug)
{
	Render_Priority();

	Render_ShadowDepth();

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_Deferred")))) { return E_FAIL; }
	Render_NonAlpha();
	if (FAILED(m_pTarget_Manager->End_MRT(TEXT("MRT_Deferred"))))   { return E_FAIL; }

	Render_LightAcc();
	Render_Blend();

	Render_Alpha();
	Render_UI();

#ifdef _DEBUG

	if (true == bRenderDebug)
	{
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_Deferred")))) { return E_FAIL; }
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_LightAcc")))) { return E_FAIL; }
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_LightDepth")))) { return E_FAIL; }
	}
	

	for (auto& pDebugObj : m_DebugObject)
	{
		pDebugObj->Render();
		Safe_Release(pDebugObj);
	}

	m_DebugObject.clear();

#endif

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (true == pGameObject->IsDelete())
		{
			continue;
		}

		if (FAILED(pGameObject->Render()))
		{
			return E_FAIL;
		}

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_NONALPHA])
	{
		if (true == pGameObject->IsDelete())
		{
			continue;
		}

		if (FAILED(pGameObject->Render()))
		{
			return E_FAIL;
		}

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_NONALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
	m_RenderObjects[RENDER_ALPHA].sort([](CGameObject* p1, CGameObject* p2) {return ((CBlender*)p1)->Get_DistZ() > ((CBlender*)p2)->Get_DistZ(); });

	for (auto& pGameObject : m_RenderObjects[RENDER_ALPHA])
	{
		if (true == pGameObject->IsDelete())
		{
			continue;
		}

		if (FAILED(pGameObject->Render()))
		{
			return E_FAIL;
		}

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_ALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_UI])
	{
		if (true == pGameObject->IsDelete())
		{
			continue;
		}

		if (FAILED(pGameObject->Render()))
		{
			return E_FAIL;
		}

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Set_RenderTaget(D3DVIEWPORT9 Viewport)
{
	if (nullptr == m_pTarget_Manager)
	{
		return E_FAIL;
	}

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Diffuse"),     Viewport.Width, Viewport.Height, D3DFMT_A8R8G8B8,      D3DXCOLOR(0.f, 0.f, 0.f, 0.f)))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Normal"),      Viewport.Width, Viewport.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f)))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Shade"),       Viewport.Width, Viewport.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Specular"),    Viewport.Width, Viewport.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Depth"),       Viewport.Width, Viewport.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)))) { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_ShadowDepth"), Viewport.Width, Viewport.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)))) { return E_FAIL; }

	if (FAILED(m_pGraphic_Device->CreateDepthStencilSurface(1680, 720, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadow_DS_Surface, nullptr)))
	{ 
		return E_FAIL; 
	}

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"),   TEXT("Target_Diffuse"))))     { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"),   TEXT("Target_Normal"))))      { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"),   TEXT("Target_Depth"))))       { return E_FAIL; }
																  							    
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"),   TEXT("Target_Shade"))))	    { return E_FAIL; }
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"),   TEXT("Target_Specular"))))    { return E_FAIL; }

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth"), TEXT("Target_ShadowDepth")))) { return E_FAIL; }

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_LightAcc"))))
	{
		return E_FAIL;
	}

	CLight_Manager* pLight_Manager = GET_INSTANCE(CLight_Manager);

	pLight_Manager->Render();

	RELEASE_INSTANCE(CLight_Manager);

	if (FAILED(m_pTarget_Manager->End_MRT(TEXT("MRT_LightAcc"))))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (FAILED(m_pShader->Set_TextureConstTable("g_DiffuseTexture",     m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Diffuse")))))	    { return E_FAIL; }
	if (FAILED(m_pShader->Set_TextureConstTable("g_ShadeTexture",	    m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Shade")))))	    { return E_FAIL; }
	if (FAILED(m_pShader->Set_TextureConstTable("g_SpecularTexture",    m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Specular")))))    { return E_FAIL; }
	if (FAILED(m_pShader->Set_TextureConstTable("g_DepthTexture",       m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Depth")))))       { return E_FAIL; }
	if (FAILED(m_pShader->Set_TextureConstTable("g_ShadowDepthTexture", m_pTarget_Manager->Get_TargetTexture(TEXT("Target_ShadowDepth")))))	{ return E_FAIL; }

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix ViewMatrixInv = pGameInstance->Get_Transform(D3DTS_VIEW);
	_matrix ProjMatrixInv = pGameInstance->Get_Transform(D3DTS_PROJECTION);

	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);
	D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);

	D3DVIEWPORT9 Viewport;

	m_pGraphic_Device->GetViewport(&Viewport);

	_matrix LightProjMatrix = *D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(120.f), (_float)Viewport.X / (_float)Viewport.Y, 0.2f, 1000.f);

	_float3 vLightEye = _float3(-5.f, 15.f, -5.f);
	_float3 vLightAt  = _float3(60.f,  0.f, 60.f);
	_float3 vLightUp  = _float3(0.f, 1.f, 0.f);

	_matrix LightViewMatrix = *D3DXMatrixLookAtLH(&LightViewMatrix, &vLightEye, &vLightAt, &vLightUp);

	m_pShader->Set_ConstTable("g_ProjMatrixInv",   &ProjMatrixInv,	 sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ViewMatrixInv",   &ViewMatrixInv,   sizeof(_matrix));
	m_pShader->Set_ConstTable("g_LightViewMatrix", &LightViewMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_LightProjMatrix", &LightProjMatrix, sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstance);
	
	m_pShader->Begin_Shader(3);

	m_pVIBuffer->Render_VIBuffer();

	m_pShader->End_Shader();

	return S_OK;
}

HRESULT CRenderer::Render_ShadowDepth()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_LightDepth"))))
	{
		return E_FAIL;
	}

	m_pGraphic_Device->GetDepthStencilSurface(&m_pOriginal_DS_Surface);
	m_pGraphic_Device->SetDepthStencilSurface(m_pShadow_DS_Surface);

	m_pGraphic_Device->Clear(0, 0, D3DCLEAR_ZBUFFER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 1.f, 0);

	for (auto& pGameObject : m_RenderObjects[RENDER_SHADOWDEPTH])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_ShadowDepth();
		}

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_SHADOWDEPTH].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(TEXT("MRT_LightDepth"))))
	{
		return E_FAIL;
	}

	m_pGraphic_Device->SetDepthStencilSurface(m_pOriginal_DS_Surface);
	Safe_Release(m_pOriginal_DS_Surface);

	return S_OK;
}

CRenderer* CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer* pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Renderer");
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

	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderObjects[i])
		{
			Safe_Release(pGameObject);
		}

		m_RenderObjects[i].clear();
	}

	Safe_Release(m_pShadow_DS_Surface);
	Safe_Release(m_pOriginal_DS_Surface);

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTarget_Manager);
}
