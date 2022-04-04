#include "pch.h"
#include "Sky.h"
#include "GameInstance.h"

CSky::CSky(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{}

CSky::CSky(const CSky& rhs)
	: CGameObject(rhs)
{}

HRESULT CSky::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CSky::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSky::Tick(_double DeltaTime)
{
	__super::Tick(DeltaTime);

	return _int();
}

_int CSky::Late_Tick(_double DeltaTime)
{
	__super::Late_Tick(DeltaTime);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix		ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);

	_matrix		CamWorldMatrix = *D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)&CamWorldMatrix.m[3][0]);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CSky::Render()
{
	__super::Render();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix vWorld = *(m_pTransformCom->Get_WorldMatrix());
	_matrix vView = pGameInstance->Get_Transform(D3DTS_VIEW);
	_matrix vProj = pGameInstance->Get_Transform(D3DTS_PROJECTION);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &vWorld);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &vView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &vProj);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	if (FAILED(m_pTextureCom->Set_Texture(0, 0)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSky::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"),       TEXT("Com_Renderer"),	   (CComponent**)&m_pRendererCom)))  { return E_FAIL; }
	if (FAILED(Add_Component(LEVEL_STATIC,   TEXT("Prototype_Transform"),      TEXT("Com_Transform"),      (CComponent**)&m_pTransformCom))) { return E_FAIL; }
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_VIBuffer_Cube"),  TEXT("Com_VIBuffer_Cube"),  (CComponent**)&m_pVIBufferCom)))  { return E_FAIL; }
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Skybox"), TEXT("Com_Texture_SkyBox"), (CComponent**)&m_pTextureCom)))   { return E_FAIL; }

	return S_OK;
}

CSky* CSky::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSky* pInstance = new CSky(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Sky");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSky::Clone(void* pArg)
{
	CSky* pInstance = new CSky(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Sky");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSky::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
