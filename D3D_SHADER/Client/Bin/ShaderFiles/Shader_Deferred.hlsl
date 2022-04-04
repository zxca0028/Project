
vector	g_vLightDir;
vector	g_vLightPos;
float	g_fRange;
vector	g_vLightDiffuse;
vector	g_vLightAmbient;
vector	g_vLightSpecular;

matrix	g_LightViewMatrix;
matrix	g_LightProjMatrix;

vector	g_vCamPos;
matrix	g_ViewMatrixInv;
matrix	g_ProjMatrixInv;

vector	g_vMtrlAmbient  = (vector)0.3f;
vector	g_vMtrlSpecular = (vector)0.3f;

texture g_DiffuseTexture;
texture g_NormalTexture;
texture g_ShadeTexture;
texture g_DepthTexture;
texture	g_SpecularTexture;
texture g_ShadowDepthTexture;
texture g_SkyBoxTexture;
texture g_OutlineTexture;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	mipfilter = linear;
	magfilter = linear;
};

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

	mipfilter = linear;
	magfilter = linear;
};

sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;

	mipfilter = linear;
	magfilter = linear;
};

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;

	mipfilter = linear;
	magfilter = linear;
	AddressU = clamp;
	AddressV = clamp;
};

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;

	mipfilter = linear;
	magfilter = linear;
};

sampler ShadowDepthSampler = sampler_state
{
	texture = g_ShadowDepthTexture;

	mipfilter = linear;
	magfilter = linear;
	AddressU = clamp;
	AddressV = clamp;
};

sampler SkyBoxSampler = sampler_state
{
	texture = g_SkyBoxTexture;

	mipfilter = linear;
	magfilter = linear;
};

sampler OutlineSampler = sampler_state
{
	texture = g_OutlineTexture;

	mipfilter = linear;
	magfilter = linear;
};

struct PS_IN
{
	float4 vPos   : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT_LIGHTACC
{
	vector vShade    : COLOR0;
	vector vSpecular : COLOR1;
};

PS_OUT_LIGHTACC PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHTACC	Out = (PS_OUT_LIGHTACC)0;

	vector vNormalDesc = tex2D(NormalSampler, In.vTexUV);
	vector vNormal     = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector vDepthDesc = tex2D(DepthSampler, In.vTexUV);
	float fViewSpaceZ = vDepthDesc.r * 300.f;

	vector vWorldPos = (vector)0.f;

	vWorldPos.x = In.vTexUV.x *  2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.g;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewSpaceZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector vReflect = reflect(normalize(g_vLightDir), vNormal);
	vector vLook    = normalize(vWorldPos - g_vCamPos);

	if (1.f == vNormalDesc.a)
	{
		Out.vShade   = g_vLightDiffuse * max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient);
		Out.vShade.a = 1.f;

		Out.vSpecular.rgb = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), 30.f);
		Out.vSpecular.a = 1.f;
	}
	else
	{
		discard;
	}

	return Out;
}

struct PS_OUT_BLEND
{
	vector vColor : COLOR0;
};

PS_OUT_BLEND PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT_BLEND	Out = (PS_OUT_BLEND)0;

	vector vDiffuse  = tex2D(DiffuseSampler,  In.vTexUV);
	vector vShade    = tex2D(ShadeSampler,    In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vSkyBox   = tex2D(SkyBoxSampler,   In.vTexUV);
	vector vOutline  = tex2D(OutlineSampler,  In.vTexUV);

	Out.vColor = vDiffuse * vShade + vSpecular;

	if (Out.vColor.a == 0.f)
	{
		Out.vColor = vSkyBox;
	}

	if (vOutline.a == 1.f)
	{
		Out.vColor = vOutline;
	}

	vector vDepth       = tex2D(DepthSampler, In.vTexUV);

	float fViewZ = In.vPos.w * 300.f;

	vector vPos = vector(0.f, 0.f, 0.f, 0.f);

	vPos.x = (In.vTexUV.x *  2.f - 1.f) * fViewZ;
	vPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPos.z = vDepth.y * fViewZ;
	vPos.w = fViewZ;

	vPos = mul(vPos, g_ProjMatrixInv);
	vPos = mul(vPos, g_ViewMatrixInv);
	vPos = mul(vPos, g_LightViewMatrix);
	vPos = mul(vPos, g_LightProjMatrix);

	float2 vNewUV;

	vNewUV.x = (vPos.x / vPos.w) *  0.5f + 0.5f;
	vNewUV.y = (vPos.y / vPos.w) * -0.5f + 0.5f;

	vector vShadowDepth = tex2D(ShadowDepthSampler, In.vTexUV);

	if (vPos.z - 0.f > vShadowDepth.x * 300.f)
	{
		//Out.vColor = vector(1.f, 0.f, 0.f, 1.f);
	}

	return Out;
}

texture g_DebugTexture;

sampler DebugSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_DebugTexture;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(DebugSampler, In.vTexUV);

	return Out;
}

PS_OUT_LIGHTACC PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHTACC	Out = (PS_OUT_LIGHTACC)0;

	vector vNormalDesc = tex2D(NormalSampler, In.vTexUV);
	vector vNormal     = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector vDepthDesc = tex2D(DepthSampler, In.vTexUV);
	float fViewSpaceZ = vDepthDesc.r * 300.f;

	vector vWorldPos = (vector)0.f;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.g;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewSpaceZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	/////////////////////////////////////////////////////////////////
	vector vLightDir = vWorldPos - g_vLightPos;
	float  fDistance = length(vLightDir);
	float  fAtt      = saturate((g_fRange - fDistance) / g_fRange);
	/////////////////////////////////////////////////////////////////

	vector vReflect = reflect(normalize(g_vLightDir), vNormal);
	vector vLook = normalize(vWorldPos - g_vCamPos);

	if (1.f == vNormalDesc.a)
	{
		Out.vShade = (g_vLightDiffuse * max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
		Out.vShade.a = 1.f;

		Out.vSpecular.rgb = ((g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), 30.f)) * fAtt;
		Out.vSpecular.a = 1.f;
	}
	else
	{
		discard;
	}

	return Out;
}

technique DefaultTechnique
{
	pass Render_LightAcc_Directional
	{
		AlphaTestEnable = false;

		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = One;
		DestBlend        = One;

		ZWriteEnable = false;

		VertexShader = NULL;
		PixelShader  = compile ps_3_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Render_Blend
	{
		ZWriteEnable    = false;
		AlphaTestEnable = true;
		AlphaFunc       = Greater;
		AlphaRef        = 0;

		VertexShader = NULL;
		PixelShader  = compile ps_3_0 PS_MAIN_BLEND();
	}

	pass Render_Debug
	{
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN_DEBUG();
	}

	pass Render_LightAcc_Point
	{
		AlphaTestEnable = false;

		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = One;
		DestBlend        = One;

		ZWriteEnable = false;

		VertexShader = NULL;
		PixelShader  = compile ps_3_0 PS_MAIN_LIGHT_POINT();
	}
}