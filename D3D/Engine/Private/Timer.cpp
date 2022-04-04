#include "Timer.h"

CTimer::CTimer()
{
}

HRESULT CTimer::NativeConstruct()
{
    QueryPerformanceFrequency(&m_tCpuTick);
    QueryPerformanceCounter  (&m_tBegin);
    QueryPerformanceCounter  (&m_tEnd);

    return S_OK;
}

_float CTimer::Update_Timer()
{
    QueryPerformanceCounter(&m_tEnd);

    m_fDeltaTime = _float(m_tEnd.QuadPart - m_tBegin.QuadPart) / m_tCpuTick.QuadPart;

    m_tBegin = m_tEnd;

    return m_fDeltaTime;
}

CTimer* CTimer::Create()
{
    CTimer* pInstance = new CTimer;

    if (FAILED(pInstance->NativeConstruct()))
    {
        MSGBOX("Failed to Creating Timer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTimer::Free()
{}
