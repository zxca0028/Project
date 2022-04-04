#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Start final : public CLevel
{
private:

	explicit CLevel_Start(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Start() = default;

public:

	virtual HRESULT NativeConstruct()		     override;
	virtual _int    Tick(_double DeltaTime) override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

public:

	static  CLevel_Start* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END