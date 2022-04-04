#include "pch.h"
#include "Block.h"
#include "GameInstance.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "Transform.h"
#include "Grass.h"
#include "Collider.h"

BEGIN(Client)

CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

HRESULT CBlock::NativeConstruct(void* pArg)
{
    memcpy(&m_BlockDesc, pArg, sizeof(BLOCKDESC));

    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    if (m_BlockDesc.eType == POSITION_TOP)
    {
        m_pTransform->Rotation(180.f);
    }
  
    m_pTransform->Set_State(CTransform::STATE_POSITION, m_BlockDesc.vInitPos);
    m_pTransform->Set_Scale(m_BlockDesc.vScale);
    
    return S_OK;
}

_int CBlock::Tick(_double DeltaTime)
{
    m_pTransform->Move_Left(DeltaTime);

    _float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

    if (vPos.x <= m_BlockDesc.fDeletePos || true == m_bIsGameOver)
    {
        __super::Add_DeleteQueue(m_strMyLayerTag, this);
    }

    m_pCollider->Tick(DeltaTime);

    return _int();
}

_int CBlock::Late_Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_RenderObject(CRenderer::RENDER_OBJECT, this)))
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

HRESULT CBlock::Render()
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

HRESULT CBlock::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTexture = CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Pipe.png")))))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBuffer = CVIBuffer_Rect::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    _float fSpeed = m_BlockDesc.fSpeed;

    if (FAILED(__super::Add_Component(TEXT("Com_Transform"), m_pTransform = CTransform::Create(m_pGraphic_Device, &fSpeed))))
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

HRESULT CBlock::Set_ConstTable()
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

CBlock* CBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CBlock* pInstance = new CBlock(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create BackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBlock::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pCollider);
    Safe_Release(m_pTransform);
}

END