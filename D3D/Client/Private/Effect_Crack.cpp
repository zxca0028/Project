#include "pch.h"
#include "Effect_Crack.h"
#include "GameInstance.h"
#include "Random.h"

CEffect_Crack::CEffect_Crack(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlender(pGraphic_Device)
{}

CEffect_Crack::CEffect_Crack(const CEffect_Crack& rhs)
	: CBlender(rhs)
{}

HRESULT CEffect_Crack::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Crack::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}

	_float4 TransformDesc;
	memcpy(&TransformDesc, pArg, sizeof(_float4));

	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(TransformDesc));
	m_pTransform->Setup_Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	m_fScale = TransformDesc.w;

	m_pTransform->Set_Scale(_float3(m_fScale, m_fScale, m_fScale));

	RELEASE_INSTANCE(CRandom);


	return S_OK;
}

_int CEffect_Crack::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	
	m_fTimeAcc += TimeDelta;

	if (m_fTimeAcc > 2.f)
	{
		m_fAlphaDelta -= TimeDelta / 2.f;
	}
	if (m_fAlphaDelta <= 0.f)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		m_bisDelete = true;

		pGameInstance->Add_DeleteContainer(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), this);

		RELEASE_INSTANCE(CGameInstance);

	}

	return _int();
}

_int CEffect_Crack::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//_matrix ViewMatrix;
	//ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	//
	//D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	//
	//m_pTransform->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	//m_pTransform->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	//m_pTransform->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	Compute_Z(m_pTransform);

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
	{
		return -1;
	}

	return _int();
}

HRESULT CEffect_Crack::Render()
{
	if (FAILED(Set_ConstTable()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Crack::Set_ConstTable()
{
	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
	_matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	_matrix PorjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

	m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix,   sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix,    sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ProjMatrix",  &PorjMatrix,    sizeof(_matrix));
	m_pShader->Set_ConstTable("g_fAlphaDelta", &m_fAlphaDelta, sizeof(_float));

	m_pShader->Set_TextureConstTable("g_tMtrlDiffuse", m_pTexture);

	m_pShader->Begin_Shader(3);
	m_pVIBuffer->Render_VIBuffer();
	m_pShader->End_Shader();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEffect_Crack::Add_Components()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Effect"), TEXT("Com_Shader"), (CComponent**)&m_pShader))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Crack"), TEXT("Com_Texture"), (CComponent**)&m_pTexture))) { return E_FAIL; }

	return S_OK;
}

CEffect_Crack* CEffect_Crack::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Crack* pInstance = new CEffect_Crack(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Crack::Clone(void* pArg)
{
	CEffect_Crack* pInstance = new CEffect_Crack(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Crack::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
}
