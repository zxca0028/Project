#include "Target_Manager.h"
#include "Target.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{
}

TARGETDESC CTarget_Manager::Get_TargetDesc(wstring pTargetTag)
{
    CTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

    if (nullptr == pRenderTarget)
    {
        return TARGETDESC();
    }

    return pRenderTarget->Get_TargetDesc();
}

HRESULT CTarget_Manager::Add_RenderTarget(LPDIRECT3DDEVICE9 pGraphic_Device, wstring pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor)
{
    if (nullptr != Find_RenderTarget(pTargetTag))
    {
        return E_FAIL;
    }

    CTarget* pRenderTarget = CTarget::Create(pGraphic_Device, iWidth, iHeight, eFormat, ClearColor);

    if (nullptr == pRenderTarget)
    {
        return E_FAIL;
    }

    m_RenderTargets.emplace(pTargetTag, pRenderTarget);

    return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(wstring pMRTTag, wstring pTargetTag)
{
    CTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

    if (nullptr == pRenderTarget)
    {
        return E_FAIL;
    }

    vector<CTarget*>* pMRTList = Find_MRT(pMRTTag);

    if (nullptr == pMRTList)
    {
        vector<CTarget*> MRTList;

        MRTList.emplace_back(pRenderTarget);

        m_MRTs.emplace(pMRTTag, MRTList);
    }
    else
    {
        pMRTList->emplace_back(pRenderTarget);
    }

    Safe_AddRef(pRenderTarget);

    return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(wstring pMRTTag)
{
    vector<CTarget*>* pMRTList = Find_MRT(pMRTTag);

    if (nullptr == pMRTList)
    {
        return E_FAIL;
    }

    for (auto& pTarget : *pMRTList)
    {
        pTarget->Clear();
    }

    _uint iIndex = 0;

    for (auto& pTarget : *pMRTList)
    {
        pTarget->Bind_RenderTarget(iIndex++);
    }

    return S_OK;
}

HRESULT CTarget_Manager::End_MRT(wstring pMRTTag)
{
    vector<CTarget*>* pMRTList = Find_MRT(pMRTTag);

    if (nullptr == pMRTList)
    {
        return E_FAIL;
    }

    _uint iIndex = 0;

    for (auto& pTarget : *pMRTList)
    {
        pTarget->Release_RenderTarget(iIndex++);
    }

    return S_OK;
}

CTarget* CTarget_Manager::Find_RenderTarget(wstring pTargetTag)
{
    auto iter = m_RenderTargets.find(pTargetTag);

    if (iter == m_RenderTargets.end())
    {
        return nullptr;
    }

    return iter->second;
}

vector<class CTarget*>* CTarget_Manager::Find_MRT(wstring pMRTTag)
{
    auto iter = m_MRTs.find(pMRTTag);

    if (iter == m_MRTs.end())
    {
        return nullptr;
    }

    return &iter->second;
}

HRESULT CTarget_Manager::Ready_Debug_Buffer(const _tchar* pTargetTag, _float fLeftX, _float fTopY, _float fSizeX, _float fSizeY)
{
    CTarget* pTarget = Find_RenderTarget(pTargetTag);

    if (nullptr == pTarget)
    {
        return E_FAIL;
    }

    return pTarget->Ready_Debug_Buffer(fLeftX, fTopY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_Debug_Buffer(const _tchar* pMRTTag)
{
    vector<CTarget*>* pMRTList = Find_MRT(pMRTTag);

    if (nullptr == pMRTList)
    {
        return E_FAIL;
    }

    for (auto& pTarget : *pMRTList)
    {
        pTarget->Render_Debug_Buffer();
    }

    return S_OK;
}

void CTarget_Manager::Free()
{
    for (auto& Pair : m_MRTs)
    {
        for (auto& pTarget : Pair.second)
        {
            Safe_Release(pTarget);
        }

        Pair.second.clear();
    }

    m_MRTs.clear();

    for (auto& Pair : m_RenderTargets)
    {
        Safe_Release(Pair.second);
    }

    m_RenderTargets.clear();
}
