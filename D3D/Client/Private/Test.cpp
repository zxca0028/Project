#include "pch.h"
#include "Test.h"
#include "GameInstance.h"
#include "KeyManager.h"
#include "ObjectTool.h"

CTest::CTest(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{}

CTest::CTest(const CTest& rhs)
    : CGameObject(rhs)
{}

HRESULT CTest::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CTest::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    m_pTransform->Set_Scale(_float3(0.05f, 0.05f, 0.05f));
    m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(64.f, 0.f, 64.f));

    //m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(52.6f, 0.11f, 50.67f));
    //m_pTransform->Set_State(CTransform::STATE_RIGHT, _float3(0.0201395f, 0.f, 0.0248677f));
    //m_pTransform->Set_State(CTransform::STATE_UP, _float3(0.f, 0.032f, 0.f));
    //m_pTransform->Set_State(CTransform::STATE_LOOK, _float3(-0.0248677f, 0.f, 0.0201395f));

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

_int CTest::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    //ImGui_Test();
    //Controller(DeltaTime);


    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CTest::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
    {
        return E_FAIL;
    }

    return _int();
}

HRESULT CTest::Render()
{
    __super::Render();

    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    __super::Render();

    if (FAILED(Set_ConstTable()))
    {
        return E_FAIL;
    }

    m_pShaderCom->Begin_Shader(0);

    _uint	iNumMaterials = m_pMeshCom->Get_NumMaterials();

    for (_uint i = 0; i < iNumMaterials; ++i)
    {
        if (FAILED(m_pMeshCom->Set_TextureOnShader(m_pShaderCom, "g_tMtrlDiffuse", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i)))
            return E_FAIL;

        m_pShaderCom->Commit();

        if (FAILED(m_pMeshCom->Render(i)))
        {
            return E_FAIL;
        }
    }

    m_pShaderCom->End_Shader();


    RELEASE_INSTANCE(CGameInstance);
    return S_OK;
}

HRESULT CTest::Set_ConstTable()
{
    if (nullptr == m_pShaderCom)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix WrldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShaderCom->Set_ConstTable("g_WorldMatrix", &WrldMatrix, sizeof(_matrix));
    m_pShaderCom->Set_ConstTable("g_ViewMatrix",  &ViewMatrix, sizeof(_matrix));
    m_pShaderCom->Set_ConstTable("g_ProjMatrix",  &ProjMatrix, sizeof(_matrix));

    D3DLIGHT9 LightDesc = pGameInstance->Get_LightDesc(0);
    _float4   LightDir = _float4(LightDesc.Direction, 0.f);

    m_pShaderCom->Set_ConstTable("g_vLightDir",      &LightDir,           sizeof(_float4));
    m_pShaderCom->Set_ConstTable("g_vLightDiffuse",  &LightDesc.Diffuse,  sizeof(_float4));
    m_pShaderCom->Set_ConstTable("g_vLightSpecular", &LightDesc.Specular, sizeof(_float4));
    m_pShaderCom->Set_ConstTable("g_vLightAmbient",  &LightDesc.Ambient,  sizeof(_float4));

    _float4 vCamPos = _float4(pGameInstance->Get_CamPosition(), 1.f);

    m_pShaderCom->Set_ConstTable("g_vCamPos", &vCamPos, sizeof(_float4));

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CTest::Add_Components(void* pArg)
{
    /* Com_Transform */
    CTransform::TRANSFORMDESC		TransformDesc;
    TransformDesc.fSpeedPerSec    = 5.0f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Transform"),   TEXT("Com_Transform"),   (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
    if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader_Mesh"), (CComponent**)&m_pShaderCom)))   { return E_FAIL; }
    if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"),    TEXT("Com_Renderer"),    (CComponent**)&m_pRendererCom))) { return E_FAIL; }

    int iIndex;
    memcpy(&iIndex, pArg, sizeof(int));
    
    _tchar szPrototypeTag[MAX_PATH] = TEXT("Prototype_Mesh_Hieracon");
    _tchar szKeyTag      [MAX_PATH] = TEXT("");
    
    _itow_s(iIndex, szKeyTag, 10);
    
    lstrcat(szPrototypeTag, szKeyTag);

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, szPrototypeTag, TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
    
    return S_OK;
}

void CTest::ImGui_Test()
{
    //_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

    //ImGui::Begin("Test");
    //if (ImGui::Button("++State"))
    //{
    //    ++m_iAniState;
    //}
    //ImGui::SameLine();
    //if (ImGui::Button("--State"))
    //{
    //    if (m_iAniState > 0)
    //    {
    //        --m_iAniState;
    //    }
    //}
    //ImGui::Text("AnimationState = %d ", m_iAniState);
    //ImGui::End();
}

void CTest::Controller(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    CKeyManager*   pKey          = GET_INSTANCE(CKeyManager);

    if (GetAsyncKeyState('Z') & 0x8000)
    {
        m_pTransform->Rotation_Axis(_float3(0.f, 1.f, 0.f), DeltaTime);
    }
    if (GetAsyncKeyState('C') & 0x8000)
    {
        m_pTransform->Rotation_Axis(_float3(0.f, 1.f, 0.f), -DeltaTime);
    }

    if (pKey->Key_Pressing(KEY_UP))
    {
        if (pKey->Key_Pressing(KEY_LEFT))
        {
            m_pTransform->Move_Left(DeltaTime * 0.707);
            m_pTransform->Move_Front(DeltaTime * 0.707);
        }
        else if (pKey->Key_Pressing(KEY_RIGHT))
        {
            m_pTransform->Move_Right(DeltaTime * 0.707);
            m_pTransform->Move_Front(DeltaTime * 0.707);
        }
        else
        {
            m_pTransform->Move_Front(DeltaTime);
        }
    }
    else if (pKey->Key_Pressing(KEY_DOWN))
    {
        if (pKey->Key_Pressing(KEY_LEFT))
        {
            m_pTransform->Move_Left(DeltaTime / 2.f * 0.707);
            m_pTransform->Move_Back(DeltaTime / 2.f);
        }
        else if (pKey->Key_Pressing(KEY_RIGHT))
        {
            m_pTransform->Move_Right(DeltaTime / 2.f * 0.707);
            m_pTransform->Move_Back(DeltaTime / 2.f);
        }
        else
        {
            m_pTransform->Move_Back(DeltaTime / 2.f);
        }
    }
    else if (pKey->Key_Pressing(KEY_LEFT))
    {
        m_pTransform->Move_Left(DeltaTime);
    }
    else if (pKey->Key_Pressing(KEY_RIGHT))
    {
        m_pTransform->Move_Right(DeltaTime);
    }

    RELEASE_INSTANCE(CKeyManager);
    RELEASE_INSTANCE(CGameInstance);
}

CTest* CTest::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTest* pInstance = new CTest(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating Arrow");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CTest::Clone(void* pArg)
{
    CTest* pInstance = new CTest(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating Arrow");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CTest::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTransform);
    Safe_Release(m_pMeshCom);
}
