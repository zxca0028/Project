#pragma once

#include "Base.h"

BEGIN(Client)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)

private:

	explicit CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:

	HRESULT Set_Level(class CLevel* pLevel);
	_int    Tick	 (_double DeltaTime);
	_int    Late_Tick(_double DeltaTime);
	HRESULT Render();

public:

	void OnNotify(_bool bIsPlayerAlive);

private:

	class CLevel* m_pCurrentLevel = nullptr;

public:

	virtual void Free() override;

};

END