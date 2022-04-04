#include "pch.h"
#include "Random.h"

IMPLEMENT_SINGLETON(CRandom);

CRandom::CRandom()
{
}

_int CRandom::Get_RandomInt(_int iMin, _int iMax)
{
	mt19937 Generator(m_RandomDevice());
	
	uniform_int_distribution<int> RandomInt(iMin, iMax);

	return RandomInt(Generator);
}

_float CRandom::Get_RandomFloat(_float fMin, _float fMax)
{
	mt19937 Generator(m_RandomDevice());

	uniform_real_distribution<float> RandomFloat(fMin, fMax);

	return RandomFloat(Generator);
}

void CRandom::Free()
{
}
