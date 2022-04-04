#include "Collider.h"
#include "Transform.h"
#include "Shader.h"
#include "PipeLine.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent(pGraphic_Device)
{
}

CCollider::CCollider(const CCollider& rhs)
    : CComponent(rhs)
    , m_pShader(rhs.m_pShader)
    , m_pTransform((CTransform*)rhs.m_pTransform->Clone()) 
    , m_pMesh(rhs.m_pMesh)
	, m_OriginMatrix(rhs.m_OriginMatrix)
	, m_ParentMatrix(rhs.m_ParentMatrix)
	, m_vOriginPos(rhs.m_vOriginPos)
{
    Safe_AddRef(m_pShader);
    Safe_AddRef(m_pMesh);
}

HRESULT CCollider::NativeConstruct_Prototype()
{
    m_pTransform = CTransform::Create(m_pGraphic_Device);

    if (nullptr == m_pTransform)
    {
        return E_FAIL;
    }

    m_pShader = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Collider.hlsl");

    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    HRESULT hr = 0;

    hr = D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pMesh, nullptr);

    return S_OK;
}

HRESULT CCollider::NativeConstruct(void* pArg)
{
    memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

    m_pTransform->Set_State(CTransform::STATE::STATE_POSITION, m_ColliderDesc.vInitPos);
    m_pTransform->Set_Scale(_float3(m_ColliderDesc.vScale.x, m_ColliderDesc.vScale.y, m_ColliderDesc.vScale.z));
   
    m_OriginMatrix = *m_pTransform->Get_WorldMatrix();

    m_vOriginPos = *(_float3*)m_OriginMatrix.m[3];

    return S_OK;
}

HRESULT CCollider::Render()
{
    if (nullptr == m_pMesh || nullptr == m_pShader)
    {
        return E_FAIL;
    }

    CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

    _matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix  = pPipeLine->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrix  = pPipeLine->Get_Transform(D3DTS_PROJECTION);

    RELEASE_INSTANCE(CPipeLine);

    m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix,  sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix,  sizeof(_matrix));

    m_pShader->Begin_Shader(0);

    m_pMesh->DrawSubset(0);

    m_pShader->End_Shader();

    return S_OK;
}

void CCollider::Update_Collider()
{
    m_ParentMatrix = *m_ColliderDesc.pParentMatrix;

    D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[0][0], (_float3*)&m_ParentMatrix.m[0][0]);
    D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[1][0], (_float3*)&m_ParentMatrix.m[1][0]);
    D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[2][0], (_float3*)&m_ParentMatrix.m[2][0]);

    m_pTransform->Set_WorldMatrix(m_OriginMatrix * m_ParentMatrix);
}

PICKINGDESC CCollider::Get_PickingDesc()
{
    _matrix WorldMatrixInv = m_pTransform->Get_WorldMatrixInv();

    m_PickingDesc.WorldMatrixInv = WorldMatrixInv;

    _float3 vScale = m_pTransform->Get_Scale();
    _float3 vPos   = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);

    _float3 vAxisX = m_pTransform->Get_State(CTransform::STATE::STATE_RIGHT);
    _float3 vAxisY = m_pTransform->Get_State(CTransform::STATE::STATE_UP);
    _float3 vAxisZ = m_pTransform->Get_State(CTransform::STATE::STATE_LOOK);

    _float3 vMin = vPos - vScale / 2.f;
    _float3 vMax = vPos + vScale / 2.f;

    //D3DXVec3TransformNormal(&vAxisX, &vAxisX, &WorldMatrixInv);
    //D3DXVec3TransformNormal(&vAxisY, &vAxisY, &WorldMatrixInv);
    //D3DXVec3TransformNormal(&vAxisZ, &vAxisZ, &WorldMatrixInv);
    
    //D3DXVec3TransformCoord(&vPos, &vPos, &WorldMatrixInv);
    //D3DXVec3TransformCoord(&vMin, &vMin, &WorldMatrixInv);
    //D3DXVec3TransformCoord(&vMax, &vMax, &WorldMatrixInv);

    m_PickingDesc.vPos = vPos;

    m_PickingDesc.vAxis[0] = vAxisX;
    m_PickingDesc.vAxis[1] = vAxisY;
    m_PickingDesc.vAxis[2] = vAxisZ;

    m_PickingDesc.vMin = vMin;
    m_PickingDesc.vMax = vMax;

    return m_PickingDesc;
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCollider* pInstance = new CCollider(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Create Collider");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
    CCollider* pInstance = new CCollider(*this);

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

    Safe_Release(m_pTransform);
    Safe_Release(m_pMesh);
    Safe_Release(m_pShader);
}
