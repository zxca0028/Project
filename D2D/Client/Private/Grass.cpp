#include "pch.h"
#include "Grass.h"
#include "GameInstance.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "Transform.h"

BEGIN(Client)

CGrass::CGrass(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

HRESULT CGrass::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    m_pTransform->Set_State(CTransform::STATE_POSITION, m_GrassDesc.vInitPos);
    m_pTransform->Set_Scale(m_GrassDesc.vScale);

    return S_OK;
}

_int CGrass::Tick(_double DeltaTime)
{
    if (true == m_bIsGameOver)
    {
        m_bIsValid = false;

        __super::Add_DeleteQueue(m_strMyLayerTag, this);
    }

    m_pTransform->Move_Left(DeltaTime);

    _float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

    if (vPos.x <= m_GrassDesc.fDeletePos)
    {
        m_bIsValid = false;

        __super::Add_DeleteQueue(m_strMyLayerTag, this);
    }

    return _int();
}

_int CGrass::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_RenderObject(CRenderer::RENDER_OBJECT, this)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

    return _int();
}

HRESULT CGrass::Render()
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

HRESULT CGrass::Add_Components(void* pArg)
{
    memcpy(&m_GrassDesc, pArg, sizeof(GRASSDESC));

    if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTexture = CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Grass.png")))))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBuffer = CVIBuffer_Rect::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    _float fSpeed = m_GrassDesc.fSpeed;

    if (FAILED(__super::Add_Component(TEXT("Com_Transform"), m_pTransform = CTransform::Create(m_pGraphic_Device, &fSpeed))))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CGrass::Set_ConstTable()
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

CGrass* CGrass::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CGrass* pInstance = new CGrass(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create BackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGrass::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}

END