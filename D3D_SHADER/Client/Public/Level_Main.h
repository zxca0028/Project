#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Main final : public CLevel
{
private:

	explicit CLevel_Main(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Main() = default;

public:

	virtual HRESULT NativeConstruct()		     override;
	virtual _int    Tick     (_double DeltaTime) override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

private:

	HRESULT Ready_Light();
	HRESULT Ready_Layer_Camera    (wstring pLayerTag);
	HRESULT Ready_Layer_SkyBox    (wstring pLayerTag);
	HRESULT Ready_Layer_Terrain   (wstring pLayerTag);
	HRESULT Ready_Layer_Player    (wstring pLayerTag);
	HRESULT Ready_Layer_TestObject(wstring pLayerTag);

public:

	static  CLevel_Main* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END	