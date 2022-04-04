
matrix  g_WorldMatrix;
matrix  g_ViewMatrix;
matrix  g_ProjMatrix;

struct VS_IN
{
	float3 vPosition : POSITION;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out;

	float4 vPosition = float4(In.vPosition.xyz, 1.f);

	vPosition = mul(vPosition, g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;

	return Out;
}

///////////////////////////////////////////////////

struct PS_IN
{
	float4 vPosition : POSITION;
};

struct PS_OUT
{
	float4 vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out;

	Out.vColor = float4(1.f, 0.f, 0.f, 1.f);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN();
	}
}
