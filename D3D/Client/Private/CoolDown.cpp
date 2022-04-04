#include "pch.h"
#include "CoolDown.h"
#include "GameInstance.h"
#include "Collision_Manager.h"
#include "KeyManager.h"
#include "Skill_Manager.h"

CCoolDown::CCoolDown(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{}

CCoolDown::CCoolDown(const CCoolDown& rhs)
    : CGameObject(rhs)
{}

HRESULT CCoolDown::NativeConstruct_Prototype()
{
    __super::NativeConstruct_Prototype();

    return S_OK;
}

HRESULT CCoolDown::NativeConstruct(void* pArg)
{
    __super::NativeConstruct(pArg);

    memcpy(&m_iIndex, pArg, sizeof(_uint));

    if (FAILED(Add_Components()))
    {
        return E_FAIL;
    }

    m_fSizeX = 44;
    m_fSizeY = 44;

    if (m_iIndex == 0)
    {
        m_fX = (g_iWinCX / 2) - 248;
        m_fY = (g_iWinCY / 2) + 271;
    }
    else if (m_iIndex == 1)
    {
        m_fX = (g_iWinCX / 2) - 199;
        m_fY = (g_iWinCY / 2) + 271;
    }
    else if (m_iIndex == 2)
    {
        m_fX = (g_iWinCX / 2) - 150;
        m_fY = (g_iWinCY / 2) + 271;
    }
    else if (m_iIndex == 3)
    {
        m_fX = (g_iWinCX / 2) - 101;
        m_fY = (g_iWinCY / 2) + 271;
    }
    else if (m_iIndex == 4)
    {
        m_fX = (g_iWinCX / 2) - 0 + 16;
        m_fY = (g_iWinCY / 2) + 180;
    }
    else if (m_iIndex == 5)
    {
        m_fX = (g_iWinCX / 2) - 0 + 16;
        m_fY = (g_iWinCY / 2) + 180;
    }
    else if (m_iIndex == 6)
    {
        m_fX = (g_iWinCX / 2) - 78;
        m_fY = (g_iWinCY / 2) + 317;
    }
    else if (m_iIndex == 7)
    {
        m_fX = (g_iWinCX / 2) - 136 + 9;
        m_fY = (g_iWinCY / 2) + 317;
    }
    
    D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

    D3DXMatrixIdentity(&m_WorldMatrix);
    D3DXMatrixIdentity(&m_ViewMatrix);

    return S_OK;
}

_int CCoolDown::Tick(_double DeltaTime)
{
    __super::Tick(DeltaTime);

    if (m_iIndex == 0)
    {
        if (true == CSkill_Manager::Get_Instance()->IsActive_Q())
        {
            CoolTime(6.f, DeltaTime);//6
        }
        else
        {
            m_iToF = 0;
        }
    }
    else if (m_iIndex == 1)
    {
        if (true == CSkill_Manager::Get_Instance()->IsActive_W())
        {
            CoolTime(8.f, DeltaTime);//8
        }
        else
        {
            m_iToF = 0;
        }
    }
    else if (m_iIndex == 2)
    {
        if (true == CSkill_Manager::Get_Instance()->IsActive_E())
        {
            CoolTime(7.f, DeltaTime);//7
        }
        else
        {
            m_iToF = 0;
        }
    }
    else if (m_iIndex == 3)
    {
        m_fX = (g_iWinCX / 2) - 101;
        m_fY = (g_iWinCY / 2) + 271;

        if (true == CSkill_Manager::Get_Instance()->IsActive_R())
        {
            CoolTime(14.f, DeltaTime);//14
        }
        else
        {
            m_iToF = 0;
        }
    }
    else if (m_iIndex == 5)
    {
        if (true == CSkill_Manager::Get_Instance()->IsActive_Space())
        {
            CoolTime(8.f, DeltaTime);//8
        }
        else
        {
            m_iToF = 0;
        }
    }
    else if (m_iIndex == 6)
    {
        if (true == CSkill_Manager::Get_Instance()->IsActive_F())
        {
            CoolTime(24.f, DeltaTime);//24
        }
        else
        {
            m_iToF = 0;
        }
    }
    else if (m_iIndex == 7)
    {
        if (true == CSkill_Manager::Get_Instance()->IsActive_D())
        {
            CoolTime(30.f, DeltaTime);//30
        }
        else
        {
            m_iToF = 0;
        }
    }

    return _int();
}

_int CCoolDown::Late_Tick(_double DeltaTime)
{
    __super::Late_Tick(DeltaTime);

    CCollision_Manager* pCollision_Manager = GET_INSTANCE(CCollision_Manager);

    if (false == pCollision_Manager->Release_Controller())
    {
        if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
        {
            return -1;
        }
    }

    RELEASE_INSTANCE(CCollision_Manager);

    return _int();
}

HRESULT CCoolDown::Render()
{
    if (m_iIndex < 4)
    {
        if (FAILED(Set_ConstTable()))
        {
            return E_FAIL;
        }
    }
	else if (m_iIndex < 6)
    {
        if (true == CSkill_Manager::Get_Instance()->IsActive_Space())
        {
            if (FAILED(Set_ConstTable()))
            {
                return E_FAIL;
            }
        }
    }
    else
    {
        if (FAILED(Set_ConstTable()))
        {
            return E_FAIL;
        }
    }
    
    return S_OK;
}

void CCoolDown::CoolTime(_float fCoolTime, _double DeltaTime)
{
	m_iToF = 1;

	m_fAngle += (360.f / fCoolTime) * DeltaTime;

    if (m_fAngle >= 360.f)
    {
        if (m_iIndex == 0)
        {
            CSkill_Manager::Get_Instance()->InActive_Q();
            m_fAngle = 0.f;
        }
        else if (m_iIndex == 1)
        {
            CSkill_Manager::Get_Instance()->InActive_W();
            m_fAngle = 0.f;
        }
        else if (m_iIndex == 2)
        {
            CSkill_Manager::Get_Instance()->InActive_E();
            m_fAngle = 0.f;
        }
        else if (m_iIndex == 3)
        {
            CSkill_Manager::Get_Instance()->InActive_R();
            m_fAngle = 0.f;
        }
        else if (m_iIndex == 5)
        {
            CSkill_Manager::Get_Instance()->InActive_Space();
            m_fAngle = 0.f;
        }
        else if (m_iIndex == 6)
        {
            CSkill_Manager::Get_Instance()->InActive_F();
            m_fAngle = 0.f;
        }
        else if (m_iIndex == 7)
        {
            CSkill_Manager::Get_Instance()->InActive_D();
            m_fAngle = 0.f;
        }
    }
}

HRESULT CCoolDown::Set_ConstTable()
{
    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    m_WorldMatrix._11 = m_fSizeX;
    m_WorldMatrix._22 = m_fSizeY;

    m_WorldMatrix._41 =  m_fX - (g_iWinCX / 2);
    m_WorldMatrix._42 = -m_fY + (g_iWinCY / 2);

    m_pShader->Set_ConstTable("g_WorldMatrix", &m_WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix", &m_ViewMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

    if (m_iIndex != 4)
    {
        // m_fAngle += (360.f / fCoolTime) * DeltaTime;
                
        _float3 vLook = _float3(0.f, 1.f, 0.f);

        _float3 vLook_Rotation = _float3
        (
            cosf(-D3DXToRadian(m_fAngle)) * vLook.x - sinf(-D3DXToRadian(m_fAngle)) * vLook.y,
            sinf(-D3DXToRadian(m_fAngle)) * vLook.x + cosf(-D3DXToRadian(m_fAngle)) * vLook.y,
            0.f
        );

        m_pShader->Set_ConstTable("g_vLook", &vLook_Rotation, sizeof(_float3));
        m_pShader->Set_ConstTable("g_ToF", &m_iToF, sizeof(_int));

        m_pShader->Begin_Shader(1);
        m_pVIBuffer->Render_VIBuffer();
        m_pShader->End_Shader();
    }
    else
    {
        m_pShader->Set_TextureConstTable("g_tMtrlDiffuse", m_pTexture);

        m_pShader->Begin_Shader(0);
        m_pVIBuffer->Render_VIBuffer();
        m_pShader->End_Shader();
    }
    

    return S_OK;
}

HRESULT CCoolDown::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShader)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
    {
        return E_FAIL;
    }
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
    {
        return E_FAIL;
    }

    if (m_iIndex == 4)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Texture_Avoid"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
        {
            return E_FAIL;
        }
    }


    return S_OK;
}

CCoolDown* CCoolDown::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCoolDown* pInstance = new CCoolDown(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCoolDown::Clone(void* pArg)
{
    CCoolDown* pInstance = new CCoolDown(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating UI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCoolDown::Free()
{
    CGameObject::Free();

    Safe_Release(m_pShader);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pVIBuffer);

    if (m_iIndex == 4)
    {
        Safe_Release(m_pTexture);
    }
}
