#include "pch.h"
#include "Body.h"
#include "GameInstance.h"

CBody::CBody(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{}

CBody::CBody(const CBody& rhs)
    : CGameObject(rhs)
{}

HRESULT CBody::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CBody::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    D3DXMatrixIdentity(&m_OriginMatrix);

    CTransform* pMonsterTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Dragon"), TEXT("Com_Transform"));
    m_pParentWorldMatrix = pMonsterTransform->Get_WorldMatrix();

    CMesh_Dynamic* pMonsterMesh = (CMesh_Dynamic*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Dragon"), TEXT("Com_Mesh"));
    m_pParentBoneMatrix = pMonsterMesh->Get_BoneMatrix("Bip01-Spine2");

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

_int CBody::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    /*ImGui::Begin("Head_Collider");
    if (ImGui::DragFloat3("Position", vPos, 0.1f, -100.f, 100.f, "%.2f"))
    {
        m_pOBB->Set_Collider(vScale, vPos);
    }
    ImGui::Separator();
    if (ImGui::DragFloat3("Scale", vScale, 0.1f, 0.01f, 100.f, "%.2f"))
    {
        m_pOBB->Set_Collider(vScale, vPos);
    }
    ImGui::End();*/

    return _int();
}

_int CBody::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    _matrix WorldMatrix = m_OriginMatrix * (*m_pParentBoneMatrix * *m_pParentWorldMatrix);

    m_pTransformCom->Set_State(CTransform::STATE_RIGHT,   *(_float3*)&WorldMatrix.m[0][0]);
    m_pTransformCom->Set_State(CTransform::STATE_UP,       *(_float3*)&WorldMatrix.m[1][0]);
    m_pTransformCom->Set_State(CTransform::STATE_LOOK,     *(_float3*)&WorldMatrix.m[2][0]);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)&WorldMatrix.m[3][0]);

    return _int();
}

HRESULT CBody::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CBody::Add_Components(void* pArg)
{
    CTransform::TRANSFORMDESC TransformDesc;
    TransformDesc.fSpeedPerSec = 5.0f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc))) { return E_FAIL; }

    CCollider::COLLIDERDESC ColliderDesc;
    ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
    ColliderDesc.vScale   = vScale;
    ColliderDesc.vInitPos = vPos;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Collider_AABB"), TEXT("Com_ABB"), (CComponent**)&m_pOBB, &ColliderDesc))) { return E_FAIL; }

    return S_OK;
}

CBody* CBody::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBody* pInstance = new CBody(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating BodyCollider");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CBody::Clone(void* pArg)
{
    CBody* pInstance = new CBody(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating BodyCollider");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CBody::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pOBB);
}
