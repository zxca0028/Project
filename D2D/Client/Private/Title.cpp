#include "pch.h"
#include "Title.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Texture.h"
#include "Shader.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"

BEGIN(Client)

CTitle::CTitle(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

HRESULT CTitle::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(g_iCenterX, g_iCenterY, 0.f));
    m_pTransform->Set_Scale(_float3(g_iWinCX, g_iWinCY, 0.f));

    return S_OK;
}

_int CTitle::Tick(_double DeltaTime)
{
    if (m_eLevelIndex != LEVEL_MAIN)
    {
        __super::Add_DeleteQueue(TEXT("Layer_Title"), this);
    }

	return _int();
}

_int CTitle::Late_Tick(_double DeltaTime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CTitle::Render()
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    m_pShader->Begin_Shader(1);

    m_pVIBuffer->Render();

    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CTitle::Add_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(TEXT("Com_Transform"), m_pTransform = CTransform::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBuffer = CVIBuffer_Rect::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
	{
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTexture = CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Main.png")))))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTitle::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &ProjMatrix, sizeof(_matrix));

    m_pShader->Set_Texture("g_MtrlDiffuse", m_pTexture);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CTitle* CTitle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CTitle* pInstance = new CTitle(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Title");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTitle::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}

END