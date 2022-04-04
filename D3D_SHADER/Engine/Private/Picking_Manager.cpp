#include "Picking_Manager.h"
#include "PipeLine.h"
#include "Key_Manager.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CPicking_Manager)

CPicking_Manager::CPicking_Manager()
{
}

HRESULT CPicking_Manager::NativeConstruct_Prototype(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd)
{
    m_pGraphic_Device = pGraphic_Device;

    Safe_AddRef(m_pGraphic_Device);

    m_hWnd = hWnd;

    return S_OK;
}

_int CPicking_Manager::Tick(_float2 TextureSize)
{
    Compute_MousePos(TextureSize);

    CKey_Manager* pKey = GET_INSTANCE(CKey_Manager);

    if (pKey->Key_Down(KEY_LB))
    {
        m_bIsClick = true;
    }

    RELEASE_INSTANCE(CKey_Manager);

    return _int();
}

_int CPicking_Manager::Late_Tick()
{
    m_bIsClick = false;

    return _int();
}

_bool CPicking_Manager::IsIntersect_Ray(class CCollider* pCollider)
{
    
    return _bool();
}

HRESULT CPicking_Manager::Compute_MousePos(_float2 TextureSize)
{
    CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

    _float4 vMousePos;
    POINT   vMouseViewportPos;

    GetCursorPos(&vMouseViewportPos);
    ScreenToClient(m_hWnd, &vMouseViewportPos);

    D3DVIEWPORT9 Viewport;
    m_pGraphic_Device->GetViewport(&Viewport);

    vMousePos.x = vMouseViewportPos.x / (Viewport.Width  /  2.f) - 1.f;
    vMousePos.y = vMouseViewportPos.y / (Viewport.Height / -2.f) + 1.f;
    vMousePos.z = 0.f;
    vMousePos.w = 1.f;

    _matrix ProjMatrixInv = pPipeLine->Get_Transform(D3DTS_PROJECTION);
    D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);

    D3DXVec4Transform(&vMousePos, &vMousePos, &ProjMatrixInv);

    m_vMousePivot = _float3(0.f, 0.f, 0.f);
    m_vMouseRay   = _float3(vMousePos.x - m_vMousePivot.x, vMousePos.y - m_vMousePivot.y, vMousePos.z - m_vMousePivot.z);

    _matrix ViewMatrixInv = pPipeLine->Get_Transform(D3DTS_VIEW);
    D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);

     D3DXVec3TransformCoord(&m_vMousePivot, &m_vMousePivot, &ViewMatrixInv);
    D3DXVec3TransformNormal(&m_vMouseRay,   &m_vMouseRay,   &ViewMatrixInv);

    D3DXVec3Normalize(&m_vMouseRay, &m_vMouseRay);

    RELEASE_INSTANCE(CPipeLine);

    return S_OK;
}

void CPicking_Manager::Free()
{
    Safe_Release(m_pGraphic_Device);
}
