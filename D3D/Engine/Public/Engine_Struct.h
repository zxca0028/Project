#pragma once

namespace Engine
{
	typedef struct MeshMaterialTexture
	{
		enum TYPE { TYPE_DIFFUSE, TYPE_NORMAL, TYPE_SPECULAR, TYPE_END };
		
		LPDIRECT3DTEXTURE9		pDiffuseMap	 = nullptr;
		LPDIRECT3DTEXTURE9		pNormalMap	 = nullptr;
		LPDIRECT3DTEXTURE9		pSpecularMap = nullptr;

	}MESHMATERIALTEXTURE;

	typedef struct D3DXMeshContainerDerived : public D3DXMESHCONTAINER
	{
		MESHMATERIALTEXTURE** ppMaterialTextures     = nullptr;

		LPD3DXMESH   pOriginalMesh					 = nullptr;
										            
		unsigned int iNumBones						 = 0;
		D3DXMATRIX*  pOffsetMatrices                 = nullptr;
		D3DXMATRIX*  pRenderingMatrices              = nullptr;
		D3DXMATRIX** ppCombindTransformationMatrices = nullptr;

	}D3DXMESHCONTAINER_DERIVED;

	typedef struct D3DXFrame_Derived : public D3DXFRAME
	{
		bool bIsCloned = false;

		D3DXMATRIX CombinedTransformationMatrix;

	}D3DXFRAME_DERIVED;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3 vPosition;
		D3DXVECTOR2 vTexUV;
	
	}VTXTEX;

	typedef struct tagVertexTexture_Viewport
	{
		D3DXVECTOR4 vPosition;
		D3DXVECTOR2 vTexUV;

	}VTXTEX_VP;

	typedef struct tagVertexCubeTexture
	{
		D3DXVECTOR3 vPosition;
		D3DXVECTOR3 vNormal;
		D3DXVECTOR3 vTexUV;

	}VTXCUBETEX;

	typedef struct tagVertexNormalTexture
	{
		D3DXVECTOR3 vPosition;
		D3DXVECTOR3 vNormal;
		D3DXVECTOR2 vTexUV;

	}VTXNORTEX;
}