#include "Mesh_Dynamic.h"
#include "HierarchyLoader.h"
#include "Shader.h"
#include "Animation.h"

CMesh_Dynamic::CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CMesh(pGraphic_Device)
{}

CMesh_Dynamic::CMesh_Dynamic(const CMesh_Dynamic& rhs)
    : CMesh(rhs)
    , m_pRootFrame(rhs.m_pRootFrame)
    , m_PivotMatrix(rhs.m_PivotMatrix)
    , m_MeshContainers(rhs.m_MeshContainers)
    , m_pAnimation(rhs.m_pAnimation->Clone())
    , m_pLoader(rhs.m_pLoader)
{

    Safe_AddRef(m_pLoader);

    _uint iNumMeshContainer = m_MeshContainers.size();

    for (_uint i = 0; i < iNumMeshContainer; ++i)
    {
        Safe_AddRef(m_MeshContainers[i]->MeshData.pMesh);
        Safe_AddRef(m_MeshContainers[i]->pOriginalMesh);

        for (_uint j = 0; j < m_MeshContainers[i]->NumMaterials; ++j)
        {
            Safe_AddRef(m_MeshContainers[i]->ppMaterialTextures[j]->pDiffuseMap);
            Safe_AddRef(m_MeshContainers[i]->ppMaterialTextures[j]->pSpecularMap);
            Safe_AddRef(m_MeshContainers[i]->ppMaterialTextures[j]->pNormalMap);
        }
    }
}

HRESULT CMesh_Dynamic::NativeConstruct_Prototype(const _tchar* pFilePath, const _tchar* pFileName)
{
    _tchar szFullPath[MAX_PATH] = TEXT("");

    lstrcpy(szFullPath, pFilePath);
    lstrcat(szFullPath, pFileName);

    m_pLoader = CHierarchyLoader::Create(m_pGraphic_Device, pFilePath);

    LPD3DXANIMATIONCONTROLLER pAnimationController = nullptr;

    if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device, m_pLoader, nullptr, &m_pRootFrame, &pAnimationController)))
    {
        return E_FAIL;
    }

    m_pAnimation = CAnimation::Create(m_pGraphic_Device, pAnimationController);

    if (nullptr == m_pAnimation)
    {
        return E_FAIL;
    }

    D3DXMatrixIdentity(&m_PivotMatrix);

    Update_CombinedTransformationMatrices(m_pRootFrame, m_PivotMatrix);

    Set_CombinedTransformationMatricesPointer(m_pRootFrame);

    return S_OK;
}

HRESULT CMesh_Dynamic::NativeConstruct(void* pArg)
{
    m_CombinedTransformationMatrixPointers.resize(m_MeshContainers.size());
    
    D3DXFRAME_DERIVED* pOriginalFrame = (D3DXFRAME_DERIVED*)m_pRootFrame;
    
    if (FAILED(Clone_Frame(pOriginalFrame)))
    {
        return E_FAIL;
    }
    
    for (_uint i = 0; i < m_MeshContainers.size(); ++i)
    {
        m_CombinedTransformationMatrixPointers[i] = new _matrix * [m_MeshContainers[i]->iNumBones];
    }
    
    _uint iIndex = 0;
    
    Set_CloneCombinedTransformationMatricesPointer(m_pRootFrame, iIndex);
    
    m_pAnimation->Bind_Frames((D3DXFRAME_DERIVED*)m_pRootFrame);

    return S_OK;
}

void CMesh_Dynamic::Update_CombinedTransformationMatrices(LPD3DXFRAME pFrame, _matrix ParentCombinedTransformationMatrix)
{
    D3DXFRAME_DERIVED* pFrame_Derived = (D3DXFRAME_DERIVED*)pFrame;

    pFrame_Derived->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * ParentCombinedTransformationMatrix;

    if (nullptr != pFrame_Derived->pFrameFirstChild)
    {
        Update_CombinedTransformationMatrices(pFrame_Derived->pFrameFirstChild, pFrame_Derived->CombinedTransformationMatrix);
    }
    if (nullptr != pFrame_Derived->pFrameSibling)
    {
        Update_CombinedTransformationMatrices(pFrame_Derived->pFrameSibling, ParentCombinedTransformationMatrix);
    }
}

void CMesh_Dynamic::Set_CombinedTransformationMatricesPointer(LPD3DXFRAME pFrame)
{
    if (nullptr != pFrame->pMeshContainer)
    {
        D3DXMESHCONTAINER_DERIVED* pMeshContainer_Derived = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

        m_MeshContainers.push_back(pMeshContainer_Derived);

        for (_uint i = 0; i < pMeshContainer_Derived->iNumBones; ++i)
        {
            const char* pBoneName = pMeshContainer_Derived->pSkinInfo->GetBoneName(i);

            D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
       
            pMeshContainer_Derived->ppCombinedTransformationMatrices[i] = &pFindFrame->CombinedTransformationMatrix;
        }
    }

    if (nullptr != pFrame->pFrameFirstChild)
    {
        Set_CombinedTransformationMatricesPointer(pFrame->pFrameFirstChild);
    }

    if (nullptr != pFrame->pFrameSibling)
    {
        Set_CombinedTransformationMatricesPointer(pFrame->pFrameSibling);
    }
}

void CMesh_Dynamic::Set_CloneCombinedTransformationMatricesPointer(LPD3DXFRAME pFrame, _uint& iIndex)
{
    if (nullptr != pFrame->pMeshContainer)
    {
        D3DXMESHCONTAINER_DERIVED* pMeshContainer_Derived = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;
    
        for (_uint i = 0; i < pMeshContainer_Derived->iNumBones; ++i)
        {
            const char* pBoneName = pMeshContainer_Derived->pSkinInfo->GetBoneName(i);

            D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);

            m_CombinedTransformationMatrixPointers[iIndex][i] = &pFindFrame->CombinedTransformationMatrix;
        }   

        ++iIndex;
    }

    if (nullptr != pFrame->pFrameFirstChild)
    {
        Set_CloneCombinedTransformationMatricesPointer(pFrame->pFrameFirstChild, iIndex);
    }
    if (nullptr != pFrame->pFrameSibling)
    {
        Set_CloneCombinedTransformationMatricesPointer(pFrame->pFrameSibling, iIndex);
    }
}

HRESULT CMesh_Dynamic::Clone_Frame(D3DXFRAME_DERIVED* pOriginalFrame)
{
    m_pRootFrame = new D3DXFRAME_DERIVED;
    memcpy(m_pRootFrame, pOriginalFrame, sizeof(D3DXFRAME_DERIVED));

    ((D3DXFRAME_DERIVED*)m_pRootFrame)->bIsCloned = true;

    if (nullptr != pOriginalFrame->pFrameFirstChild)
    {
        m_pRootFrame->pFrameFirstChild = new D3DXFRAME_DERIVED;
        Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameFirstChild, (D3DXFRAME_DERIVED*)m_pRootFrame->pFrameFirstChild);
    }
    if (nullptr != pOriginalFrame->pFrameSibling)
    {
        m_pRootFrame->pFrameSibling = new D3DXFRAME_DERIVED;
        Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameSibling, (D3DXFRAME_DERIVED*)m_pRootFrame->pFrameSibling);
    }


    return S_OK;
}

void CMesh_Dynamic::Clone_Frame(D3DXFRAME_DERIVED* pOriginalFrame, D3DXFRAME_DERIVED* pOut)
{
    memcpy(pOut, pOriginalFrame, sizeof(D3DXFRAME_DERIVED));

    pOut->bIsCloned = true;

    if (nullptr != pOriginalFrame->pFrameFirstChild)
    {
        pOut->pFrameFirstChild = new D3DXFRAME_DERIVED;
        Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameFirstChild, (D3DXFRAME_DERIVED*)pOut->pFrameFirstChild);
    }
    if (nullptr != pOriginalFrame->pFrameSibling)
    {
        pOut->pFrameSibling = new D3DXFRAME_DERIVED;
        Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameSibling, (D3DXFRAME_DERIVED*)pOut->pFrameSibling);
    }
}

HRESULT CMesh_Dynamic::Set_AnimationIndex(_uint iAnimationIndex, _float fAnimationSpeed)
{
    if (nullptr == m_pAnimation)
    {
        return E_FAIL;
    }

    return m_pAnimation->Set_AnimationIndex(iAnimationIndex, fAnimationSpeed);
}

HRESULT CMesh_Dynamic::Play_Animation(_double DeltaTime)
{
    if (nullptr == m_pAnimation)
    {
        return E_FAIL;
    }

    if (FAILED(m_pAnimation->Play_Animation(DeltaTime)))
    {
        return E_FAIL;
    }

    Update_CombinedTransformationMatrices(m_pRootFrame, m_PivotMatrix);

    return S_OK;
}

_double CMesh_Dynamic::Get_Period(_uint iAnimationIndex)
{
    if (nullptr == m_pAnimation)
    {
        return E_FAIL;
    }    

    return m_pAnimation->Get_Period(iAnimationIndex);
}

HRESULT CMesh_Dynamic::Set_Texture(CShader* pShader, D3DXHANDLE hParameter, MESHMATERIALTEXTURE::TYPE eType, _uint iMeshContainerIndex, _uint iMaterialIndex)
{
    LPDIRECT3DTEXTURE9		pTexture = nullptr;

    switch (eType)
    {
    case MeshMaterialTexture::TYPE::TYPE_DIFFUSE:

        pTexture = m_MeshContainers[iMeshContainerIndex]->ppMaterialTextures[iMaterialIndex]->pDiffuseMap;
        break;

    case MeshMaterialTexture::TYPE::TYPE_NORMAL:

        pTexture = m_MeshContainers[iMeshContainerIndex]->ppMaterialTextures[iMaterialIndex]->pNormalMap;
        break;

    case MeshMaterialTexture::TYPE::TYPE_SPECULAR:

        pTexture = m_MeshContainers[iMeshContainerIndex]->ppMaterialTextures[iMaterialIndex]->pSpecularMap;
        break;
    }

    if (FAILED(pShader->Set_Texture(hParameter, pTexture)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CMesh_Dynamic::Update_SkinnedMesh(_uint iMeshContainerIndex)
{
    _uint iSize = m_MeshContainers[iMeshContainerIndex]->iNumBones;

    for (_uint i = 0; i < iSize; ++i)
    {
        m_MeshContainers[iMeshContainerIndex]->pRenderingMatrices[i] = 
            m_MeshContainers[iMeshContainerIndex]->pOffsetMatrices[i] * 
            *m_CombinedTransformationMatrixPointers[iMeshContainerIndex][i];
    }
   
    void* pSour = nullptr;
    void* pDest = nullptr;

    m_MeshContainers[iMeshContainerIndex]->pOriginalMesh->LockVertexBuffer(0, &pSour);
    m_MeshContainers[iMeshContainerIndex]->MeshData.pMesh->LockVertexBuffer(0, &pDest);

    m_MeshContainers[iMeshContainerIndex]->pSkinInfo->UpdateSkinnedMesh(m_MeshContainers[iMeshContainerIndex]->pRenderingMatrices, nullptr, pSour, pDest);

    m_MeshContainers[iMeshContainerIndex]->pOriginalMesh->UnlockVertexBuffer();
    m_MeshContainers[iMeshContainerIndex]->MeshData.pMesh->UnlockVertexBuffer();

    return S_OK;
}

HRESULT CMesh_Dynamic::Render(_uint iMeshContainerIndex, _uint iMaterialIndex)
{
    _uint		iNumMeshContainer = m_MeshContainers.size();

    m_MeshContainers[iMeshContainerIndex]->MeshData.pMesh->DrawSubset(iMaterialIndex);

    return S_OK;
}

CMesh_Dynamic* CMesh_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName)
{
    CMesh_Dynamic* pInstance = new CMesh_Dynamic(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype(pFilePath, pFileName)))
    {
        MSGBOX("Failed to Creating Mesh_Dynamic");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CMesh_Dynamic::Clone(void* pArg)
{
    CMesh_Dynamic* pInstance = new CMesh_Dynamic(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating Mesh_Dynamic");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMesh_Dynamic::Free()
{
    __super::Free();

    _uint iNumMeshContainer = m_MeshContainers.size();

    for (_uint i = 0; i < iNumMeshContainer; ++i)
    {
        Safe_Release(m_MeshContainers[i]->MeshData.pMesh);
        Safe_Release(m_MeshContainers[i]->pOriginalMesh);

        for (_uint j = 0; j < m_MeshContainers[i]->NumMaterials; ++j)
        {
            Safe_Release(m_MeshContainers[i]->ppMaterialTextures[j]->pDiffuseMap);
            Safe_Release(m_MeshContainers[i]->ppMaterialTextures[j]->pSpecularMap);
            Safe_Release(m_MeshContainers[i]->ppMaterialTextures[j]->pNormalMap);
        }
    }

    m_MeshContainers.clear();

    m_pLoader->DestroyFrame(m_pRootFrame);

    for (auto& ppCombinedMatrixPointers : m_CombinedTransformationMatrixPointers)
    {
        Safe_Delete_Array(ppCombinedMatrixPointers);
    }

    m_CombinedTransformationMatrixPointers.clear();

    Safe_Release(m_pLoader);
    Safe_Release(m_pAnimation);
}
