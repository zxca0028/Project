#include "pch.h"
#include "Sphere.h"
#include "GameInstance.h"

CSphere::CSphere(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CSphere::CSphere(const CSphere& rhs)
    : CGameObject(rhs)
{
}

HRESULT CSphere::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CSphere::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    m_pTransform->Set_Scale(_float3(0.1f, 0.1f, 0.1f));
    m_pTransform->Set_State(CTransform::STATE::STATE_POSITION, _float3(3.f, 2.f, 0.f));

    if (FAILED(D3DXCreateSphere(m_pGraphic_Device, 5.f, 100, 100, &m_pMesh, nullptr)))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CSphere::Tick(_double DeltaTime)
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
    if (pGameInstance->Key_Pressing(KEY_LE))
    {
        m_pTransform->Move_Left(DeltaTime);
    }
    if (pGameInstance->Key_Pressing(KEY_RI))
    {
        m_pTransform->Move_Right(DeltaTime);
    }

    if (pGameInstance->Key_Pressing(KEY_Z))
    {
        m_pTransform->Rotation(_float3(0.f, 1.f, 0.f), DeltaTime);
    }
    if (pGameInstance->Key_Pressing(KEY_C))
    {
        m_pTransform->Rotation(_float3(0.f, 1.f, 0.f), -DeltaTime);
    }

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CSphere::Late_Tick(_double DeltaTime)
{
    if (true == m_pPicking->Intersect_Ray(m_pCollider))
    {

    }
    if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pRenderer->Add_RenderGroup(m_pCollider)))
    {
        return E_FAIL;
    }
    return _int();
}

HRESULT CSphere::Render(TYPE eType)
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    m_pShader->Begin_Shader(2 - (_uint)eType);

    m_pMesh->DrawSubset(0);

    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CSphere::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShader)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_STATIC, L"Prototype_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
    {
        return E_FAIL;
    }

    CTransform::TRANSFORMDESC TransformDesc;
    TransformDesc.fRotatePerSec = 30.f;
    TransformDesc.fSpeedPerSec = 5.f;

    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Picking", L"Com_Picking", (CComponent**)&m_pPicking)))
    {
        return E_FAIL;
    }

    CCollider::COLLIDERDESC ColliderDesc;
    ColliderDesc.pParentMatrix = m_pTransform->Get_WorldMatrix();

    if (FAILED(__super::Add_Component((_uint)LEVEL::LEVEL_MAIN, L"Prototype_Collider", L"Com_Collider", (CComponent**)&m_pCollider, &ColliderDesc)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CSphere::Set_ConstTable()
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

CSphere* CSphere::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSphere* pInstance = new CSphere(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Create Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSphere::Clone(void* pArg)
{
    CSphere* pInstance = new CSphere(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSphere::Free()
{
    __super::Free();

    Safe_Release(m_pMesh);
    Safe_Release(m_pShader);
    Safe_Release(m_pPicking);
    Safe_Release(m_pCollider);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pTransform);
}
