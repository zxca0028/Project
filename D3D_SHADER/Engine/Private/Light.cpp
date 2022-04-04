#include "Light.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Target_Manager.h"
#include "VIBuffer_Viewport.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
    :m_pGraphic_Device(pGraphic_Device)
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLight::NativeConstruct(const D3DLIGHT9_DERIVED& LightDesc)
{
    m_LightDesc = LightDesc;

    D3DVIEWPORT9 Viewport;
    m_pGraphic_Device->GetViewport(&Viewport);

    m_pVIBuffer = CVIBuffer_Viewport::Create(m_pGraphic_Device, -0.5f, -0.5f, Viewport.Width, Viewport.Height);
    if (nullptr == m_pVIBuffer)
    {
        return E_FAIL;
    }

    m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"));
    
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    D3DXMatrixLookAtLH(&m_ViewMatrix, &m_LightDesc.vLightEye, &m_LightDesc.vLightAt, &m_LightDesc.vLightUp);

    CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

    m_ProjMatrix = pPipeLine->Get_Transform(D3DTS_PROJECTION);

    RELEASE_INSTANCE(CPipeLine);

    return S_OK;
}

HRESULT CLight::Render()
{
    CTarget_Manager* pTarget_Manager = GET_INSTANCE(CTarget_Manager);
    CPipeLine*       pPipeLine       = GET_INSTANCE(CPipeLine);

    _uint iPassIndex = 0;

    if (D3DLIGHT_DIRECTIONAL == m_LightDesc.Type)
    {
        iPassIndex = 0;

        _float4 vLightDir = _float4(m_LightDesc.Direction, 0.f);

        m_pShader->Set_ConstTable("g_vLightDir", &vLightDir, sizeof(_float4));
    }
    else if (D3DLIGHT_POINT == m_LightDesc.Type)
    {
        iPassIndex = 3;

        _float4 vLightPos = _float4(m_LightDesc.Position, 1.f);
        _float fRange     = m_LightDesc.Range;

        m_pShader->Set_ConstTable("g_vLightPos", &vLightPos, sizeof(_float4));
        m_pShader->Set_ConstTable("g_fRange",    &fRange,    sizeof(_float));
    }

    _float4 CamPos = _float4(pPipeLine->Get_CamPos(), 1.f);

    _matrix ViewMatrixInv = pPipeLine->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrixInv = pPipeLine->Get_Transform(D3DTS_PROJECTION);
    
    D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);
    D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);

    m_pShader->Set_ConstTable("g_vLightDiffuse",  &m_LightDesc.Diffuse,  sizeof(_float4));
    m_pShader->Set_ConstTable("g_vLightAmbient",  &m_LightDesc.Ambient,  sizeof(_float4));
    m_pShader->Set_ConstTable("g_vLightSpecular", &m_LightDesc.Specular, sizeof(_float4));
    m_pShader->Set_ConstTable("g_vCamPos",        &CamPos,               sizeof(_float4));

    m_pShader->Set_ConstTable("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_matrix));

    if (FAILED(m_pShader->Set_Texture("g_NormalTexture", pTarget_Manager->Get_TargetDesc(TEXT("Target_Normal")).pTexture))) { return E_FAIL; }
    if (FAILED(m_pShader->Set_Texture("g_DepthTexture",  pTarget_Manager->Get_TargetDesc(TEXT("Target_Depth")).pTexture)))  { return E_FAIL; }

    m_pShader->Begin_Shader(iPassIndex);

    m_pVIBuffer->Render_VIBuffer();

    m_pShader->End_Shader();

    RELEASE_INSTANCE(CPipeLine);
    RELEASE_INSTANCE(CTarget_Manager);

    return S_OK;
}

_matrix CLight::Get_LightTransform(D3DTRANSFORMSTATETYPE eStateType)
{
    return eStateType == D3DTS_VIEW ? m_ViewMatrix : m_ProjMatrix;
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9_DERIVED& LightDesc)
{
    CLight* pInstance = new CLight(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(LightDesc)))
    {
        MSGBOX("Failed to Create Light");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLight::Free()
{
    Safe_Release(m_pShader);
    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pVIBuffer);
}
