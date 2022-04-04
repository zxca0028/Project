#pragma once

#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class CTime_Manager final : public CBase
{
	DECLARE_SINGLETON(CTime_Manager)

private:

	explicit CTime_Manager();
	virtual ~CTime_Manager() = default;

public:

	HRESULT Add_Timer(string pTimerTag);//const _tchar* pTimerTag);
	_float  Compute_DeltaTime(string pTimerTag);//const _tchar* pTimerTag);

private:

	CTimer* Find_Timer(string pTimerTag);//const _tchar* pTimerTag);

private:

	//typedef map<const _tchar*, CTimer*> TIMER;
	//TIMER m_Timer;

	typedef unordered_map<string, CTimer*> TIMER;
	TIMER m_Timer;

public:

	virtual void Free() override;

};

END