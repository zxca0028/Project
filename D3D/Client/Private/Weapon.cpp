#include "pch.h"
#include "Weapon.h"
#include "GameInstance.h"
#include "Collision_Manager.h"
#include "Trail.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{}

CWeapon::CWeapon(const CWeapon& rhs)
    : CGameObject(rhs)
{}

HRESULT CWeapon::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CWeapon::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    if (FAILED(Add_Components(pArg)))
    {
        return E_FAIL;
    }

    memcpy(&m_iIndex, pArg, sizeof(_uint));

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    D3DXMatrixIdentity(&m_OriginMatrix);

    CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
    m_pParentWorldMatrix = pPlayerTransform->Get_WorldMatrix();

    const _matrix* pWorldMatrix = m_pTransform->Get_WorldMatrix();

    if (m_iIndex == 0)
    {
        CMesh_Dynamic* pPlayerMesh = (CMesh_Dynamic*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Mesh"));
        m_pParentBoneMatrix = pPlayerMesh->Get_BoneMatrix("BN_Weapon_R");

        CTrail::TRAILINFO TrailInfo;

        TrailInfo.pWorldMatrix = m_pTransform->Get_WorldMatrix();
        TrailInfo.iIndex = 0;

        CTrail* pTrail = CTrail::Create(m_pGraphic_Device, &TrailInfo);

        if (nullptr == pTrail)
        {
            return E_FAIL;
        }

        if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Trail"), pTrail)))
        {
            return E_FAIL;
        }
    }
    else
    {
        CMesh_Dynamic* pPlayerMesh = (CMesh_Dynamic*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Mesh"));
        m_pParentBoneMatrix = pPlayerMesh->Get_BoneMatrix("BN_Weapon_L");

        CTrail::TRAILINFO TrailInfo;

        TrailInfo.pWorldMatrix = m_pTransform->Get_WorldMatrix();
        TrailInfo.iIndex = 1;

        CTrail* pTrail = CTrail::Create(m_pGraphic_Device, &TrailInfo);

        if (nullptr == pTrail)
        {
            return E_FAIL;
        }

        if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Trail"), pTrail)))
        {
            return E_FAIL;
        }
    }

    RELEASE_INSTANCE(CGameInstance);
    
    return S_OK;
}

_int CWeapon::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    return _int();
}

_int CWeapon::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    _matrix WorldMatrix = m_OriginMatrix * (*m_pParentBoneMatrix * *m_pParentWorldMatrix);

    m_pTransform->Set_State(CTransform::STATE_RIGHT,    *(_float3*)&WorldMatrix.m[0][0]);
    m_pTransform->Set_State(CTransform::STATE_UP,       *(_float3*)&WorldMatrix.m[1][0]);
    m_pTransform->Set_State(CTransform::STATE_LOOK,     *(_float3*)&WorldMatrix.m[2][0]);
    m_pTransform->Set_State(CTransform::STATE_POSITION, *(_float3*)&WorldMatrix.m[3][0]);

    _float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
    _float3 vUp = m_pTransform->Get_State(CTransform::STATE_UP);

    D3DXVec3Normalize(&vUp, &vUp);

    _float3 vDownPos = vPos + vUp * 0.6f;
    _float3 vUpPos   = vPos + vUp * 0.1f;

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CTrail* pTrail = (CTrail*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Trail"), m_iIndex);

    pTrail->AddNewTrail(vDownPos, vUpPos, DeltaTime);
    pTrail->UpdateTrail(DeltaTime);

    RELEASE_INSTANCE(CGameInstance);

    if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
    {
        return E_FAIL;
    }

    return _int();
}

HRESULT CWeapon::Render()
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

    m_pShader->Begin_Shader(1);

    _uint	iNumMaterials = m_pMesh->Get_NumMaterials();

    for (_uint i = 0; i < iNumMaterials; ++i)
    {
        if (FAILED(m_pMesh->Set_TextureOnShader(m_pShader, "g_tMtrlDiffuse", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i)))
        {
            return E_FAIL;
        }
		if (FAILED(m_pMesh->Set_TextureOnShader(m_pShader, "g_NormalDiffuse", MESHMATERIALTEXTURE::TYPE_NORMAL, i)))
        {
            return E_FAIL;
        }

        m_pShader->Commit();

        if (FAILED(m_pMesh->Render(i)))
        {
            return E_FAIL;
        }
        
    }

    m_pShader->End_Shader();

    RELEASE_INSTANCE(CGameInstance);
    return S_OK;
}

HRESULT CWeapon::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _matrix WrldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrix = pGameInstance->Get_Transform(D3DTS_PROJECTION);

    m_pShader->Set_ConstTable("g_WorldMatrix", &WrldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix, sizeof(_matrix));

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CWeapon::Add_Components(void* pArg)
{

	/* Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Transform"),     TEXT("Com_Transform"),     (CComponent**)&m_pTransform, &TransformDesc))) { return E_FAIL; }
    if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_Mesh"),   TEXT("Com_Shader_Mesh"),   (CComponent**)&m_pShader)))       { return E_FAIL; }
    if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Shader_Effect"), TEXT("Com_Shader_Effect"), (CComponent**)&m_pEffectShader))) { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_STATIC,   TEXT("Prototype_Renderer"),      TEXT("Com_Renderer"),      (CComponent**)&m_pRenderer)))     { return E_FAIL; }
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Mesh_Weapon"),   TEXT("Com_Mesh"),          (CComponent**)&m_pMesh)))         { return E_FAIL; }

	return S_OK;
}

CWeapon* CWeapon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CWeapon* pInstance = new CWeapon(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating Arrow");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CWeapon::Clone(void* pArg)
{
    CWeapon* pInstance = new CWeapon(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating Arrow");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CWeapon::Free()
{
    __super::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pEffectShader);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pTransform);
    Safe_Release(m_pMesh);
}
