#include "pch.h"
#include "Structure.h"
#include "GameInstance.h"
#include "KeyManager.h"
#include "ObjectTool.h"

CStructure::CStructure(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{}

CStructure::CStructure(const CStructure& rhs)
    : CGameObject(rhs)
{}

HRESULT CStructure::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CStructure::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CStructure::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    return _int();
}

_int CStructure::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
    {
        return E_FAIL;
    }

    return _int();
}

HRESULT CStructure::Render()
{
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
        if (FAILED(m_pMeshCom->Set_TextureOnShader(m_pShaderCom, "g_NormalDiffuse", MESHMATERIALTEXTURE::TYPE_NORMAL, i)))
        {
            return E_FAIL;
        }

        m_pShaderCom->Commit();

        if (FAILED(m_pMeshCom->Render(i)))
        {
            return E_FAIL;
        }
    }

    m_pShaderCom->End_Shader();

    return S_OK;
}

HRESULT CStructure::Set_ConstTable()
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

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CStructure::Add_Components(void* pArg)
{
    CTransform::TRANSFORMDESC		TransformDesc;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"),   TEXT("Com_Transform"),   (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader_Mesh"), (CComponent**)&m_pShaderCom)))   { return E_FAIL; }
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"),    TEXT("Com_Renderer"),    (CComponent**)&m_pRendererCom))) { return E_FAIL; }

    _int iStructureIndex = 0;
    memcpy(&iStructureIndex, pArg, sizeof(_int));

    if (iStructureIndex == 1)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon17"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(-15.f, 0.5f, -10.f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0283072f, 0.f, -0.0282613f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.04f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0282613f, 0.f, 0.0283072f));
    }
    else if (iStructureIndex == 2)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon18"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(11.14f, 0.49f, 6.5f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(-0.0283566f, 0.f, 0.0282117f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.04f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0282117f, 0.f, -0.0283566f));
    }
    else if (iStructureIndex == 3)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon18"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(37.6f, 0.5f, 66.41f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0282867f, 0.f, -0.0282819f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.04f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0282819f, 0.f, 0.0282867f));
    }
    else if (iStructureIndex == 4)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon16"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(58.22f, 9.89f, 87.07f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0283097f, 0.f, -0.0282589f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.04f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0282589f, 0.f, 0.0283097f));
    } 
    else if (iStructureIndex == 5)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon216"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(67.73f, 222.f, 90.92f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0352518f, 0.f, -0.0354586f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0354586f, 0.f, 0.0352518f));
    }
    else if (iStructureIndex == 6)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon141"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(65.6f, -29.73f, 87.68f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(-0.0141746f, 0.f, -0.0141095f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.02f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0141095f, 0.f, -0.0141746f));
    }
    else if (iStructureIndex == 7)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon187"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(63.12f, 9.1f, 97.27f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353517f, 0.f, -0.035359f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.035359f, 0.f, 0.0353517f));
    }
    else if (iStructureIndex == 8)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon187"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(52.9f, 9.1f, 107.49f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353558f, 0.f, -0.0353549f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0353549f, 0.f, 0.0353558f));
    }
    else if (iStructureIndex == 9)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon136"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(54.93f, 9.42f, 91.47f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353576f, 0.f, 0.0353531f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0353531f, 0.f, 0.0353576f));
    }
    else if (iStructureIndex == 10)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon141"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(86.95f, -29.73f, 66.29f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0141409f, 0.f, 0.0141433f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.02f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0141433f, 0.f, 0.0141409f));
    }
    else if (iStructureIndex == 11)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon53"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(58.26f, 9.27f, 87.53f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(-0.0106012f, 0.f, -0.0280645f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.03f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0280645f, 0.f, -0.0106012f));
    }
    else if (iStructureIndex == 12)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon53"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(63.51f, 9.27f, 82.45f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0282795f, 0.f, -0.0100136f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.03f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0100136f, 0.f, 0.0282795f));
    }
    else if (iStructureIndex == 13)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon187"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(73.3f, 9.1f, 87.03f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353567f, 0.f, -0.035354f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.035354f, 0.f, 0.0353567f));
    }
    else if (iStructureIndex == 14)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon187"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(83.55f, 9.1f, 76.78f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353357f, 0.f, -0.035375f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.035375f, 0.f, 0.0353357f));
    }
    else if (iStructureIndex == 15)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon187"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(93.77f, 9.1f, 66.52f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353678f, 0.f, -0.0353428f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0353428f, 0.f, 0.0353678f));
    }
    else if (iStructureIndex == 16)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon134"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(93.13f, 9.2f, 60.1f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(-0.0283115f, 0.f, -0.0282571f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.04f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.0282571f, 0.f, -0.0283115f));
    }
    else if (iStructureIndex == 17)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon136"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(67.5f, 9.42f, 78.87f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353241f, 0.f, 0.0353865f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0353865f, 0.f, 0.0353241f));
    }
    else if (iStructureIndex == 18)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon136"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(74.44f, 9.42f, 71.93f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.035355f, 0.f, 0.0353556f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0353556f, 0.f, 0.035355f));
    }
    else if (iStructureIndex == 19)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon136"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(81.42f, 9.42f, 64.95f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353818f, 0.f, 0.0353288f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0353288f, 0.f, 0.0353818f));
    }
    else if (iStructureIndex == 20)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon136"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(88.39f, 9.42f, 57.98f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353431f, 0.f, 0.0353676f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0353676f, 0.f, 0.0353431f));
    }
    else if (iStructureIndex == 21)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon136"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(95.36f, 9.42f, 51.01f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353486f, 0.f, 0.035362f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.035362f, 0.f, 0.0353486f));
    }
    else if (iStructureIndex == 22)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon136"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(47.97f, 9.42f, 98.43f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0353558f, 0.f, 0.0353549f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.05f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0353549f, 0.f, 0.0353558f));
    }
    else if (iStructureIndex == 23)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon56"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(-105.37f, 105.f, 281.44f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(-0.0110371f, 0.f, 0.0278959f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.03f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0278959f, 0.f, -0.0110371f));
    }
    else if (iStructureIndex == 24)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon142"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(64.f, 2.96f, 64.f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.f, 0.f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(0.f, 0.f, 0.f));
    }
    else if (iStructureIndex == 25)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon142"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(59.36f, -29.7f, 93.99f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.014143f, 0.f, 0.0141412f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.02f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0141412f, 0.f, 0.014143f));
    }
    else if (iStructureIndex == 26)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon54"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(8.79f, 14.f, 144.3f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT,    _float3(0.0141225f, 0.f, 0.0141618f));
        m_pTransform->Set_State(CTransform::STATE_UP,       _float3(0.f, 0.02f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK,     _float3(-0.0141618f, 0.f, 0.0141225f));
    }
    else if (iStructureIndex == 27)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon54"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(28.13f, 14.01f, 124.98f));
        m_pTransform->Set_State(CTransform::STATE_RIGHT, _float3(-0.0141157f, 0.f, -0.0141685f));
        m_pTransform->Set_State(CTransform::STATE_UP, _float3(0.f, 0.02f, 0.f));
        m_pTransform->Set_State(CTransform::STATE_LOOK, _float3(0.0141685f, 0.f, -0.0141157f));
    }
    else if (iStructureIndex == 28)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Hieracon73"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom))) { return E_FAIL; }
        {
            m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(61.1f, 9.5f, 84.8f));
            m_pTransform->Set_Scale(_float3(0.1f, 0.1f, 0.1f));
            m_pTransform->Rotation_Axis_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(45.f));
        }
    }

    return S_OK;
}

CStructure* CStructure::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CStructure* pInstance = new CStructure(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating Structure");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CStructure::Clone(void* pArg)
{
    CStructure* pInstance = new CStructure(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating Structure");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CStructure::Free()
{
    __super::Free();

    Safe_Release(m_pMeshCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTransform);
    Safe_Release(m_pRendererCom);
}
