
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

texture g_MtrlDiffuse;

sampler DiffuseSampler = sampler_state
{
	texture = g_MtrlDiffuse;

	mipfilter = linear;
	magfilter = linear;
};

texture g_MtrlNormal;
		
sampler NormalSampler = sampler_state
{
	texture = g_MtrlNormal;

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
	float3 vNormal   : NORMAL;
	float2 vTexUV    : TEXCOORD0;
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
	Out.vProjPos = vPos;
	Out.vTexUV   = In.vTexUV;
	Out.vNormal  = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;

	Out.vTangent  = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent)).xyz;

	return Out;
}

struct PS_IN
{
	float4 vPos      : POSITION;
	float3 vNormal   : NORMAL;
	float2 vTexUV    : TEXCOORD0;
	float4 vProjPos  : TEXCOORD1;
	float3 vTangent  : TANGENT;
	float3 vBinormal : BINORMAL;
};

struct PS_OUT
{
	vector vDiffuse : COLOR0;
	vector vNormal  : COLOR1;
	vector vDepth   : COLOR2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV * 16.f);
	vector vNormalDesc  = tex2D(NormalSampler,  In.vTexUV * 16.f);

	float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3 TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, TBN);

	Out.vDiffuse   = vMtrlDiffuse;
	Out.vDiffuse.a = 1.f;
	Out.vNormal    = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth     = vector(In.vProjPos.w / 300.f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN();
	}
};