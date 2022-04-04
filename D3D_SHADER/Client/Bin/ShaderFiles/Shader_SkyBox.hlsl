
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

texture g_MtrlDiffuse;

sampler DiffuseSampler = sampler_state
{
	texture = g_MtrlDiffuse;

	mipfilter = none;
	magfilter = none;
};

struct VS_IN
{
	float3 vPos     : POSITION;
	float3 vNormal  : NORMAL;
	float3 vTexUV   : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPos   : POSITION;
	float3 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	vector vPos = vector(In.vPos, 1.f);

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);
	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos   = vPos;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4 vPos    : POSITION;
	float3 vTexUV  : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse = texCUBE(DiffuseSampler, In.vTexUV);

	Out.vColor = vDiffuse;

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		ZWriteEnable = false;
		CullMode     = CW;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN();
	}
};