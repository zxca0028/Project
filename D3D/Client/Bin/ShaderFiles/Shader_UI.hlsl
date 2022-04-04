
matrix  g_WorldMatrix;
matrix  g_ViewMatrix;
matrix  g_ProjMatrix;

float3 g_vLook;
int g_ToF;
float g_fHPDelta;

texture g_tMtrlDiffuse;

sampler DiffuseSampler = sampler_state
{
	texture = g_tMtrlDiffuse;

	mipfilter = none;
	magfilter = none;
};

texture g_FrameDiffuse;

sampler FrameSampler = sampler_state
{
	texture = g_FrameDiffuse;

	mipfilter = none;
	magfilter = none;
};

texture g_HPDiffuse;

sampler HPSampler = sampler_state
{
	texture = g_HPDiffuse;

	mipfilter = none;
	magfilter = none;
};

struct VS_IN
{
	float3 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;

};

struct VS_OUT
{
	float4 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
	float3 vLocalPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	vector vPos = vector(In.vPos, 1.f);

	vPos = mul(vPos, g_WorldMatrix);
	vPos = mul(vPos, g_ViewMatrix);
	vPos = mul(vPos, g_ProjMatrix);

	Out.vPos = vPos;
	Out.vTexUV = In.vTexUV;
	Out.vLocalPos = In.vPos;

	return Out;
}

struct PS_IN
{
	float4 vPos      : POSITION;
	float2 vTexUV    : TEXCOORD0;
	float3 vLocalPos : TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	//float3 vColor = dot(vMtrlDiffuse.rgb, float3(0.222f, 0.707f, 0.071f));

	//Out.vColor = vector(vColor, vMtrlDiffuse.a);
	Out.vColor = vMtrlDiffuse;

	return Out;
}

PS_OUT PS_MAIN_COOLDOWN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	if (1 == g_ToF)
	{
		if (g_vLook.x >= 0)
		{
			if (In.vLocalPos.x > 0 && In.vLocalPos.x * g_vLook.y - In.vLocalPos.y * g_vLook.x < 0)
			{
				Out.vColor = vector(0.f, 0.f, 0.f, 0.f);
			}
			else
			{
				Out.vColor = vector(0.f, 0.f, 0.f, 0.7f);
			}
		}
		else
		{
			if (In.vLocalPos.x <= 0 && In.vLocalPos.x * g_vLook.y - In.vLocalPos.y * g_vLook.x > 0)
			{
				Out.vColor = vector(0.f, 0.f, 0.f, 0.7f);
			}
			else
			{
				Out.vColor = vector(0.f, 0.f, 0.f, 0.f);
			}
		}
	}
	else
	{
		Out.vColor = vector(0.f, 0.f, 0.f, 0.f);
	}

	return Out;
}

PS_OUT PS_MAIN_HP(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vFrameDiffuse = tex2D(FrameSampler, In.vTexUV);
	vector vHPDiffuse    = tex2D(HPSampler,    In.vTexUV);

	if (In.vLocalPos.x < g_fHPDelta)
	{
		Out.vColor = vHPDiffuse;
	}
	else
	{
		Out.vColor = vector(float3(0.f, 0.f, 0.f), 1.f);
	}

	if (In.vLocalPos.x > 0.48f && In.vLocalPos.x < 0.48f)
	{
		Out.vColor = vector(0.5f, 0.5f, 0.5f, 1.f);
	}

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 200;

		CullMode = none;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN();
	}

	pass CoolDownRendering
	{
		AlphaBlendEnable = true;
		//BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN_COOLDOWN();
	}

	pass HPRendering
	{
		AlphaBlendEnable = true;
		//BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader  = compile ps_2_0 PS_MAIN_HP();
	}
}