#include "BoundingBox.h"
#include "Transform.h"

CBoundingBox::CBoundingBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CBoundingBox::CBoundingBox(const CBoundingBox& rhs)
	: CComponent(rhs)
	, m_pTransform((CTransform*)rhs.m_pTransform->Clone())
	, m_BoxDesc(rhs.m_BoxDesc)
	, m_OriginalMatrix(rhs.m_OriginalMatrix)
	, m_ParentMatrix(rhs.m_ParentMatrix)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
{
}

HRESULT CBoundingBox::NativeConstruct_Prototype()
{
	m_pTransform = CTransform::Create(m_pGraphic_Device);

	if (nullptr == m_pTransform)
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBoundingBox::NativeConstruct(void* pArg)
{
	memcpy(&m_BoxDesc, pArg, sizeof(BOXDESC));

	return S_OK;
}

void CBoundingBox::Update_Box()
{
	m_ParentMatrix = *m_BoxDesc.pParentMatrix;

	D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[0][0], (_float3*)&m_ParentMatrix.m[0][0]);
	D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[1][0], (_float3*)&m_ParentMatrix.m[1][0]);
	D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[2][0], (_float3*)&m_ParentMatrix.m[2][0]);

	m_pTransform->Set_WorldMatrix(m_OriginalMatrix * m_ParentMatrix);
}

CBoundingBox* CBoundingBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoundingBox* pInstance = new CBoundingBox(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create BoundingBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CBoundingBox::Clone(void* pArg)
{
	CBoundingBox* pInstance = new CBoundingBox(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create BoundingBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoundingBox::Free()
{
	__super::Free();
}
