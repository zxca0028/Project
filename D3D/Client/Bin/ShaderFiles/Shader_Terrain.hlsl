//
//matrix g_WorldMatrix;
//matrix g_ViewMatrix;
//matrix g_ProjMatrix;
//
//float2 g_Pos;
//float  g_Rad;
//
//vector g_vLightDir;
//vector g_vLightDiffuse;
//vector g_vLightAmbient;
//vector g_vLightSpecular;
//
//vector g_vMtrlAmbient  = vector(0.3f, 0.3f, 0.3f, 1.f);
//vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
//
//vector g_vCamPos;
//
//texture g_DiffuseSourTexture;
//texture g_DiffuseDestTexture;
//texture g_FilterTexture;
//texture g_MagicTexture;
//
//sampler DiffuseSourSampler = sampler_state
//{
//	texture = g_DiffuseSourTexture;
//
//	mipfilter = linear;
//	magfilter = linear;
//};
//
//sampler DiffuseDestSampler = sampler_state
//{
//	texture = g_DiffuseDestTexture;
//
//	mipfilter = linear;
//	magfilter = linear;
//};
//
//sampler FilterSampler = sampler_state
//{
//	texture = g_FilterTexture;
//
//	mipfilter = linear;
//	magfilter = linear;
//};
//
//sampler MagicSampler = sampler_state
//{
//	texture = g_MagicTexture;
//
//	mipfilter = linear;
//	magfilter = linear;
//};
//
//struct VS_IN
//{
//	float3 vPos    : POSITION;
//	float3 vNormal : NORMAL;
//	float2 vTexUV  : TEXCOORD0;
//};
//
//struct VS_OUT
//{
//	float4 vPos      : POSITION;
//	float  fShade    : COLOR0;
//	float  fSpecular : COLOR1;
//	float2 vTexUV    : TEXCOORD0;
//	float2 vMagicUV  : TEXCOORD1;
//};
//
//struct PS_IN
//{
//	float4 vPos      : POSITION;
//	float  fShade    : COLOR0;
//	float  fSpecular : COLOR1;
//	float2 vTexUV    : TEXCOORD0;
//	float2 vMagicUV  : TEXCOORD1;
//};
//
//struct PS_OUT
//{
//	float4 vColor : COLOR0;
//};
//
//VS_OUT VS_MAIN(VS_IN In)
//{
//	VS_OUT Out;
//
//	vector vWorldPos    = mul(vector(In.vPos,    1.f), g_WorldMatrix);
//	vector vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
//
//	Out.vPos   = mul(float4(In.vPos, 1.f), g_WorldMatrix);
//	Out.vPos   = mul(Out.vPos, g_ViewMatrix);
//	Out.vPos   = mul(Out.vPos, g_ProjMatrix);
//
//	Out.vTexUV   = In.vTexUV;
//	Out.vMagicUV = ((In.vTexUV + ((128 - g_Pos) / 128)) - 0.5);
//	Out.vMagicUV -= 0.5;
//	Out.vMagicUV *= 128 / g_Rad;
//	Out.vMagicUV += 0.5;
//
//	/* Diffuse */
//	Out.fShade = saturate(dot(normalize(g_vLightDir * -1.f), normalize(vWorldNormal)));
//
//	/* Reflect */
//	vector vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));
//
//	/* Specular */
//	vector vCamLook = vWorldPos - g_vCamPos;
//
//	Out.fSpecular = pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vCamLook))), 30.f);
//
//	return Out;
//}
//
//
//PS_OUT PS_MAIN(PS_IN In)
//{
//	PS_OUT Out;
//
//	vector vSourDiffuse = tex2D(DiffuseSourSampler, In.vTexUV * 64.f);
//	vector vDestDiffuse = tex2D(DiffuseDestSampler, In.vTexUV * 64.f);
//	vector vFilter	    = tex2D(FilterSampler,      In.vTexUV);
//	vector vMagic		= tex2D(MagicSampler,		In.vMagicUV);
//
//	vector vDiffuse = vFilter * vSourDiffuse + (1.f - vFilter) * vDestDiffuse;
//
//	Out.vColor = (g_vLightDiffuse * vDiffuse) * saturate(In.fShade + (g_vLightAmbient * g_vMtrlAmbient)) + (g_vLightSpecular * g_vMtrlSpecular) * In.fSpecular;
//
//	if (In.vMagicUV.x >= 0 && In.vMagicUV.x <= 1 && In.vMagicUV.y >= 0 && In.vMagicUV.y <= 1)
//	{
//		Out.vColor += vMagic;
//	}
//
//	return Out;
//}
//
//technique DefaultTechnique
//{
//	pass DefaultRendering
//	{
//		VertexShader = compile vs_2_0 VS_MAIN();
//		PixelShader  = compile ps_2_0 PS_MAIN();
//	}
//}

matrix  g_WorldMatrix;
matrix  g_ViewMatrix;
matrix  g_ProjMatrix;

texture g_tMtrlDiffuse;

sampler DiffuseSampler = sampler_state
{
	texture = g_tMtrlDiffuse;

	mipfilter = linear;
	magfilter = linear;
};

texture g_NormalDiffuse;

sampler NormalSampler = sampler_state
{
	texture = g_NormalDiffuse;

	mipfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3 vPos     : POSITION;
	float3 vNormal  : NORMAL;
	float2 vTexUV   : TEXCOORD0;
	float3 vTangent : TANGENT;
};

struct VS_OUT
{
	float4 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
	float3 vNormal   : NORMAL;
	float4 vProjPos  : TEXCOORD1;
	float3 vTangent  : TANGENT;
	float3 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	vector vPos = vector(In.vPos, 1.f);

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);
	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos     = vPos;
	Out.vTexUV   = In.vTexUV;
	Out.vNormal  = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vProjPos = vPos;

	Out.vTangent  = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent)).xyz;

	return Out;
}

struct PS_IN
{
	float4 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
	float3 vNormal   : NORMAL;
	float4 vProjPos  : TEXCOORD1;
	float3 vTangent  : TANGENT;
	float3 vBinormal : BINORMAL;
};

struct PS_OUT
{
	float4 vDiffuse  : COLOR0;
	float4 vNormal   : COLOR1;
	float4 vDepth    : COLOR2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV * 128.f);
	vector vNormalDesc  = tex2D(NormalSampler,  In.vTexUV * 128.f);

	float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

	Out.vDiffuse   = vMtrlDiffuse;
	Out.vDiffuse.a = 1.f;

	float3x3 TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	
	vNormal = mul(vNormal, TBN);

	//Out.vNormal    = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vNormal    = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth	   = vector(In.vProjPos.w / 500.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		//fillmode = wireframe;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN();
	}
}