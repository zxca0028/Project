#pragma once

#include "Timer.h"

BEGIN(Client)

class CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)

private:

	explicit CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:

	HRESULT Add_Timer(wstring pTimerTag);
	_double Compute_DeltaTime(wstring pTimerTag);

private:

	CTimer* Find_Timer(wstring pTimerTag);

private:

	typedef unordered_map<wstring, CTimer*> TIMERS;
	TIMERS m_Timers;

public:

	virtual void Free() override;

};

END	