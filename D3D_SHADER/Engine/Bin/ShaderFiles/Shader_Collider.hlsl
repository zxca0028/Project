
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

bool g_IsIntersect = false;

struct VS_IN
{
	float3 vPos : POSITION;
};

struct VS_OUT
{
	float4 vPos :POSITION;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

	vector vPos = vector(In.vPos, 1.f);

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);
	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos = vPos;

	return Out;
}

struct PS_IN
{
	float4 vPos : POSITION;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(g_IsIntersect, 1.f - g_IsIntersect, 0.f, 1.f);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		Fillmode = WireFrame;
		Cullmode = none;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN();
	}
}