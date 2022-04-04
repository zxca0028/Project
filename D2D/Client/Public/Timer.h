#pragma once

#include "Base.h"

BEGIN(Client)

class CTimer final : public CBase
{
private:

	explicit CTimer();
	virtual ~CTimer() = default;

public:

	HRESULT NativeConstruct();
	_double Update_Timer();

private:

	_double		  m_fDeltaTime = 0.f;
	LARGE_INTEGER m_tCPUTick   = {};
	LARGE_INTEGER m_tBegin     = {};
	LARGE_INTEGER m_tEnd       = {};

public:

	static  CTimer* Create();
	virtual void Free() override;

};

END