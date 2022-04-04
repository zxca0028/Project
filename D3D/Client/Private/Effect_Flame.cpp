#include "pch.h"
#include "Effect_Flame.h"
#include "GameInstance.h"
#include "Random.h"
#include "Effect_FireExplosion.h"

CEffect_Flame::CEffect_Flame(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlender(pGraphic_Device)
{}

CEffect_Flame::CEffect_Flame(const CEffect_Flame& rhs)
	: CBlender(rhs)
{}

HRESULT CEffect_Flame::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Flame::NativeConstruct(void* pArg)
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

	CRandom* pRandom = GET_INSTANCE(CRandom);
	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(19.11f + pRandom->Get_RandomFloat(-10.f, 10.f), 20.5f, 135.07f + pRandom->Get_RandomFloat(-10.f, 10.f)));
	m_pTransform->Setup_Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	m_fScale = TransformDesc.w;

	RELEASE_INSTANCE(CRandom);

	return S_OK;
}

_int CEffect_Flame::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_fFrame += 32.f * TimeDelta;;

	if (m_fFrame >= 26.f && false == m_bLock)
	{
		m_bLock = true;

		_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

		D3DXVec3Normalize(&vLook, &vLook);

		vPos -= 3 * vLook;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		_float4 TransformDesc = _float4(vPos, 10.f);

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Effect_FireExplosion"), TEXT("Layer_Effect"), &TransformDesc)))
		{
			return E_FAIL;
		}

		RELEASE_INSTANCE(CGameInstance);
	}

	if (m_fFrame >= 32.f)
	{
		m_fFrame = 0.f;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		m_bisDelete = true;

		pGameInstance->Add_DeleteContainer(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), this);

		RELEASE_INSTANCE(CGameInstance);
	}

	return _int();
}

_int CEffect_Flame::Late_Tick(_double TimeDelta)
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
	
	m_pTransform->Set_Scale(_float3(m_fScale, m_fScale, m_fScale));
	
	Compute_Z(m_pTransform);

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
	{
		return -1;
	}

	return _int();
}

HRESULT CEffect_Flame::Render()
{
	if (FAILED(Set_ConstTable()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Flame::Set_ConstTable()
{
	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
	_matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	_matrix PorjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

	m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ProjMatrix", &PorjMatrix, sizeof(_matrix));

	m_pShader->Set_TextureConstTable("g_tMtrlDiffuse", m_pTexture, (_uint)m_fFrame);
	m_pShader->Set_TextureConstTable("g_tFilterDiffuse", m_pFilterTexture);

	m_pShader->Begin_Shader(1);
	m_pVIBuffer->Render_VIBuffer();
	m_pShader->End_Shader();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEffect_Flame::Add_Components()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Effect"), TEXT("Com_Shader"), (CComponent**)&m_pShader))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Flame"), TEXT("Com_Texture"), (CComponent**)&m_pTexture))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_FlameFilter"), TEXT("Com_FilterTexture"), (CComponent**)&m_pFilterTexture))) { return E_FAIL; }

	return S_OK;
}

CEffect_Flame* CEffect_Flame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Flame* pInstance = new CEffect_Flame(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Flame::Clone(void* pArg)
{
	CEffect_Flame* pInstance = new CEffect_Flame(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Flame::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pFilterTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
}
