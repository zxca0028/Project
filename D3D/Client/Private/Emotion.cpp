#include "pch.h"
#include "Emotion.h"
#include "GameInstance.h"

CEmotion::CEmotion(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CEmotion::CEmotion(const CEmotion& rhs)
    : CGameObject(rhs)
{}

HRESULT CEmotion::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CEmotion::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    m_pTransform->Set_Scale(_float3(0.1f, 0.1f, 0.1f));

    m_bCreate = true;

    return S_OK;
}

_int CEmotion::Tick(_double TimeDelta)
{
    __super::Tick(TimeDelta);

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _float3 vPlayerPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

    vPlayerPos.y += 3.5f;

    m_pTransform->Set_State(CTransform::STATE_POSITION, vPlayerPos);

    RELEASE_INSTANCE(CGameInstance);

    if (true == m_bGIF)
    {
        m_fFrame += 14.f * TimeDelta;

        if (m_fFrame >= 14)
        {
            m_fFrame = 0.f;
        }
    }

    return _int();
}

_int CEmotion::Late_Tick(_double TimeDelta)
{
    __super::Late_Tick(TimeDelta);

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);

    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

    m_pTransform->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
    m_pTransform->Set_State(CTransform::STATE_UP,    *(_float3*)&ViewMatrix.m[1][0]);
    m_pTransform->Set_State(CTransform::STATE_LOOK,  *(_float3*)&ViewMatrix.m[2][0]);

    if (true == m_bCreate)
    {
        if (m_fTime > 2.4f)
        {
            m_bCreate = false;
            m_fTime = 0.f;
        }

        m_fTime += TimeDelta;
        m_fScale = m_fTime * 15 > 1.6f ? 1.6f : m_fTime * 15;

        m_pTransform->Set_Scale(_float3(m_fScale, m_fScale, m_fScale));
    }
    else
    {
        m_fTime += TimeDelta;
        m_fScale = m_fTime * 15 > 1.6f ? 1.6f : m_fTime * 15;

        m_pTransform->Set_Scale(_float3(1.6f - m_fScale, 1.6f - m_fScale, 1.6f - m_fScale));
    }

    RELEASE_INSTANCE(CGameInstance);

    if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
    {
        return -1;
    }

    return _int();
}

HRESULT CEmotion::Render()
{
    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CEmotion::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix  = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix PorjMatrix  = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix,  sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &PorjMatrix,  sizeof(_matrix));

    if (false == m_bGIF)
    {
        m_pShader->Set_TextureConstTable("g_tMtrlDiffuse", m_pTexture, 0);
    }
    else
    {
        m_pShader->Set_TextureConstTable("g_tMtrlDiffuse", m_pTexture, (_uint)m_fFrame);
    }

    m_pShader->Begin_Shader(0);
    m_pVIBuffer->Render_VIBuffer();
    m_pShader->End_Shader();

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CEmotion::Add_Components(void* pArg)
{
    CTransform::TRANSFORMDESC TransformDesc;
    TransformDesc.fSpeedPerSec    = 0.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(0.f);

    if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Transform"),     TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_UI"),     TEXT("Com_Shader"),    (CComponent**)&m_pShader)))   { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"),      TEXT("Com_Renderer"),  (CComponent**)&m_pRenderer))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"),  (CComponent**)&m_pVIBuffer))) { return E_FAIL; }

    int i = 0;
    memcpy(&i, pArg, sizeof(int));

    if (i == 0)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_³îÀÚ¿¡¿ä"), TEXT("Com_Texture"), (CComponent**)&m_pTexture))) { return E_FAIL; }
    }
    else if (i == 1)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_¸Ó¾³ÇØ¿ä"), TEXT("Com_Texture"), (CComponent**)&m_pTexture))) { return E_FAIL; }
    }
    else if (i == 2)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_¿ôÇÁ³×¿ä"), TEXT("Com_Texture"), (CComponent**)&m_pTexture))) { return E_FAIL; }
    }
    else if (i == 3)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_ÃßÃµÀÌ¿ä"), TEXT("Com_Texture"), (CComponent**)&m_pTexture))) { return E_FAIL; }
    }
    else if (i == 4)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_¸ô?·ç"), TEXT("Com_Texture"), (CComponent**)&m_pTexture))) { return E_FAIL; }
        m_bGIF = true;
    }
	

	return S_OK;
}

CEmotion* CEmotion::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEmotion* pInstance = new CEmotion(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating Emotion");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEmotion::Clone(void* pArg)
{
    CEmotion* pInstance = new CEmotion(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating Emotion");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEmotion::Free()
{
    CGameObject::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pTexture);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}
