
matrix  g_WorldMatrix;
matrix  g_ViewMatrix;
matrix  g_ProjMatrix;

texture g_tMtrlDiffuse;

float g_fAlphaDelta;

sampler DiffuseSampler = sampler_state
{
	texture = g_tMtrlDiffuse;

	mipfilter = none;
	magfilter = none;
};

texture g_tFilterDiffuse;

sampler FilterSampler = sampler_state
{
	texture = g_tFilterDiffuse;

	mipfilter = none;
	magfilter = none;
};

texture g_TrailDiffuse;

sampler TrailSampler = sampler_state
{
	texture = g_TrailDiffuse;

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
	float4 vLocalPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	vector vPos = vector(In.vPos, 1.f);

	Out.vLocalPos = vPos;

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);
	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos      = vPos;
	Out.vTexUV    = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
	float4 vLocalPos : TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vColor = vMtrlDiffuse;

	return Out;
}

PS_OUT PS_MAIN_FILTER(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector vFilterDiffuse = tex2D(FilterSampler, In.vTexUV);
	
	Out.vColor = vMtrlDiffuse * vFilterDiffuse;

	//if (g_fRadius <= 0.5f)
	//{
	//	if ((In.vLocalPos.x) * (In.vLocalPos.x) + (In.vLocalPos.y) * (In.vLocalPos.y) <= g_fRadius * g_fRadius)
	//	{
	//		Out.vColor = vector(1.f, 0.f, 0.f, 0.5f);
	//	}
	//}

	//Out.vColor = vMtrlDiffuse * vFilterDiffuse;

	return Out;
}

PS_OUT PS_MAIN_TRAIL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(TrailSampler, In.vTexUV);

	Out.vColor = vMtrlDiffuse;

	return Out;
}

PS_OUT PS_MAIN_STATIC(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vColor = vMtrlDiffuse * g_fAlphaDelta;

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
		//BlendOp = Add;
		SrcBlend  = One;
		DestBlend = One;
		zenable = false;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN();
	}

	pass FilterRendering
	{
		AlphaBlendEnable = true;
		SrcBlend  = One;
		DestBlend = One;
		//zenable = false;

		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader = compile ps_2_0 PS_MAIN_FILTER();
	}

	pass TrailRendering
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend  = SrcAlpha;
		DestBlend = InvSrcAlpha;
		zenable = false;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader = compile ps_2_0 PS_MAIN_TRAIL();
	}

	pass StaticRendering
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = One;
		DestBlend = One;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader = compile ps_2_0 PS_MAIN_STATIC();
	}
}