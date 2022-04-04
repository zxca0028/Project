
matrix  g_WorldMatrix;
matrix  g_ViewMatrix;
matrix  g_ProjMatrix;

texture g_DiffuseSampler;

int g_ToF;
float g_fHPDelta;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseSampler;

	mipfilter = none;
	magfilter = none;
};
struct VS_IN
{
	float3 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
	float3 vLocalPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	vector vPos = vector(In.vPos, 1.f);

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);
	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos      = vPos;
	Out.vTexUV    = In.vTexUV;
	Out.vLocalPos = In.vPos;

	return Out;
}

struct PS_IN
{
	float4 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
	float3 vLocalPos : TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	if (g_ToF == 0)
	{
		Out.vColor = vDiffuse;
	}
	else
	{
		if (In.vLocalPos.x < g_fHPDelta)
		{
			Out.vColor = vDiffuse;
		}
	}

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		//AlphaTestEnable = true;
		//AlphaFunc = Greater;
		//AlphaRef = 200;
		//
		//CullMode = none;

		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN();
	}

	pass EffectRendering
	{
		//AlphaTestEnable = true;
		//AlphaFunc = Greater;
		//AlphaRef = 200;
		//
		//CullMode = none;

		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = One;
		DestBlend = One;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader = compile ps_2_0 PS_MAIN();
	}
}