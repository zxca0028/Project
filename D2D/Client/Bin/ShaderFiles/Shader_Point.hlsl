
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

bool g_bIsCollision = false;

texture g_MtrlDiffuse;

sampler DiffuseSampler = sampler_state
{
	texture = g_MtrlDiffuse;

	mipfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3 vPos   : POSITION;
	float  fSize  : PSIZE;
	float2 vTexUV : TEXCOORD0;

	float4 vRight       : TEXCOORD1;
	float4 vUp          : TEXCOORD2;
	float4 vLook        : TEXCOORD3;
	float4 vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float4 vPos   : POSITION;
	float  fSize  : PSIZE;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

	vector vPos = vector(In.vPos, 1.f);

	matrix VtxMatrix = matrix(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vPos = mul(vPos, VtxMatrix);
	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);
	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos  = vPos;
	Out.fSize = 107.f * In.fSize;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4 vPos   : POSITION;
	float  fSize  : PSIZE;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		PointSpriteEnable = true;

		AlphaBlendEnable = true;

		SrcBlend  = SrcAlpha;
		DestBlend = InvSrcAlpha;
		Cullmode  = none;


		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN();
	}
}