#include "Light.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Target_Manager.h"
#include "VIBuffer_Rect_Viewport.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device(pGraphic_Device)
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLight::NativeConstruct(const D3DLIGHT9& LightDesc)
{
    m_LightDesc = LightDesc;

    D3DVIEWPORT9 Viewport;
    m_pGraphic_Device->GetViewport(&Viewport);

    m_pVIBuffer = CVIBuffer_Rect_Viewport::Create(m_pGraphic_Device, -0.5f, -0.5f, Viewport.Width, Viewport.Height);
    if (nullptr == m_pVIBuffer)
    {
        return E_FAIL;
    }

    m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"));
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CLight::Render()
{
    CTarget_Manager* pTargetManager = GET_INSTANCE(CTarget_Manager);
    CPipeLine*       pPipeLine      = GET_INSTANCE(CPipeLine);

    _uint   iPassIndex = 0;

    if (D3DLIGHT_DIRECTIONAL == m_LightDesc.Type)
    {
        iPassIndex = 1;
        _float4 vLightDir = _float4(m_LightDesc.Direction, 0.f);
        m_pShader->Set_ConstTable("g_vLightDir", &vLightDir, sizeof(_float4));
    }
    else if (D3DLIGHT_POINT == m_LightDesc.Type)
    {
        iPassIndex = 2;
        _float4 vLightPos = _float4(m_LightDesc.Position, 1.f);
        _float  fRange = m_LightDesc.Range;
        m_pShader->Set_ConstTable("g_vLightPos", &vLightPos, sizeof(_float4));
        m_pShader->Set_ConstTable("g_fRange", &fRange, sizeof(_float));
    }

    _float4 CameraPosition = _float4(pPipeLine->Get_CamPosition(), 1.f);

    _matrix ViewMatrixInv = pPipeLine->Get_Transform(D3DTS_VIEW);
    D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);

    _matrix ProjMatrixInv = pPipeLine->Get_Transform(D3DTS_PROJECTION);
    D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);

    m_pShader->Set_ConstTable("g_vLightDiffuse", &m_LightDesc.Diffuse, sizeof(_float4));
    m_pShader->Set_ConstTable("g_vLightAmbient", &m_LightDesc.Ambient, sizeof(_float4));
    m_pShader->Set_ConstTable("g_vLightSpecular", &m_LightDesc.Specular, sizeof(_float4));
    m_pShader->Set_ConstTable("g_vCamPosition", &CameraPosition, sizeof(_float4));

    m_pShader->Set_ConstTable("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_matrix));

    if (FAILED(m_pShader->Set_TextureConstTable("g_NormalTexture", pTargetManager->Get_TargetTexture(TEXT("Target_Normal"))))) { return E_FAIL; }
    if (FAILED(m_pShader->Set_TextureConstTable("g_DepthTexture", pTargetManager->Get_TargetTexture(TEXT("Target_Depth"))))) { return E_FAIL; }

    m_pShader->Begin_Shader(iPassIndex);

    m_pVIBuffer->Render_VIBuffer();

    m_pShader->End_Shader();

    RELEASE_INSTANCE(CPipeLine);
    RELEASE_INSTANCE(CTarget_Manager);

    return S_OK;
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightDesc)
{
    CLight* pInstance = new CLight(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(LightDesc)))
    {
        MSGBOX("Failed to Creating Light");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLight::Free()
{
    Safe_Release(m_pShader);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pGraphic_Device);
}
