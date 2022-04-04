
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float  g_fAlpha;

texture g_MtrlDiffuse;

sampler DiffuseSampler = sampler_state
{
	texture = g_MtrlDiffuse;

	mipfilter = none;
	magfilter = none;
};

struct VS_IN
{
	float3 vPos   : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPos   : POSITION;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out;

	matrix matWV, matWVP;

	matWV  = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPos   = mul(float4(In.vPos, 1.f), matWV);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4 vPos   : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out;

	Out.vColor = vector(1.f, 1.f, 0.f, 0.5f);

	return Out;
}

PS_OUT PS_MAIN2(PS_IN In)
{
	PS_OUT Out;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vColor = vMtrlDiffuse;

	return Out;
}

PS_OUT PS_MAIN3(PS_IN In)
{
	PS_OUT Out;

	Out.vColor = vector(0.f, 0.f, 0.f, g_fAlpha);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		Vertexshader = compile vs_2_0 VS_MAIN();
		Pixelshader  = compile ps_2_0 PS_MAIN();
	}

	pass DefaultRendering2
	{
		Vertexshader = compile vs_2_0 VS_MAIN();
		Pixelshader  = compile ps_2_0 PS_MAIN2();
	}

	pass DefaultRendering2
	{
		AlphaBlendEnable = true;
		//BlendOp = Add;
		SrcBlend  = SrcAlpha;
		DestBlend = InvSrcAlpha;

		Vertexshader = compile vs_2_0 VS_MAIN();
		Pixelshader = compile ps_2_0 PS_MAIN3();
	}
};