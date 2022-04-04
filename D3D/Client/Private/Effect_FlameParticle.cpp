#include "pch.h"
#include "Effect_FlameParticle.h"
#include "GameInstance.h"
#include "Random.h"

CEffect_FlameParticle::CEffect_FlameParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlender(pGraphic_Device)
{}

CEffect_FlameParticle::CEffect_FlameParticle(const CEffect_FlameParticle& rhs)
	: CBlender(rhs)
{}

HRESULT CEffect_FlameParticle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_FlameParticle::NativeConstruct(void* pArg)
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

	if (TransformDesc.w == 0)
	{
		m_iIndex = TransformDesc.w;
	}
	else
	{
		m_iIndex = 1;
		m_fScale = TransformDesc.w;
	}

	return S_OK;
}

_int CEffect_FlameParticle::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_fFrame += 61.f * TimeDelta;;

	if (m_fFrame >= 61.f)
	{
		m_fFrame = 0.f;

		if (m_iIndex == 0)
		{
			m_bisDelete = true;

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance->Add_DeleteContainer(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), this)))
			{
				return E_FAIL;
			}

			RELEASE_INSTANCE(CGameInstance);
		}
	}

	return _int();
}

_int CEffect_FlameParticle::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix ViewMatrix;
	ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	
	m_pTransform->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransform->Set_State(CTransform::STATE_UP,    *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransform->Set_State(CTransform::STATE_LOOK,  *(_float3*)&ViewMatrix.m[2][0]);

	if (m_iIndex == 1)
	{
		_float3 vPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Flogas"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

		_float3 vCurrentPos = vPos;
		_float3 vLook = _float3(0.f, 1.f, 0.f);
		
		D3DXVec3Normalize(&vLook, &vLook);

		vCurrentPos += (vLook * 2.f);

		m_pTransform->Set_State(CTransform::STATE_POSITION, vCurrentPos);


		Compute_Z(m_pTransform);
	}

	m_pTransform->Set_Scale(_float3(7.f, 7.f, 7.f));

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
	{
		return -1;
	}

	return _int();
}

HRESULT CEffect_FlameParticle::Render()
{
	if (FAILED(Set_ConstTable()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_FlameParticle::Set_ConstTable()
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

	m_pShader->Begin_Shader(0);
	m_pVIBuffer->Render_VIBuffer();
	m_pShader->End_Shader();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEffect_FlameParticle::Add_Components()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Effect"), TEXT("Com_Shader"), (CComponent**)&m_pShader))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer))) { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_FlameParticle"), TEXT("Com_Texture"), (CComponent**)&m_pTexture))) { return E_FAIL; }

	return S_OK;
}

CEffect_FlameParticle* CEffect_FlameParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_FlameParticle* pInstance = new CEffect_FlameParticle(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_FlameParticle::Clone(void* pArg)
{
	CEffect_FlameParticle* pInstance = new CEffect_FlameParticle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_FlameParticle::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pFilterTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
}
