
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

///////////////////////
vector g_LightDir;
///////////////////////

struct VS_IN
{
	float3 vPos     : POSITION;
	float3 vNormal0 : NORMAL;
	float3 vNormal1 : TEXCOORD0;
	float3 vNormal2 : TEXCOORD1;
};

struct VS_OUT
{
	float4 vPos     : POSITION;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	vector vPos = vector(In.vPos, 1.f);

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);

	vector EyeToVertex = vPos;

	vector vNormal0 = vector(In.vNormal0, 0.f);
	vector vNormal1 = vector(In.vNormal1, 0.f);
	vector vNormal2 = vector(In.vNormal2, 0.f);

	vNormal0 = mul(vNormal0, g_WorldMatrix);
	vNormal0 = mul(vNormal0, g_ViewMatrix);

	vNormal1 = mul(vNormal1, g_WorldMatrix);
	vNormal1 = mul(vNormal1, g_ViewMatrix);

	vNormal2 = mul(vNormal2, g_WorldMatrix);
	vNormal2 = mul(vNormal2, g_ViewMatrix);

	float Dot0 = dot(EyeToVertex, vNormal1);
	float Dot1 = dot(EyeToVertex, vNormal2);

	if ((Dot0 * Dot1) < 0.f)
	{
		vPos += 0.1f * vNormal0;
	}

	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos = vPos;

	return Out;
}

struct PS_IN
{
	float4 vPos     : POSITION;
};

struct PS_OUT
{
	float4 vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(0.f, 1.f, 0.f, 1.f);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN();
	}
}