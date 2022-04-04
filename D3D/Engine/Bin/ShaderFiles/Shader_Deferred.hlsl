
/* Global */////////////////////////////////

vector	g_vLightDir;
vector	g_vLightPos;
float	g_fRange;
vector	g_vLightDiffuse;
vector	g_vLightAmbient;
vector	g_vLightSpecular;

matrix  g_LightProjMatrix;
matrix	g_LightViewMatrix;

vector	g_vCamPosition;
matrix	g_ProjMatrixInv;
matrix	g_ViewMatrixInv;

vector	g_vMtrlAmbient  = (vector)1.f;
vector	g_vMtrlSpecular = (vector)1.f;

////////////////////////////////////////////

/* Texture *////////////////////////////////

texture g_DebugTexture;

sampler DebugSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_DebugTexture;
};

texture	g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_DiffuseTexture;
};

texture	g_NormalTexture;

sampler NormalSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_NormalTexture;
};

texture	g_ShadeTexture;

sampler ShadeSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_ShadeTexture;
};

texture	g_DepthTexture;

sampler DepthSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;

	texture = g_DepthTexture;
};

texture	g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;

	texture = g_SpecularTexture;
};

texture g_ShadowDepthTexture;

sampler ShadowDepthSampler = sampler_state
{
	mipfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;

	texture = g_ShadowDepthTexture;
};

////////////////////////////////////////////

struct PS_IN
{
	float4	vPosition : POSITION;
	float2  vTexUV    : TEXCOORD0;
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

struct PS_OUT_LIGHTACC
{
	vector	vShade    : COLOR0;
	vector	vSpecular : COLOR1;
};

PS_OUT_LIGHTACC PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHTACC		Out = (PS_OUT_LIGHTACC)0;

	vector  vNormalDesc = tex2D(NormalSampler, In.vTexUV);
	vector	vNormal		= vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	////////////////////////////
	
	vector vDepthDesc  = tex2D(DepthSampler, In.vTexUV);
	float  fViewSpaceZ = vDepthDesc.r * 500.f;

	vector vWorldPos = (vector)0.f;

	vWorldPos.x = In.vTexUV.x *  2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.g;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewSpaceZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector vReflect = reflect(normalize(g_vLightDir), vNormal);
	vector vLook    = normalize(vWorldPos -g_vCamPosition);

	////////////////////////////

	if (1.f == vNormalDesc.a)
	{
		Out.vShade   = g_vLightDiffuse * max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient);
		Out.vShade.a = 1.f;

		Out.vSpecular.rgb = (g_vLightSpecular * g_vMtrlSpecular) * pow
		(max
		(dot
		(normalize
		(vReflect) * -1.f, vLook), 0.f), 30.f);
	}
	else
	{
		discard;
	}

	return Out;
}

PS_OUT_LIGHTACC PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHTACC		Out = (PS_OUT_LIGHTACC)0;

	vector  vNormalDesc = tex2D(NormalSampler, In.vTexUV);
	vector	vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	////////////////////////////

	vector vDepthDesc = tex2D(DepthSampler, In.vTexUV);
	float  fViewSpaceZ = vDepthDesc.r * 500.f;

	vector vWorldPos = (vector)0.f;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.g;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewSpaceZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector	vLightDir = vWorldPos - g_vLightPos;
	float	fDistance = length(vLightDir);

	float	fAtt = saturate((g_fRange - fDistance) / g_fRange);

	vector vReflect = reflect(normalize(vLightDir), vNormal);
	vector vLook = normalize(vWorldPos - g_vCamPosition);

	////////////////////////////

	if (1.f == vNormalDesc.a)
	{
		Out.vShade = (g_vLightDiffuse * max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
		Out.vShade.a = 1.f;

		Out.vSpecular.rgb = ((g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), 30.f)) * fAtt;
	}
	else
	{
		discard;
	}

	return Out;
}

struct PS_OUT_BLEND
{
	vector	vColor : COLOR0;
};

PS_OUT_BLEND PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT_BLEND	Out = (PS_OUT_BLEND)0;

	vector	vDiffuse  = tex2D(DiffuseSampler,  In.vTexUV);
	vector	vShade    = tex2D(ShadeSampler,    In.vTexUV);
	vector	vSpecular = tex2D(SpecularSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade + vSpecular;

	/*////////////////*/

	vector	vDepthDesc  = tex2D(DepthSampler, In.vTexUV);
	float	fViewSpaceZ = vDepthDesc.r * 500.f;

	vector vWorldPos = (vector)0.f;

	vWorldPos.x = In.vTexUV.x *  2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.g;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewSpaceZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector	vLightPos = vWorldPos;

	vLightPos = mul(vLightPos, g_LightViewMatrix);

	vector vUVPos = mul(vLightPos, g_LightProjMatrix);

	float2	vNewUV;

	vNewUV.x = (vUVPos.x / vUVPos.w) *  0.5f + 0.5f;
	vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

	vector	vShadowDepthInfo = tex2D(ShadowDepthSampler, vNewUV);

	if (vLightPos.z - 0.1f > vShadowDepthInfo.r * 500.f)
	{
		Out.vColor = vector(0.f, 0.f, 0.f, 1.f);
	}

	return Out;
}

technique DefaultTechnique
{
	pass Render_Debug
	{
		vertexshader = NULL;
		pixelshader  = compile ps_3_0 PS_MAIN_DEBUG();
	}

	pass Render_LightAcc_Directional
	{
		AlphaTestEnable = false;

		AlphaBlendEnable = true;
		BlendOp   = Add;
		SrcBlend  = One;
		DestBlend = One;

		ZWriteEnable = false;
		vertexshader = NULL;
		pixelshader  = compile ps_3_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Render_LightAcc_Point
	{
		AlphaTestEnable = false;

		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = One;
		DestBlend        = One;

		ZWriteEnable = false;
		vertexshader = NULL;
		pixelshader  = compile ps_3_0 PS_MAIN_LIGHT_POINT();
	}

	pass Render_Blend
	{
		ZWriteEnable    = false;
		AlphaTestEnable = true;
		AlphaFunc       = Greater;
		AlphaRef        = 0;

		vertexshader = NULL;
		pixelshader  = compile ps_3_0 PS_MAIN_BLEND();
	}
}