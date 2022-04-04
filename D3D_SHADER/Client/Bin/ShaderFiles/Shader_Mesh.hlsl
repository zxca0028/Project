
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

struct VS_IN
{
	float3 vPos    : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV  : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPos     : POSITION;
	float3 vNormal  : NORMAL;
	float2 vTexUV   : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
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

	return Out;
}

struct PS_IN
{
	float4 vPos     : POSITION;
	float3 vNormal  : NORMAL;
	float2 vTexUV   : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
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

	vector vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	
	Out.vDiffuse = vDiffuse;
	Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth   = vector(In.vProjPos.w / 300.f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);

	return Out;
}

struct VS_IN_OUTLINE
{
	float3 vPos    : POSITION;
	float3 vNormal : NORMAL;
};

struct VS_OUT_OUTLINE
{
	float4 vPos       : POSITION;
};

VS_OUT_OUTLINE VS_OUTLINE(VS_IN_OUTLINE In)
{
	VS_OUT_OUTLINE	Out = (VS_OUT_OUTLINE)0;

	vector vPos    = vector(In.vPos,    1.f);
	vector vNormal = vector(In.vNormal, 0.f);

	vPos = vPos + normalize(vNormal) * 0.3f;

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);
	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos = vPos;

	return Out;
}

struct PS_IN_OUTLINE
{
	float4 vPos       : POSITION;
};

struct PS_OUT_OUTLINE
{
	vector vColor : COLOR0;
};

PS_OUT_OUTLINE PS_OUTLINE(PS_IN_OUTLINE In)
{
	PS_OUT_OUTLINE	Out = (PS_OUT_OUTLINE)0;

	Out.vColor = vector(1.f, 0.f, 0.f, 1.f);

	return Out;
}

PS_OUT PS_TEST(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vDiffuse = vector(1.f, 1.f, 1.f, 1.f);
	Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth   = vector(In.vProjPos.w / 300.f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);

	return Out;
}

/* Shadow Shader */

struct VS_IN_SHADOW
{
	float3 vPos : POSITION;
};

struct VS_OUT_SHADOW
{
	float4 vPos : POSITION;
};

VS_OUT_SHADOW VS_SHADOW(VS_IN_SHADOW In)
{
	VS_OUT_SHADOW	Out = (VS_OUT_SHADOW)0;

	vector vPos = vector(In.vPos, 1.f);

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);
	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos = vPos;

	return Out;
}

struct PS_IN_SHADOW
{
	float4 vPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
	vector vShadowDepth : COLOR0;
};

PS_OUT_SHADOW PS_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW	Out = (PS_OUT_SHADOW)0;

	Out.vShadowDepth.r = In.vPos.w / 300.f;
	Out.vShadowDepth.a = 1.f;

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN();
	}

	pass OutlineRendering
	{
		//ZWriteEnable = false;
		//CullMode     = CW;

		VertexShader = compile vs_3_0 VS_OUTLINE();
		PixelShader  = compile ps_3_0 PS_OUTLINE();
	}

	pass TestObjectRendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_TEST();
	}

	pass ShadowRendering
	{
		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_SHADOW();
		PixelShader  = compile ps_3_0 PS_SHADOW();
	}
}