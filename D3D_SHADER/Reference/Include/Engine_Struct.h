#pragma once

BEGIN(Engine)

typedef struct MeshMaterialTexture
{
	enum class TYPE { TYPE_DIFFUSE, TYPE_NORMAL, TYPE_SPECULAR, TYPE_END };

	LPDIRECT3DTEXTURE9	pDiffuseMap  = nullptr;
	LPDIRECT3DTEXTURE9	pNormalMap   = nullptr;
	LPDIRECT3DTEXTURE9	pSpecularMap = nullptr;

}MESHMATERIALTEXTURE;

typedef struct D3DXMeshContainer_Derived : public D3DXMESHCONTAINER
{
	MESHMATERIALTEXTURE** ppMaterialTextures = nullptr;

	unsigned int iNumBones	   = 0;
	LPD3DXMESH	 pOriginalMesh = nullptr;

	D3DXMATRIX*  pOffsetMatrices				  = nullptr;
	D3DXMATRIX*  pRenderingMatrices				  = nullptr;
	D3DXMATRIX** ppCombinedTransformationMatrices = nullptr;

}D3DXMESHCONTAINER_DERIVED;

typedef struct D3DXFrame_Derived : public D3DXFRAME
{
	bool bIsCloned = false;

	D3DXMATRIX CombinedTransformationMatrix;

}D3DXFRAME_DERIVED;

typedef struct VertexTexture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR2 vTexUV;

}VTXTEX;

typedef struct VertexTexture_Viewport
{
	D3DXVECTOR4 vPosition;
	D3DXVECTOR2 vTexUV;

}VTXTEX_VIEWPORT;

typedef struct VertexCubeTexture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR3 vTexUV;

}VTXCUBETEX;

typedef struct VertexNormalTexture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vTexUV;

}VTXNORTEX;

typedef struct tagVertexTexture_Viewport
{
	D3DXVECTOR4 vPosition;
	D3DXVECTOR2 vTexUV;

}VTXTEX_VP;

typedef struct tagTargetDesc
{
	LPDIRECT3DTEXTURE9 pTexture;

	_int iWitdh;
	_int iHeight;

}TARGETDESC;

typedef struct tagPickingDesc
{
	_matrix WorldMatrixInv;

	_float3 vPos;
	_float3 vAxis[3];

	_float3 vMin;
	_float3 vMax;

}PICKINGDESC;

typedef struct tagLightDesc_Derived : public D3DLIGHT9
{
	_float3 vLightEye;
	_float3 vLightAt;
	_float3 vLightUp;
	
}D3DLIGHT9_DERIVED;

END