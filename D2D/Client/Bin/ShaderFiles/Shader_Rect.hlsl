
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
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector vPos   : POSITION;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

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
	vector vPos   : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vColor = vDiffuse;

	return Out;
}

PS_OUT PS_COLLIDER(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(g_bIsCollision, 1.f - g_bIsCollision, 0.f , 1.f);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultPass
	{
		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN();
	}

	pass BlendPass
	{
		AlphaBlendEnable = true;

		SrcBlend  = SrcAlpha;
		DestBlend = InvSrcAlpha;
		Cullmode  = none;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN();
	}

	pass ColliderPass
	{
		Fillmode = WireFrame;
		Cullmode = none;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_COLLIDER();
	}
}