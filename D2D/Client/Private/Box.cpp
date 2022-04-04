#include "pch.h"
#include "Box.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Transform.h"

BEGIN(Client)

CBox::CBox(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

HRESULT CBox::NativeConstruct(void* pArg)
{
    memcpy(&m_BoxDesc, pArg, sizeof(BOXDESC));

    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    m_pTransform->Set_State(CTransform::STATE_POSITION, m_BoxDesc.vPos);
    m_pTransform->Set_Scale(m_BoxDesc.vScale);

    return S_OK;
}

_int CBox::Tick(_double DeltaTime)
{
    if (m_eLevelIndex != LEVEL_GAMEPLAY)
    {
        __super::Add_DeleteQueue(m_strMyLayerTag, this);
    }

    m_pCollider->Tick(DeltaTime);

    return _int();
}

_int CBox::Late_Tick(_double DeltaTime)
{
#ifdef _DEBUG

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_RenderComponent(m_pCollider)))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

#endif

    return _int();
}

HRESULT CBox::Render()
{
    return S_OK;
}

HRESULT CBox::Add_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(TEXT("Com_Transform"), m_pTransform = CTransform::Create(m_pGraphic_Device))))
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

HRESULT CBox::Set_ConstTable()
{
    return S_OK;
}

CBox* CBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CBox* pInstance = new CBox(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Box");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBox::Free()
{
    __super::Free();

    Safe_Release(m_pCollider);
    Safe_Release(m_pTransform);
}

END