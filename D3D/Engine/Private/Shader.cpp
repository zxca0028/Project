#include "Shader.h"
#include "Texture.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CComponent(pGraphic_Device)
{}

CShader::CShader(const CShader& rhs)
    :CComponent(rhs)
    , m_pEffect(rhs.m_pEffect)
{
    Safe_AddRef(m_pEffect);
}

HRESULT CShader::NativeConstruct_Prototype(const _tchar* pShaderPilePath)
{
    LPD3DXBUFFER pBuffer = nullptr;

    if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, pShaderPilePath, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &pBuffer)))
    {
        return E_FAIL;
    }

    Safe_Release(pBuffer);

    return S_OK;
}

HRESULT CShader::NativeConstruct(void* pArg)
{
    return S_OK;
}

HRESULT CShader::Set_ConstTable(D3DXHANDLE hHnd, const void* pData, _uint iLengthByte)
{
    if (nullptr == m_pEffect)
    {
        return E_FAIL;
    }

    if (FAILED(m_pEffect->SetValue(hHnd, pData, iLengthByte)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CShader::Set_Bool(D3DXHANDLE hHandle, BOOL isBool)
{
    if (nullptr == m_pEffect)
    {
        return E_FAIL;
    }

    m_pEffect->SetBool(hHandle, isBool);

    return S_OK;
}

HRESULT CShader::Set_TextureConstTable(D3DXHANDLE hHnd, CTexture* pTextureCom, _uint iTextureIndex)
{
    if (nullptr == m_pEffect)
    {
        return E_FAIL;
    }

    LPDIRECT3DBASETEXTURE9 pTexture = pTextureCom->Get_Texture(iTextureIndex);

    if (nullptr == pTexture)
    {
        return E_FAIL;
    }

    return m_pEffect->SetTexture(hHnd, pTexture);
}

HRESULT CShader::Set_TextureConstTable(D3DXHANDLE hHnd, LPDIRECT3DBASETEXTURE9 pTexture)
{
    return m_pEffect->SetTexture(hHnd, pTexture);
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

CShader* CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderPilePath)
{
    CShader* pInstance = new CShader(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype(pShaderPilePath)))
    {
        MSGBOX("Failed to Creating Shader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
    AddRef();

    return this;
}

void CShader::Free()
{
    __super::Free();

    Safe_Release(m_pEffect);
}
