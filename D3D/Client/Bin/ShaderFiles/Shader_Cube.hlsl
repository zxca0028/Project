
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

vector g_vLightDir;
vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;

vector g_vMtrlAmbient  = vector(0.3f, 0.3f, 0.3f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vCamPos;

texture g_MtrlDiffuse;

sampler MtrlDiffuseSampler = sampler_state
{
	texture = g_MtrlDiffuse;

	mipfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3 vLocalPos    : POSITION;
	float3 vLocalNormal : NORMAL;
	float2 vTexUV       : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
	float  fShade    : COLOR0;
	float  fSpecular : COLOR1;
};

struct PS_IN
{
	float4 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
	float  fShade    : COLOR0;
	float  fSpecular : COLOR1;
};

struct PS_OUT
{
	float4 vColor : COLOR0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out;

	float4 vWorldPos    = mul(float4(In.vLocalPos,    1.f), g_WorldMatrix);
	float4 vWorldNormal = mul(float4(In.vLocalNormal, 0.f), g_WorldMatrix);

	Out.vPos = mul(vWorldPos, g_WorldMatrix);
	Out.vPos = mul(Out.vPos,  g_ViewMatrix);
	Out.vPos = mul(Out.vPos,  g_ProjMatrix);

	Out.vTexUV = In.vTexUV;

	/* Shade */
	Out.fShade = saturate(dot(normalize(g_vLightDir) * -1.f, normalize(vWorldNormal)));

	/* Reflect */
	vector Reflect = reflect(normalize(g_vLightDir), normalize(g_vCamPos));
	
	/* Specular */
	vector CamLook = vWorldPos - g_vCamPos;

	Out.fSpecular = pow(saturate(dot(normalize(Reflect) * -1.f, normalize(CamLook))), 20.f);

	return Out;
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out;

	vector vMtrlDiffuse = tex2D(MtrlDiffuseSampler, In.vTexUV);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(In.fShade + (g_vLightAmbient * g_vMtrlAmbient)) + (g_vLightSpecular * g_vMtrlSpecular) * In.fSpecular;

	return Out;
}

technique DefaultTechnique
{
	pass DefaultPass
	{
		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN();
	}
}