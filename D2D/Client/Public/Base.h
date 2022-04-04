#pragma once

#include "Client_Define.h"

BEGIN(Client)

class CBase abstract
{
protected:

	explicit CBase();
	virtual ~CBase() = default;

public:

	unsigned long AddRef();
	unsigned long Release();

private:

	unsigned long m_dwRefCnt = 0;

public:

	virtual void Free() = 0;
};

END