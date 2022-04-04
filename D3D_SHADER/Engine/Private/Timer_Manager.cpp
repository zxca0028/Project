#include "Timer_Manager.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

HRESULT CTimer_Manager::Add_Timer(wstring pTimerTag)
{
    if (nullptr != Find_Timer(pTimerTag))
    {
        return E_FAIL;
    }

    CTimer* pTimer = CTimer::Create();

    if (nullptr == pTimer)
    {
        return E_FAIL;
    }

    m_Timers.emplace(pTimerTag, pTimer);
    
    return S_OK;
}

_double CTimer_Manager::Compute_DeltaTime(wstring pTimerTag)
{
    CTimer* pTimer = Find_Timer(pTimerTag);

    if (nullptr == pTimer)
    {
        return 0.f;
    }

    return pTimer->Update_Timer();
}

CTimer* CTimer_Manager::Find_Timer(wstring pTimerTag)
{
    if (m_Timers.end() == m_Timers.find(pTimerTag))
    {
        return nullptr;
    }

    return m_Timers.find(pTimerTag)->second;
}

void CTimer_Manager::Free()
{
    for (auto& Pair : m_Timers)
    {
        Safe_Release(Pair.second);
    }

    m_Timers.clear();
}
