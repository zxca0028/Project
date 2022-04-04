#include "pch.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Component.h"

BEGIN(Client)

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
}

HRESULT CRenderer::NativeConstruct()
{
    return S_OK;
}

HRESULT CRenderer::Render()
{
    Render_BackGround();

    Render_Object();
    
    Render_Player();

    Render_UI();

    Render_Score();

#ifdef _DEBUG

    Render_Collider();

#endif

    return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERGROUP eGroup, CGameObject* pGameObject)
{
    if (eGroup >= RENDER_END || nullptr == pGameObject)
    {
        return E_FAIL;
    }

    m_RenderObjects[eGroup].emplace_back(pGameObject);

    Safe_AddRef(pGameObject);

    return S_OK;
}

HRESULT CRenderer::Render_BackGround()
{
    for (auto& pGameObject : m_RenderObjects[RENDER_BACKGROUND])
    {
        if (FAILED(pGameObject->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[RENDER_BACKGROUND].clear();

    return S_OK;
}

HRESULT CRenderer::Render_Object()
{
    for (auto& pGameObject : m_RenderObjects[RENDER_OBJECT])
    {
        if (FAILED(pGameObject->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[RENDER_OBJECT].clear();

    return S_OK;
}

HRESULT CRenderer::Render_Player()
{
    for (auto& pGameObject : m_RenderObjects[RENDER_PLAYER])
    {
        if (FAILED(pGameObject->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[RENDER_PLAYER].clear();

    return S_OK;
}

HRESULT CRenderer::Render_UI()
{
    for (auto& pGameObject : m_RenderObjects[RENDER_UI])
    {
        if (FAILED(pGameObject->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[RENDER_UI].clear();

    return S_OK;
}

HRESULT CRenderer::Render_Score()
{
    for (auto& pGameObject : m_RenderObjects[RENDER_SCORE])
    {
        if (FAILED(pGameObject->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_RenderObjects[RENDER_SCORE].clear();

    return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Add_RenderComponent(CComponent* pComponent)
{
    if (nullptr == pComponent)
    {
        return E_FAIL;
    }

    m_RenderComponent.emplace_back(pComponent);

    Safe_AddRef(pComponent);

    return S_OK;
}

HRESULT CRenderer::Render_Collider()
{
    for (auto& pComponent : m_RenderComponent)
    {
        if (FAILED(pComponent->Render()))
        {
            return E_FAIL;
        }

        Safe_Release(pComponent);
    }

    m_RenderComponent.clear();

    return S_OK;
}

#endif

void CRenderer::Free()
{
    for (_uint i = 0; i < RENDER_END; ++i)
    {
        for (auto& pGameObject : m_RenderObjects[i])
        {
            Safe_Release(pGameObject);
        }

        m_RenderObjects[i].clear();
    }

#ifdef _DEBUG

    for (auto& pComponent: m_RenderComponent)
    {
        Safe_Release(pComponent);
    }

    m_RenderComponent.clear();

#endif
}

END