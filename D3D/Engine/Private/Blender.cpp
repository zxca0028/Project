#include "Blender.h"
#include "GameInstance.h"
#include "Transform.h"

CBlender::CBlender(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{}

CBlender::CBlender(const CBlender& rhs)
    : CGameObject(rhs)
    , m_fDistZ(0.f)
{}

HRESULT CBlender::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CBlender::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    return S_OK;
}

_int CBlender::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    return _int();
}

_int CBlender::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    return _int();
}

HRESULT CBlender::Render()
{
    __super::Render();

    return S_OK;
}

void CBlender::Compute_Z(CTransform* pTransform)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _float3 vWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);

    _matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);

    _matrix CamWorldMatrix = *D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);

    _float3 vCameraPos = *(_float3*)&CamWorldMatrix.m[3][0];

    _float3 vDir = vWorldPos - vCameraPos;
    
    m_fDistZ = D3DXVec3Length(&vDir);

    m_fDistZ;

    RELEASE_INSTANCE(CGameInstance);
}

void CBlender::Free()
{
    __super::Free();
}
