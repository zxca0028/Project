#include "Collider.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Transform.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{}

CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_eType(rhs.m_eType)
	, m_pTransform((CTransform*)rhs.m_pTransform->Clone())
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)

	#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
	#endif
{
	Safe_AddRef(m_pMesh);

	memcpy(m_vPoints, rhs.m_vPoints, sizeof(_float3) * 8);

	#ifdef _DEBUG
	Safe_AddRef(m_pShader);
	#endif
}

HRESULT CCollider::NativeConstruct_Prototype(COLLIDER eType)
{
	__super::NativeConstruct_Prototype();

	m_pTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransform)
	{
		return E_FAIL;
	}

	#ifdef _DEBUG
	if (FAILED(Compile_ShaderDebug()))
	{
		return E_FAIL;
	}
	#endif

	m_eType = eType;

	HRESULT hr = 0;

	switch (m_eType)
	{
	case CCollider::TYPE_AABB:

		hr = D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pMesh, nullptr);

		m_vMin = _float3(-0.5f, -0.5f, -0.5f);
		m_vMax = _float3( 0.5f,  0.5f,  0.5f);

		break;

	case CCollider::TYPE_OBB:

		hr = D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pMesh, nullptr);

		m_vPoints[0] = _float3(-0.5f,  0.5f, -0.5f);
		m_vPoints[1] = _float3( 0.5f,  0.5f, -0.5f);
		m_vPoints[2] = _float3( 0.5f, -0.5f, -0.5f);
		m_vPoints[3] = _float3(-0.5f, -0.5f, -0.5f);

		m_vPoints[4] = _float3(-0.5f,  0.5f,  0.5f);
		m_vPoints[5] = _float3( 0.5f,  0.5f,  0.5f);
		m_vPoints[6] = _float3( 0.5f, -0.5f,  0.5f);
		m_vPoints[7] = _float3(-0.5f, -0.5f,  0.5f);

		break;

	case CCollider::TYPE_SPHERE:

		hr = D3DXCreateSphere(m_pGraphic_Device, 0.5f, 20, 20, &m_pMesh, nullptr);

		break;
	}

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	//_float3 vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

	//vPosition.y += m_ColliderDesc.vScale.y;

	m_pTransform->Set_State(CTransform::STATE_POSITION, m_ColliderDesc.vInitPos);
	m_pTransform->Set_Scale(_float3(m_ColliderDesc.vScale.x, m_ColliderDesc.vScale.y, m_ColliderDesc.vScale.z));
	m_pTransform->Rotation_Axis(_float3(1.f, 0.f, 0.f), m_ColliderDesc.vRadians.x);
	m_pTransform->Rotation_Axis(_float3(0.f, 1.f, 0.f), m_ColliderDesc.vRadians.y);
	m_pTransform->Rotation_Axis(_float3(0.f, 0.f, 1.f), m_ColliderDesc.vRadians.z);

	m_fRadius *= m_ColliderDesc.vScale.x;

	m_OriginalMatrix = *m_pTransform->Get_WorldMatrix();

	vOriginPos = *(_float3*)m_OriginalMatrix.m[3];

	return S_OK;
}

void CCollider::Update_Collider()
{
	m_ParentMatrix = *m_ColliderDesc.pParentMatrix;

	D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[0][0], (_float3*)&m_ParentMatrix.m[0][0]);
	D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[1][0], (_float3*)&m_ParentMatrix.m[1][0]);
	D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[2][0], (_float3*)&m_ParentMatrix.m[2][0]);

	m_pTransform->Set_WorldMatrix(m_OriginalMatrix * m_ParentMatrix);

	if (m_eType == CCollider::TYPE_AABB)
	{
		m_pTransform->Remove_Rotation();
	}
}

void CCollider::Set_Collider(_float3 vScale, _float3 vPos)
{
	//_float3 vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

	//vPosition.y += m_ColliderDesc.vScale.y;

	//m_pTransform->Set_State(CTransform::STATE_POSITION, vInitPos);
	//m_pTransform->Set_Scale(_float3(vScale.x, vScale.y, vScale.z));
	//m_pTransform->Rotation_Axis(_float3(1.f, 0.f, 0.f), 0.f);
	//m_pTransform->Rotation_Axis(_float3(0.f, 1.f, 0.f), 0.f);
	//m_pTransform->Rotation_Axis(_float3(0.f, 0.f, 1.f), 0.f);

	*(_float3*)m_OriginalMatrix.m[3] = vOriginPos;

	_float3 vRight = *(_float3*)m_OriginalMatrix.m[0];
	_float3 vUp    = *(_float3*)m_OriginalMatrix.m[1];
	_float3 vLook  = *(_float3*)m_OriginalMatrix.m[2];

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp,    &vUp);
	D3DXVec3Normalize(&vLook,  &vLook);

	vRight *= vScale.x;
	vUp    *= vScale.y;
	vLook  *= vScale.z;

	m_OriginalMatrix.m[3][0] += vPos.x;
	m_OriginalMatrix.m[3][1] += vPos.y;
	m_OriginalMatrix.m[3][2] += vPos.z;
	
	*(_float3*)m_OriginalMatrix.m[0] = vRight;
	*(_float3*)m_OriginalMatrix.m[1] = vUp;
	*(_float3*)m_OriginalMatrix.m[2] = vLook;

	//m_OriginalMatrix = *m_pTransform->Get_WorldMatrix();
}

bool CCollider::Collision_AABB(CCollider* pTargetCollider)
{
	_float3 vSourMin, vSourMax;
	_float3 vDestMin, vDestMax;

	_matrix TransformMatrix = *m_pTransform->Get_WorldMatrix();

	D3DXVec3TransformCoord(&vSourMin, &m_vMin, &TransformMatrix);
	D3DXVec3TransformCoord(&vSourMax, &m_vMax, &TransformMatrix);

	D3DXVec3TransformCoord(&vDestMin, &pTargetCollider->m_vMin, pTargetCollider->m_pTransform->Get_WorldMatrix());
	D3DXVec3TransformCoord(&vDestMax, &pTargetCollider->m_vMax, pTargetCollider->m_pTransform->Get_WorldMatrix());

	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
	{
		m_isCollision = false;
		return false;
	}
	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
	{
		m_isCollision = false;
		return false;
	}
	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
	{
		m_isCollision = false;
		return false;
	}

	m_isCollision = true;

	return true;
}

bool CCollider::Collision_OBB(CCollider* pTargetCollider)
{
	_float3 vSourPoinst[8], vDestPoint[8];

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vSourPoinst[i], &m_vPoints[i],				    m_pTransform->Get_WorldMatrix());
		D3DXVec3TransformCoord(&vDestPoint[i],  &pTargetCollider->m_vPoints[i], pTargetCollider->m_pTransform->Get_WorldMatrix());
	}

	OBBDESC OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc(vSourPoinst);
	OBBDesc[1] = Compute_OBBDesc(vDestPoint);

	_float fDistance[3];

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = 
				fabs(D3DXVec3Dot(&OBBDesc[0].vCenterAxis[0], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[0].vCenterAxis[1], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[0].vCenterAxis[2], &OBBDesc[i].vAlignAxis[j]));

			fDistance[1] = 
				fabs(D3DXVec3Dot(&OBBDesc[1].vCenterAxis[0], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[1].vCenterAxis[1], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[1].vCenterAxis[2], &OBBDesc[i].vAlignAxis[j]));

			_float3 vCenterToCenter = OBBDesc[1].vCenter - OBBDesc[0].vCenter;

			fDistance[2] = 
				fabs(D3DXVec3Dot(&vCenterToCenter, &OBBDesc[i].vAlignAxis[j]));

			if (fDistance[2] > fDistance[0] + fDistance[1])
			{
				m_isCollision = false;
				return false;
			}
		}
	}

	m_isCollision = true;

	return true;
}

bool CCollider::Collision_SPHERE(CCollider* pTargetCollider)
{
	_float3 vTargetPos = pTargetCollider->m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vMyPos     = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = vTargetPos - vMyPos;

	_float fDistance = D3DXVec3Length(&vDir);

	if (fDistance < m_fRadius + pTargetCollider->m_fRadius)
	{
		m_isCollision = true;
		return true;
	}

	m_isCollision = false;

	return false;
}

CCollider::OBBDESC CCollider::Compute_OBBDesc(_float3* pPoints)
{
	OBBDESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	OBBDesc.vCenter		  = (pPoints[5] + pPoints[3]) * 0.5f;
	OBBDesc.vAlignAxis[0] =  pPoints[5] - pPoints[4];
	OBBDesc.vAlignAxis[1] =  pPoints[5] - pPoints[6];
	OBBDesc.vAlignAxis[2] =  pPoints[5] - pPoints[1];

	for (_uint i = 0; i < 3; ++i)
	{
		D3DXVec3Normalize(&OBBDesc.vAlignAxis[i], &OBBDesc.vAlignAxis[i]);
	}

	OBBDesc.vCenterAxis[0] = (pPoints[5] + pPoints[2]) * 0.5f - OBBDesc.vCenter;
	OBBDesc.vCenterAxis[1] = (pPoints[5] + pPoints[0]) * 0.5f - OBBDesc.vCenter;
	OBBDesc.vCenterAxis[2] = (pPoints[5] + pPoints[7]) * 0.5f - OBBDesc.vCenter;

	return OBBDesc;
}

#ifdef _DEBUG
HRESULT CCollider::Render_Debug()
{
	if (nullptr == m_pMesh || nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
	_matrix ViewMatrix = pPipeLine->Get_Transform(D3DTS_VIEW);
	_matrix ProjMatrix = pPipeLine->Get_Transform(D3DTS_PROJECTION);

	m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix,  sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix,  sizeof(_matrix));
	m_pShader->Set_Bool("g_isCollision", (BOOL)m_isCollision);

	RELEASE_INSTANCE(CPipeLine);

	m_pShader->Begin_Shader(0);

	m_pMesh->DrawSubset(0);

	m_pShader->End_Shader();

	return S_OK;
}

HRESULT CCollider::Render()
{
	if (nullptr == m_pMesh || nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
	_matrix ViewMatrix = pPipeLine->Get_Transform(D3DTS_VIEW);
	_matrix ProjMatrix = pPipeLine->Get_Transform(D3DTS_PROJECTION);

	m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ProjMatrix", &ProjMatrix, sizeof(_matrix));
	m_pShader->Set_Bool("g_isCollision", (BOOL)m_isCollision);

	RELEASE_INSTANCE(CPipeLine);

	m_pShader->Begin_Shader(0);

	m_pMesh->DrawSubset(0);

	m_pShader->End_Shader();

	return S_OK;
}

HRESULT CCollider::Compile_ShaderDebug()
{
	m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Collider.hlsl"));
	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}

	return S_OK;
}
#endif

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER eType)
{
	CCollider* pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSGBOX("Failed to Creating Collider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Collider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pMesh);

	#ifdef _DEBUG
	Safe_Release(m_pShader);
	#endif
}
