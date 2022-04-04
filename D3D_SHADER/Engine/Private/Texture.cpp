#include "Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent(pGraphic_Device)
{
}

CTexture::CTexture(const CTexture& rhs)
    : CComponent(rhs)
    , m_Textures(rhs.m_Textures)
{
    for (auto& pTexture : m_Textures)
    {
        Safe_AddRef(pTexture);
    }
}

HRESULT CTexture::NativeConstruct_Prototype(TYPE eType, wstring pTextureFilePath, _uint iNumTexture)
{
    m_Textures.reserve(iNumTexture);

    _tchar  szFilePath[MAX_PATH] = TEXT("");

    for (_uint i = 0; i < iNumTexture; ++i)
    {
        IDirect3DBaseTexture9* pTexture = nullptr;

        wsprintf(szFilePath, (LPCWSTR)pTextureFilePath.c_str(), i);

        if (eType == TYPE::TYPE_2D)
        {
            if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFilePath, (LPDIRECT3DTEXTURE9*)&pTexture)))
            {
                return E_FAIL;
            }
        }
        else if (eType == TYPE::TYPE_UI)
        {
            if (FAILED(D3DXCreateTextureFromFileEx(m_pGraphic_Device, szFilePath, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, 0, 0, (LPDIRECT3DTEXTURE9*)&pTexture)))
            {
                return E_FAIL;
            }
        }
        else if (eType == TYPE::TYPE_CUBE)
        {
            if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFilePath, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
            {
                return E_FAIL;
            }
        }

        m_Textures.emplace_back(pTexture);
    }

    return S_OK;
}

HRESULT CTexture::NativeConstruct(void* pArg)
{
    return S_OK;
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, wstring pTextureFilePath, _uint iNumTexture)
{
    CTexture* pInstance = new CTexture(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype(eType, pTextureFilePath, iNumTexture)))
    {
        MSGBOX("Failed to Create Texture");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
    CTexture* pInstance = new CTexture(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Create Texture");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTexture::Free()
{
    __super::Free();

    for (auto& pTexture : m_Textures)
    {
        Safe_Release(pTexture);
    }

    m_Textures.clear();
}
