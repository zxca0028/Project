#include "pch.h"
#include "Point.h"
#include "GameInstance.h"
#include "Transform.h"
#include "VIBuffer_Point.h"
#include "Texture.h"
#include "Shader.h"

#include "Collider.h"

BEGIN(Client)

CPoint::CPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

HRESULT CPoint::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(100.f, 100.f, 0.f));

    return S_OK;
}

_int CPoint::Tick(_double DeltaTime)
{
    return _int();
}

_int CPoint::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this)))
    {
        return E_FAIL;
    }

#ifdef _DEBUG

    if (FAILED(pGameInstance->Add_RenderComponent(m_pCollider)))
    {
        return E_FAIL;
    }

#endif

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

HRESULT CPoint::Render()
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    m_pShader->Begin_Shader(0);

    m_pVIBuffer->Render();

    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CPoint::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(TEXT("Com_Transform"), m_pTransform = CTransform::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBuffer = CVIBuffer_Point::Create(m_pGraphic_Device, 3))))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Point.hlsl")))))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTexture = CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Seed_Outline.png")))))
    {
        return E_FAIL;
    }


    CCollider::COLLIDERDESC ColliderDesc;

    ColliderDesc.pParentWorldMatrix = m_pTransform->Get_WorldMatrix();

    if (FAILED(__super::Add_Component(TEXT("Com_Collider"), m_pCollider = CCollider::Create(m_pGraphic_Device, &ColliderDesc))))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CPoint::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix  = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrix  = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix,  sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix,  sizeof(_matrix));

    m_pShader->Set_Texture("g_MtrlDiffuse", m_pTexture);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CPoint* CPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CPoint* pInstance = new CPoint(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPoint::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform); 
	Safe_Release(m_pCollider);
}

END