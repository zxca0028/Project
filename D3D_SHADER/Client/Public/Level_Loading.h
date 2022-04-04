#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:

	explicit CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Loading() = default;

public:

	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual _int    Tick	 (_double DeltaTime) override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()				     override;

private:

	class CLoader* m_pLoader	= nullptr;
	LEVEL		   m_eNextLevel = LEVEL::LEVEL_END;

public:

	static  CLevel_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void Free() override;
	
};

END