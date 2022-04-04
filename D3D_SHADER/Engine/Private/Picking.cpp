#include "Picking.h"
#include "PipeLine.h"
#include "Collider.h"

#include<DirectXMath.h>

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent(pGraphic_Device)
{
}

HRESULT CPicking::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CPicking::Compute_MousePos(_float2 MousePos, _float2 TextureSize)
{
    CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

    _float4 vMousePos;

    vMousePos.x = MousePos.x /  (TextureSize.x / 2.f) - 1.f;
    vMousePos.y = MousePos.y / -(TextureSize.y / 2.f) + 1.f;
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
    D3DXVec3TransformNormal(&m_vMouseRay,  &m_vMouseRay,   &ViewMatrixInv);

    D3DXVec3Normalize(&m_vMouseRay, &m_vMouseRay);

    RELEASE_INSTANCE(CPipeLine);

    return S_OK;
}

_bool CPicking::Intersect_Ray(class CCollider* pCollider)
{
    PICKINGDESC Desc = pCollider->Get_PickingDesc();

    //D3DXVec3TransformCoord(&m_vMousePivot, &m_vMousePivot, &Desc.WorldMatrixInv);
    //D3DXVec3TransformNormal(&m_vMouseRay,  &m_vMouseRay,   &Desc.WorldMatrixInv);

	_float MaxS = -FLT_MAX;
	_float MinT =  FLT_MAX;

	for (_uint i = 0; i < 3; ++i)
	{
		if (fabsf(m_vMouseRay[i]) <= 0.00005f)
		{
			if (m_vMousePivot[i] < Desc.vMin[i] || m_vMousePivot[i] > Desc.vMax[i])
			{
				return false;
			}
		}
		else
		{
			_float s = (Desc.vMin[i] - m_vMousePivot[i]) / m_vMouseRay[i];
			_float t = (Desc.vMax[i] - m_vMousePivot[i]) / m_vMouseRay[i];

			if (s > t)
			{
				_float fTemp = s;
				s = t;
				t = fTemp;
			}

			if (s > MaxS) MaxS = s;
			if (t < MinT) MinT = t;

			if (MinT < 0.f || MaxS > MinT)
			{
				return false;
			}
		}
	}

    return true;
}

CPicking* CPicking::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPicking* pInstance = new CPicking(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Create Picking");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CPicking::Clone(void* pArg)
{
    AddRef();

    return this;
}

void CPicking::Free()
{
    __super::Free();
}
