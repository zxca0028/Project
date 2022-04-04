#include "pch.h"
#include "Timer.h"

BEGIN(Client)

CTimer::CTimer()
{
}

HRESULT CTimer::NativeConstruct()
{
    QueryPerformanceFrequency(&m_tCPUTick);
    QueryPerformanceCounter(&m_tBegin);
    QueryPerformanceCounter(&m_tEnd);

    return S_OK;
}

_double CTimer::Update_Timer()
{
    QueryPerformanceCounter(&m_tEnd);

    m_fDeltaTime = _double(m_tEnd.QuadPart - m_tBegin.QuadPart) / m_tCPUTick.QuadPart;

    m_tBegin = m_tEnd;

    return m_fDeltaTime;
}

CTimer* CTimer::Create()
{
    CTimer* pInstance = new CTimer;

    if (FAILED(pInstance->NativeConstruct()))
    {
        MSGBOX("Failed to Create Timer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTimer::Free()
{
}

END