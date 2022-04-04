//
//matrix		g_WorldMatrix;
//matrix		g_ViewMatrix;
//matrix		g_ProjMatrix;
//
//texture		g_DiffuseTexture;
//
//sampler DiffuseSampler = sampler_state 
//{  
//	texture = g_DiffuseTexture;	
//	mipfilter = linear;
//	magfilter = linear;
//};
//
//struct VS_IN
//{
//	float3		vPosition : POSITION;	
//	float3		vNormal : NORMAL;
//	float2		vTexUV : TEXCOORD0;			
//};
//
//struct VS_OUT
//{
//	float4		vPosition : POSITION;
//	float		fShade : COLOR0;
//	float		fSpecular : COLOR1;
//	float2		vTexUV : TEXCOORD0;
//};
//
//
//VS_OUT VS_MAIN(/* Á¤Á¡ */VS_IN In)
//{
//	VS_OUT			Out = (VS_OUT)0;	
//	
//	matrix			matWV, matWVP;
//
//	matWV = mul(g_WorldMatrix, g_ViewMatrix);
//	matWVP = mul(matWV, g_ProjMatrix);
//
//	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
//	Out.vTexUV = In.vTexUV;	
//
//	return Out;
//}
//
//struct PS_IN
//{
//	float4		vPosition : POSITION;
//	float		fShade : COLOR0;
//	float		fSpecular : COLOR1;
//	float2		vTexUV : TEXCOORD0;
//};
//
//struct PS_OUT
//{
//	float4		vColor : COLOR0;	
//};
//
//
//
//PS_OUT PS_MAIN(PS_IN In)
//{
//	PS_OUT			Out;
//
//	Out.vColor = vector(0.f, 0.f, 0.f, 1.f);	
//
//	return Out;	
//}
//
//technique DefaultTechnique
//{	
//	pass DefaultRendering
//	{	
//		AlphaTestEnable = true;
//		AlphaFunc = Greater;
//		AlphaRef = 30;
//		VertexShader = compile vs_3_0 VS_MAIN();
//		PixelShader = compile ps_3_0 PS_MAIN();
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

texture g_FilterDiffuse;

sampler FilterSampler = sampler_state
{
	texture = g_FilterDiffuse;

	mipfilter = linear;
	magfilter = linear;
};

texture g_TrailDiffuse;

sampler TrailSampler = sampler_state
{
	texture = g_TrailDiffuse;

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

	Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
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
	float4 vDiffuse : COLOR0;
	float4 vNormal  : COLOR1;
	float4 vDepth   : COLOR2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector vNormalDesc  = tex2D(NormalSampler,  In.vTexUV);

	float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

	Out.vDiffuse = vMtrlDiffuse;

	float3x3 TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, TBN);

	//Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth  = vector(In.vProjPos.w / 500.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MAIN_DYNAMIC(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector vNormalDesc = tex2D(NormalSampler, In.vTexUV);

	float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

	Out.vDiffuse = vMtrlDiffuse;

	float3x3 TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, TBN);

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.w / 500.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MAIN_FILTER(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector vNormalDesc = tex2D(NormalSampler, In.vTexUV);
	vector vFilterDiffuse = tex2D(FilterSampler, In.vTexUV);

	float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

	Out.vDiffuse = vMtrlDiffuse + vFilterDiffuse;

	float3x3 TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, TBN);

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.w / 500.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}

matrix	g_LightViewMatrix;
matrix	g_LightProjMatrix;

struct VS_IN_SHADOW
{
	float3	vPosition : POSITION;
};

struct VS_OUT_SHADOW
{
	float4	vPosition : POSITION;
	float4	vProjPos  : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN_SHADOW In)
{
	VS_OUT_SHADOW	Out = (VS_OUT_SHADOW)0;

	vector vPos = vector(In.vPosition, 1.f);

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_LightViewMatrix);
	vPos = mul(vPos, g_LightProjMatrix);

	Out.vPosition = vPos;
	Out.vProjPos  = vPos;

	return Out;
}

struct PS_IN_SHADOW
{
	float4	vPosition : POSITION;
	float4	vProjPos  : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
	vector	vLightDepth : COLOR0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW	Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / 500.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		AlphaTestEnable = true;
		AlphaFunc       = Greater;
		AlphaRef        = 30;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN();
	}

	pass DynamicMeshRendering
	{
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 30;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN_DYNAMIC();
	}

	pass FilterRendering
	{
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 30;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_FILTER();
	}

	pass ShadwRendering
	{
		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN_SHADOW();
		PixelShader  = compile ps_3_0 PS_MAIN_SHADOW();
	}
}