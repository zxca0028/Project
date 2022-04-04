#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CRandom final : public CBase
{
	DECLARE_SINGLETON(CRandom)

private:

	explicit CRandom();
	virtual ~CRandom() = default;

public:

	_int   Get_RandomInt(_int iMin, _int iMax);
	_float Get_RandomFloat(_float fMin, _float fMax);

private:

	random_device m_RandomDevice;	

private:

	virtual void Free() override;

};

END