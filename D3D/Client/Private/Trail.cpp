#include "pch.h"
#include "Trail.h"
#include "GameInstance.h"

CTrail::CTrail(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

HRESULT CTrail::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CTrail::NativeConstruct(void* pArg)
{
	memcpy(&m_TrailInfo, pArg, sizeof(TRAILINFO));

	MaxVTXCnt = 5000;

	MaxTriCnt = MaxVTXCnt - 2;
	VTXSize = sizeof(VTXTEX);

	m_Duration = 0.03f;
	AliveTime = 1;
	LerpCnt = 20;

	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(MaxVTXCnt * VTXSize, 0, m_dwFVF, D3DPOOL_MANAGED, &m_VertexBuffer, nullptr)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(sizeof(INDEX16) * MaxTriCnt, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, nullptr)))
	{
		return E_FAIL;
	}

	if (FAILED(Add_Component(pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CTrail::Tick(_double DeltaTime)
{
	return _int();
}

_int CTrail::Late_Tick(_double DeltaTime)
{
	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
	{
		return -1;
	}

	return _int();
}

HRESULT CTrail::Render()
{
	if (m_TrailDatas.size() <= 1)
	{
		return S_OK;
	}

	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix = *m_TrailInfo.pWorldMatrix;
	_matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	_matrix PorjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

	m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
	m_pShader->Set_ConstTable("g_ProjMatrix", &PorjMatrix, sizeof(_matrix));

	if (m_TrailInfo.iIndex == 2)
	{
		m_pShader->Set_TextureConstTable("g_TrailDiffuse", m_pTexture, 0);
	}
	else
	{
		m_pShader->Set_TextureConstTable("g_TrailDiffuse", m_pTexture, 1);
	}

	m_pShader->Begin_Shader(2);
	
	m_pGraphic_Device->SetStreamSource(0, m_VertexBuffer, 0, VTXSize);

	m_pGraphic_Device->SetFVF(m_dwFVF);

	m_pGraphic_Device->SetIndices(m_IndexBuffer);

	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, CurVTXCnt, 0, CurTriCnt);

	m_pShader->End_Shader();

	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

void CTrail::AddNewTrail(const _float3& UpPosition, const _float3& DownPosition, _double DeltaTime)
{
	m_fTimeAcc += DeltaTime;

	if (m_Duration < m_fTimeAcc)
	{
		TrailData Data(UpPosition, DownPosition);

		m_TrailDatas.emplace_back(Data);

		m_fTimeAcc = 0.f;
	}
}

void CTrail::UpdateTrail(_double DeltaTime)
{
	for (auto iter = m_TrailDatas.begin(); iter != m_TrailDatas.end();)
	{
		iter->TimeCount += DeltaTime;

		if (iter->TimeCount >= AliveTime)
		{
			iter = m_TrailDatas.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	if (m_TrailDatas.size() <= 1)
	{
		return ;
	}

	VTXTEX* pVertex = nullptr;
	INDEX16* pIndex = nullptr;

	m_VertexBuffer->Lock(0, 0, (void**)(&pVertex), 0);
	m_IndexBuffer->Lock(0, 0, (void**)(&pIndex), 0);

	size_t DataCnt = m_TrailDatas.size();
	_ulong Index = 0;

	for (size_t i = 0; i < DataCnt; ++i)
	{
		SplineTrailPosition(pVertex, i, Index, m_TrailInfo.pWorldMatrix);

		if (MaxVTXCnt <= Index)
		{
			return ;
		}
	}

	UVRate = 1.f / float(Index - 2);

	for (_uint i = 0; i < Index; i += 2)
	{
		pVertex[i].vTexUV = _float2(0.f, 1.f - (UVRate * i));
		pVertex[i + 1].vTexUV = _float2(1.f, 1.f - (UVRate * i));
	}

	CurVTXCnt = Index;
	CurTriCnt = CurVTXCnt - 2;

	for (_ulong i = 0; i < CurTriCnt; i += 2)
	{
		pIndex[i]._0 = i;
		pIndex[i]._1 = i + 1;
		pIndex[i]._2 = i + 3;
		pIndex[i + 1]._0 = i;
		pIndex[i + 1]._1 = i + 3;
		pIndex[i + 1]._2 = i + 2;
	}

	m_VertexBuffer->Unlock();
	m_IndexBuffer->Unlock();
}

void CTrail::SplineTrailPosition(VTXTEX* Vertex, const size_t& DataIndex, _ulong& Index, const _matrix* WorldMatrix)
{
	_matrix ToLocalMatrix;
	D3DXMatrixInverse(&ToLocalMatrix, 0, m_TrailInfo.pWorldMatrix);

	if (MaxVTXCnt <= Index)
	{
		return;
	}

	size_t iCurIndex = Index;

	D3DXVec3TransformCoord(&Vertex[Index].vPosition, &m_TrailDatas[DataIndex].Position[0], &ToLocalMatrix);

	++Index;

	if (MaxVTXCnt <= Index)
	{
		return;
	}

	D3DXVec3TransformCoord(&Vertex[Index].vPosition, &m_TrailDatas[DataIndex].Position[1], &ToLocalMatrix);

	++Index;

	if (MaxVTXCnt <= Index)
	{
		return;
	}

	_float3 vLerpPos[2];

	_ulong iSize = m_TrailDatas.size();

	for (_ulong i = 1; i < LerpCnt; ++i)
	{
		_int iEditIndexV0 = (DataIndex < 1 ? 0 : DataIndex - 1);
		_int iEditIndexV2 = (DataIndex + 1 >= iSize ? DataIndex : DataIndex + 1);
		_int iEditIndexV3 = (DataIndex + 2 >= iSize ? iEditIndexV2 : DataIndex + 2);

		D3DXVec3CatmullRom(&vLerpPos[0],
			&m_TrailDatas[iEditIndexV0].Position[0],
			&m_TrailDatas[DataIndex].Position[0],
			&m_TrailDatas[iEditIndexV2].Position[0],
			&m_TrailDatas[iEditIndexV3].Position[0],
			i / _float(LerpCnt));

		D3DXVec3CatmullRom(&vLerpPos[1],
			&m_TrailDatas[iEditIndexV0].Position[1],
			&m_TrailDatas[DataIndex].Position[1],
			&m_TrailDatas[iEditIndexV2].Position[1],
			&m_TrailDatas[iEditIndexV3].Position[1],
			i / _float(LerpCnt));

		D3DXVec3TransformCoord(&Vertex[Index].vPosition, &vLerpPos[0], &ToLocalMatrix);

		++Index;

		if (MaxVTXCnt <= Index)
		{
			return;
		}

		D3DXVec3TransformCoord(&Vertex[Index].vPosition, &vLerpPos[1], &ToLocalMatrix);

		++Index;

		if (MaxVTXCnt <= Index)
		{
			return;
		}
	}
}

HRESULT CTrail::Add_Component(void* pArg)
{
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_Effect"), TEXT("Com_Shader"),    (CComponent**)&m_pShader)))    { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"),      TEXT("Com_Renderer"),  (CComponent**)&m_pRenderer)))  { return E_FAIL; }
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Trail"), TEXT("Com_Texture"),   (CComponent**)&m_pTexture)))   { return E_FAIL; }

	return S_OK;
}

CTrail* CTrail::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CTrail* pInstance = new CTrail(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrail::Clone(void* pArg)
{
	return nullptr;
}

void CTrail::Free()
{
	__super::Free();

	Safe_Release(m_VertexBuffer);
	Safe_Release(m_IndexBuffer);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
}
