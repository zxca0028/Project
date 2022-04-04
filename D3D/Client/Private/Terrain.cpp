#include "pch.h"
#include "Terrain.h"
#include "GameInstance.h"
#include "KeyManager.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CTerrain::CTerrain(const CTerrain& rhs)
    :CGameObject(rhs)
{}

HRESULT CTerrain::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CTerrain::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    m_pTransform->Rotation_Axis_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-45.f));
    m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(128.f, -27.f, -226.f));

    return S_OK;
}

_int CTerrain::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    return _int();
}

_int CTerrain::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

    return _int();
}

HRESULT CTerrain::Render()
{
    __super::Render();

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix World = *(m_pTransform->Get_WorldMatrix());
    _matrix View  = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix Proj  = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_WorldMatrix", &World, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &View,  sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &Proj,  sizeof(_matrix));

    m_pShader->Set_TextureConstTable("g_tMtrlDiffuse", m_pTexture, 0);
    m_pShader->Set_TextureConstTable("g_NormalDiffuse", m_pTexture_N, 0);

    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CTerrain::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Frustum"),          TEXT("Com_Frustum"),          (CComponent**)&m_pFrustum)))   { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"),         TEXT("Com_Renderer"),         (CComponent**)&m_pRenderer)))  { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Transform"),        TEXT("Com_Transform"),        (CComponent**)&m_pTransform))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_Terrain"),   TEXT("Com_Shader_Terrain"),   (CComponent**)&m_pShader)))    { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_VIBuffer_Terrain"), TEXT("Com_VIBuffer_Terrain"), (CComponent**)&m_pVIBuffer)))  { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Terrain"),  TEXT("Com_Texture_Terrain"),  (CComponent**)&m_pTexture)))   { return E_FAIL; }
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Terrain_N"), TEXT("Com_Texture_Terrain_N"), (CComponent**)&m_pTexture_N))) { return E_FAIL; }

    return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTerrain* pInstance = new CTerrain(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating CTerrain");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
    CTerrain* pInstance = new CTerrain(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating CTerrain");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CTerrain::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pTexture_N);
    Safe_Release(m_pFrustum);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}
