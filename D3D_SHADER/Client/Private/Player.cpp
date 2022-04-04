#include "pch.h"
#include "Player.h"
#include "GameInstance.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
    : CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CPlayer::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    m_pTransform->Set_Scale(_float3(0.1f, 0.1f, 0.1f));

    return S_OK;
}

_int CPlayer::Tick(_double DeltaTime)
{ 
    m_pCollider->Update_Collider();

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (pGameInstance->Key_Pressing(KEY_UP))
    {
        m_pTransform->Move_Front(DeltaTime);
    }
    if (pGameInstance->Key_Pressing(KEY_DN))
    {
        m_pTransform->Move_Back(DeltaTime);
    }
    if (pGameInstance->Key_Pressing(KEY_RI))
    {
        m_pTransform->Move_Right(DeltaTime);
    }
    if (pGameInstance->Key_Pressing(KEY_LE))
    {
        m_pTransform->Move_Left(DeltaTime);
    }
    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CPlayer::Late_Tick(_double DeltaTime)
{
    /*if (true == m_pPicking->Intersect_Ray(m_pCollider))
    {
        if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_OUTLINE, this)))
        {
            return E_FAIL;
        }
    }*/
    if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this)))
    {
        return E_FAIL;
    }
    if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_SHADOW, this)))
    {
        return E_FAIL;
    }
    if (FAILED(m_pRenderer->Add_RenderGroup(m_pCollider)))
    {
        return E_FAIL;
    }

    return _int();
}

HRESULT CPlayer::Render(TYPE eType)
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    m_pShader->Begin_Shader((_uint)eType);

    _uint iNumMaterials = m_pMesh->Get_NumMaterials();

    for (_uint i = 0; i < iNumMaterials; ++i)
    {
        if (FAILED(m_pMesh->Set_Texture(m_pShader, "g_MtrlDiffuse", MESHMATERIALTEXTURE::TYPE::TYPE_DIFFUSE, i)))
        {                                         
            return E_FAIL;
        }

        m_pShader->Commit();

        m_pMesh->Render(i);
    }

    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CPlayer::Render_Shadow()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();

    /* ºûÀÇ ºä,Åõ¿µ */
    _matrix ViewMatrix  = pGameInstance->Get_LightTransform(D3DTS_VIEW);
    _matrix ProjMatrix  = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix,  sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix,  sizeof(_matrix));

    RELEASE_INSTANCE(CGameInstance);

    m_pShader->Begin_Shader(3);

    _uint iNumMaterials = m_pMesh->Get_NumMaterials();

    for (_uint i = 0; i < iNumMaterials; ++i)
    {
        m_pShader->Commit();

        m_pMesh->Render(i);
    }

    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CPlayer::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShader)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_STATIC, L"Prototype_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Mesh_Test", L"Com_Mesh", (CComponent**)&m_pMesh)))
    {
        return E_FAIL;
    }

    CTransform::TRANSFORMDESC TransformDesc;
    TransformDesc.fRotatePerSec = 30.f;
    TransformDesc.fSpeedPerSec  = 5.f;

    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
    {
        return E_FAIL;
    }

    CCollider::COLLIDERDESC ColliderDesc;
    ColliderDesc.pParentMatrix = m_pTransform->Get_WorldMatrix();
    ColliderDesc.vInitPos = _float3(0.f, 1.f, 0.f);
    ColliderDesc.vScale   = _float3(1.f, 2.f, 1.f);

    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Collider", L"Com_Collider", (CComponent**)&m_pCollider, &ColliderDesc)))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Picking", L"Com_Picking", (CComponent**)&m_pPicking)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CPlayer::Set_ConstTable()
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

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayer* pInstance = new CPlayer(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Create Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CPlayer* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer::Free()
{
    __super::Free();

    Safe_Release(m_pMesh);
    Safe_Release(m_pShader);
    Safe_Release(m_pPicking);
    Safe_Release(m_pCollider);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pTransform);
}
