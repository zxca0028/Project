#include "HierarchyLoader.h"

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
    :m_pGraphic_Device(pGraphic_Device)
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CHierarchyLoader::NativeConstruct(const _tchar* pMeshFilePath)
{
    m_pMeshFilePath = pMeshFilePath;

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
    D3DXFRAME_DERIVED* pFrame = new D3DXFRAME_DERIVED;
    ZeroMemory(pFrame, sizeof(D3DXFRAME_DERIVED));

    if (nullptr != Name)
    {
        Allocate_Name(Name, &pFrame->Name);
    }

    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

    *ppNewFrame = pFrame;

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::CreateMeshContainer(LPCSTR Name, const D3DXMESHDATA* pMeshData, const D3DXMATERIAL* pMaterials, const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
    D3DXMESHCONTAINER_DERIVED* pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
    ZeroMemory(pMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

    if (nullptr != Name)
    {
        Allocate_Name(Name, &pMeshContainer->Name);
    }

    D3DVERTEXELEMENT9 Element[] =
    {
        { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        { 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
        D3DDECL_END()
    };

    //D3DVERTEXELEMENT9 Element[MAX_FVF_DECL_SIZE];

    //pMeshData->pMesh->GetDeclaration(Element);

    if (FAILED(pMeshData->pMesh->CloneMesh(pMeshData->pMesh->GetOptions(), Element, m_pGraphic_Device, &pMeshContainer->MeshData.pMesh)))
    {
        return E_FAIL;
    }

    pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

    pMeshContainer->NumMaterials = NumMaterials;

    pMeshContainer->ppMaterialTextures = new MESHMATERIALTEXTURE*[NumMaterials];

    for (_uint i = 0; i < NumMaterials; ++i)
    {
        pMeshContainer->ppMaterialTextures[i] = new MESHMATERIALTEXTURE;
    }

    for (_uint i = 0; i < pMeshContainer->NumMaterials; ++i)
    {
        _tchar szFullPath[MAX_PATH] = TEXT("");

        lstrcpy(szFullPath, m_pMeshFilePath);

        _tchar szTextureFileName[MAX_PATH] = TEXT("");

        MultiByteToWideChar(CP_ACP, 0, pMaterials[i].pTextureFilename, strlen(pMaterials[i].pTextureFilename), szTextureFileName, MAX_PATH);
    
        lstrcat(szFullPath, szTextureFileName);

        _uint iLength = lstrlen(szFullPath);

        _uint iIndex = 0;

        for (_uint i = iLength; i >= 0; --i)
        {
            if (szFullPath[i] == L'.')
            {
                iIndex = i - 1;
                break;
            }
        }

        //szFullPath[iIndex] = L'D';
        D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->ppMaterialTextures[i]->pDiffuseMap);

        szFullPath[iIndex] = L'N';
        D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->ppMaterialTextures[i]->pNormalMap);
   
        ////szFullPath[iIndex] = L'D';
        //D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->ppMaterialTextures[i]->pSpecularMap);
    }

    pMeshContainer->pAdjacency = new _ulong[pMeshContainer->MeshData.pMesh->GetNumFaces() * 3];	
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * pMeshContainer->MeshData.pMesh->GetNumFaces() * 3);

    if (FAILED(D3DXComputeTangent(pMeshContainer->MeshData.pMesh, 0, 0, 0, 1, pMeshContainer->pAdjacency)))
    {
        return E_FAIL;
    }

    pMeshContainer->pSkinInfo = pSkinInfo;
    Safe_AddRef(pMeshContainer->pSkinInfo);

    pMeshContainer->pSkinInfo->SetDeclaration(Element);

    pMeshContainer->iNumBones = pMeshContainer->pSkinInfo->GetNumBones();
    
    pMeshContainer->pOffsetMatrices = new _matrix[pMeshContainer->iNumBones];

    for (_uint i = 0; i < pMeshContainer->iNumBones; ++i)
    {
        pMeshContainer->pOffsetMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
    }

    pMeshContainer->pRenderingMatrices = new _matrix[pMeshContainer->iNumBones];

    pMeshContainer->ppCombinedTransformationMatrices = new _matrix * [pMeshContainer->iNumBones];

    pMeshContainer->MeshData.pMesh->CloneMesh(pMeshContainer->MeshData.pMesh->GetOptions(), Element, m_pGraphic_Device, &pMeshContainer->pOriginalMesh);

    *ppNewMeshContainer = pMeshContainer;

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
    //Safe_Delete_Array(pFrameToFree->Name);
    //
    //if (nullptr != pFrameToFree->pMeshContainer)
    //    DestroyMeshContainer(pFrameToFree->pMeshContainer);
    //
    //if (nullptr != pFrameToFree->pFrameFirstChild)
    //    DestroyFrame(pFrameToFree->pFrameFirstChild);
    //
    //if (nullptr != pFrameToFree->pFrameSibling)
    //    DestroyFrame(pFrameToFree->pFrameSibling);
    //
    //Safe_Delete(pFrameToFree);

    if (false == ((D3DXFRAME_DERIVED*)pFrameToFree)->bIsCloned)
    {
        Safe_Delete_Array(pFrameToFree->Name);

        if (nullptr != pFrameToFree->pMeshContainer)
        {
            DestroyMeshContainer(pFrameToFree->pMeshContainer);
        }
    }

    if (nullptr != pFrameToFree->pFrameFirstChild)
    {
        DestroyFrame(pFrameToFree->pFrameFirstChild);
    }
    if (nullptr != pFrameToFree->pFrameSibling)
    {
        DestroyFrame(pFrameToFree->pFrameSibling);
    }

    Safe_Delete(pFrameToFree);

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
    D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

    for (_uint i = 0; i < pMeshContainer->NumMaterials; ++i)
    {
        Safe_Release(pMeshContainer->ppMaterialTextures[i]->pDiffuseMap);
        Safe_Release(pMeshContainer->ppMaterialTextures[i]->pSpecularMap);
        Safe_Release(pMeshContainer->ppMaterialTextures[i]->pNormalMap);

        Safe_Delete(pMeshContainer->ppMaterialTextures[i]);
    }
    Safe_Release(pMeshContainer->pOriginalMesh);
    Safe_Delete_Array(pMeshContainer->ppMaterialTextures);
    Safe_Delete_Array(pMeshContainer->pOffsetMatrices);
    Safe_Delete_Array(pMeshContainer->pRenderingMatrices);
    Safe_Delete_Array(pMeshContainer->ppCombinedTransformationMatrices);

    Safe_Delete_Array(pMeshContainer->Name);

    _ulong		dwRefCnt = 0;

    Safe_Release(pMeshContainer->MeshData.pMesh);
    
    Safe_Delete_Array(pMeshContainer->pAdjacency);
    Safe_Release(pMeshContainer->pSkinInfo);

    Safe_Delete(pMeshContainer);

    return S_OK;
}

void CHierarchyLoader::Allocate_Name(const char* pSourName, char** ppDestName)
{
    _uint iLength = strlen(pSourName);

    *ppDestName = new char[iLength + 1];

    strcpy(*ppDestName, pSourName);
}

CHierarchyLoader* CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pMeshFilePath)
{
    CHierarchyLoader* pInstance = new CHierarchyLoader(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pMeshFilePath)))
    {
        MSGBOX("Failed to Creating HierarchyLoader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CHierarchyLoader::Free()
{
    Safe_Release(m_pGraphic_Device);
}
