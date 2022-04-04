#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
public:

	explicit CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Loading() = default;

public:

	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual _int	Tick(_double DeltaTime) override;
	virtual HRESULT Render()				override;

private:

	class CLoader* m_pLoader	= nullptr;
	LEVEL		   m_eNextLevel = LEVEL_END;

private:

	HRESULT Ready_Layer_BG_Logo(const _tchar* PlayerTag);

public:

	static  CLevel_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void			Free() override;

};

END