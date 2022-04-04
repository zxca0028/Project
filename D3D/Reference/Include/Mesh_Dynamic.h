#pragma once

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Dynamic final : public CMesh
{
private:

	explicit CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Dynamic(const CMesh_Dynamic& rhs);
	virtual ~CMesh_Dynamic() = default;
	
public:

	_uint Get_NumMeshContainer() const
	{
		return m_MeshContainers.size();
	}

	_uint Get_NumMaterials(_uint iMeshContainerIndex) const
	{
		return m_MeshContainers[iMeshContainerIndex]->NumMaterials;
	}

	const _matrix* Get_BoneMatrix(const char* pBoneName)
	{
		return &((D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName))->CombinedTransformationMatrix;
	}

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar * pFilePath, const _tchar * pFileName);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:

	void Update_CombinedTransformationMatrices(LPD3DXFRAME pFrame, _matrix ParentCombinedTransformationMatrix);
	void Set_CombinedTransformationMatricesPointer(LPD3DXFRAME pFrame);
	void Set_CloneCombinedTransformationMatricesPointer(LPD3DXFRAME pFrame, _uint& iIndex);

	HRESULT Clone_Frame(D3DXFRAME_DERIVED* pOriginalFrame);
	void    Clone_Frame(D3DXFRAME_DERIVED* pOriginalFrame, D3DXFRAME_DERIVED* pOut);

	HRESULT Update_SkinnedMesh(_uint iMeshContainerIndex);
	HRESULT Set_AnimationIndex(_uint iAnimationIndex, _float fAnimationSpeed = 1.f);
	HRESULT Play_Animation(_double DeltaTime);
	_double Get_Period(_uint iAnimationIndex);

	HRESULT Set_TextureOnShader(class CShader* pShader, D3DXHANDLE hParameter, MESHMATERIALTEXTURE::TYPE eType, _uint iMeshContainerIndex, _uint iMaterialIndex);
	HRESULT Render(_uint iMeshContainerIndex, _uint iMaterialIndex);

private:

	LPD3DXFRAME				m_pRootFrame = nullptr;
	class CAnimation*		m_pAnimation = nullptr;
	class CHierarchyLoader* m_pLoader    = nullptr;
	_matrix				    m_PivotMatrix;

	vector<_matrix**>       m_CombinedTransformationMatrixPointers;

private:

	vector<D3DXMESHCONTAINER_DERIVED*> m_MeshContainers;

public:

	static  CMesh_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END