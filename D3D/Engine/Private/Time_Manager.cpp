#include "Time_Manager.h"

IMPLEMENT_SINGLETON(CTime_Manager)

CTime_Manager::CTime_Manager()
{}

HRESULT CTime_Manager::Add_Timer(string pTimerTag)//const _tchar* pTimerTag)
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
	
	m_Timer.emplace(pTimerTag, pTimer);

	return S_OK;
}

_float CTime_Manager::Compute_DeltaTime(string pTimerTag)//const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
	{
		return 0.f;
	}

	return pTimer->Update_Timer();
}

CTimer* CTime_Manager::Find_Timer(string pTimerTag)//const _tchar* pTimerTag)
{
	auto iter = m_Timer.find(pTimerTag);//find_if(m_Timer.begin(), m_Timer.end(), CTagFinder(pTimerTag));

	if (iter == m_Timer.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CTime_Manager::Free()
{
	for (auto& Pair : m_Timer)
	{
		Safe_Release(Pair.second);
	}

	m_Timer.clear();
}
