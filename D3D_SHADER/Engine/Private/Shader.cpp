#include "Shader.h"
#include "Texture.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent(pGraphic_Device)
{
}

CShader::CShader(const CShader& rhs)
    : CComponent(rhs)
    , m_pEffect(rhs.m_pEffect)
{
    Safe_AddRef(m_pEffect);
}

HRESULT CShader::NativeConstruct_Prototype(wstring pShaderFilePath)
{
    LPD3DXBUFFER    pErrorBuffer = nullptr;

    if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, (LPCWSTR)pShaderFilePath.c_str(), nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &pErrorBuffer)))
    {
        return E_FAIL;
    }

    Safe_Release(pErrorBuffer);

    return S_OK;
}

HRESULT CShader::NativeConstruct(void* pArg)
{
    return S_OK;
}

HRESULT CShader::Set_ConstTable(D3DXHANDLE hHandle, const void* pData, _uint iSize)
{
    if (nullptr == m_pEffect)
    {
        return E_FAIL;
    }

    if (FAILED(m_pEffect->SetValue(hHandle, pData, iSize)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CShader::Set_Texture(D3DXHANDLE hHandle, CTexture* pTexture, _uint iTextureIndex)
{
    if (nullptr == m_pEffect)
    {
        return E_FAIL;
    }

    LPDIRECT3DBASETEXTURE9 pBaseTexture = pTexture->Get_Texture(iTextureIndex);

    if (nullptr == pTexture)
    {
        return E_FAIL;
    }

    if (FAILED(m_pEffect->SetTexture(hHandle, pBaseTexture)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CShader::Set_Texture(D3DXHANDLE hHandle, LPDIRECT3DBASETEXTURE9 pTexture)
{
    return m_pEffect->SetTexture(hHandle, pTexture);
}

HRESULT CShader::Set_Bool(D3DXHANDLE hHandle, BOOL ToF)
{
    if (nullptr == m_pEffect)
    {
        return E_FAIL;
    }

    m_pEffect->SetBool(hHandle, ToF);

    return S_OK;
}

HRESULT CShader::Begin_Shader(_uint iPassIndex)
{
    if (nullptr == m_pEffect)
    {
        return E_FAIL;
    }

    m_pEffect->Begin(nullptr, 0);
    m_pEffect->BeginPass(iPassIndex);

    return S_OK;
}

HRESULT CShader::End_Shader()
{
    if (nullptr == m_pEffect)
    {
        return E_FAIL;
    }

    m_pEffect->EndPass();
    m_pEffect->End();

    return S_OK;
}

void CShader::Commit()
{
    m_pEffect->CommitChanges();
}

CShader* CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, wstring pShaderFilePath)
{
    CShader* pInstance = new CShader(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath)))
    {
        MSGBOX("Failed to Create Shader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
    CShader* pInstance = new CShader(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Shader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CShader::Free()
{
    __super::Free();

    Safe_Release(m_pEffect);
}
