#include "pch.h"
#include "Collider.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "GameInstance.h"

BEGIN(Client)

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent(pGraphic_Device)
{
}

HRESULT CCollider::NativeConstruct(void* pArg)
{
    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

    D3DXMatrixIdentity(&m_ColliderWorldMatrix);

    if (nullptr == m_ColliderDesc.pParentWorldMatrix)
    {
        D3DXMatrixIdentity(&m_IdentityMatrix);

        m_ColliderDesc.pParentWorldMatrix = &m_IdentityMatrix;
    }

    return S_OK;
}

_int CCollider::Tick(_double DeltaTime)
{
    _matrix ParentWorldMatrix = *m_ColliderDesc.pParentWorldMatrix;
    
    if (true == m_ColliderDesc.bCustomizable)
    {
        D3DXVec3Normalize((_float3*)&ParentWorldMatrix.m[0][0], (_float3*)&ParentWorldMatrix.m[0][0]);
        D3DXVec3Normalize((_float3*)&ParentWorldMatrix.m[1][0], (_float3*)&ParentWorldMatrix.m[1][0]);
        D3DXVec3Normalize((_float3*)&ParentWorldMatrix.m[2][0], (_float3*)&ParentWorldMatrix.m[2][0]);

        if (false == m_bDebug)
        {
            ParentWorldMatrix._11 = m_ColliderDesc.vInitScale.x;
            ParentWorldMatrix._22 = m_ColliderDesc.vInitScale.y;
            ParentWorldMatrix._33 = m_ColliderDesc.vInitScale.z;

            ParentWorldMatrix._41 += m_ColliderDesc.vInitPos.x;
            ParentWorldMatrix._42 += m_ColliderDesc.vInitPos.y;
            ParentWorldMatrix._43 += m_ColliderDesc.vInitPos.z;
        }
        else
        {
            ParentWorldMatrix._11 = m_vScale.x;
            ParentWorldMatrix._22 = m_vScale.y;
            ParentWorldMatrix._33 = m_vScale.z;

            ParentWorldMatrix._41 += m_vPos.x;
            ParentWorldMatrix._42 += m_vPos.y;
            ParentWorldMatrix._43 += m_vPos.z;
        }
        
    }

    m_pTransform->Set_WorldMatrix(ParentWorldMatrix);

    m_pTransform->Remove_Rotation();

    return _int();
}

HRESULT CCollider::Render()
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    m_pShader->Begin_Shader(2);

    m_pVIBuffer->Render();

    m_pShader->End_Shader();

    return S_OK;
}

_bool CCollider::OnCollision(CCollider* pCollider)
{
    /* AABB */

    _float3 vMyMin,    vMyMax;
    _float3 vBlockMin, vBlockMax;

    _matrix MyWorldMatrix    = *m_pTransform->Get_WorldMatrix();
    _matrix BlockWorldMatrix = *pCollider->m_pTransform->Get_WorldMatrix();

    D3DXVec3TransformCoord(&vMyMin, &m_vMin, &MyWorldMatrix);
    D3DXVec3TransformCoord(&vMyMax, &m_vMax, &MyWorldMatrix);

    D3DXVec3TransformCoord(&vBlockMin, &pCollider->m_vMin, &BlockWorldMatrix);
    D3DXVec3TransformCoord(&vBlockMax, &pCollider->m_vMax, &BlockWorldMatrix);

    if (max(vMyMin.x, vBlockMin.x) > min(vMyMax.x, vBlockMax.x))
    {
        m_bIsCollision = false;

        return false;
    }
    if (max(vMyMin.y, vBlockMin.y) > min(vMyMax.y, vBlockMax.y))
    {
        m_bIsCollision = false;

        return false;
    }

    m_bIsCollision = true;

    return true;
}

HRESULT CCollider::Add_Components()
{
    if (nullptr == (m_pTransform = CTransform::Create(m_pGraphic_Device)))
    {
        return E_FAIL;
    }

    if (nullptr == (m_pVIBuffer = CVIBuffer_Rect::Create(m_pGraphic_Device)))
    {
        return E_FAIL;
    }

    if (nullptr == (m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl"))))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CCollider::Set_ConstTable()
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
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix, sizeof(_matrix));

    m_pShader->Set_Bool("g_bIsCollision", (BOOL)m_bIsCollision);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
    CCollider* pInstance = new CCollider(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Collider");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCollider::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}

END