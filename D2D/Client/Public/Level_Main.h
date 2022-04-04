#pragma once

#include "Level.h"

BEGIN(Client)

class CLevel_Main final : public CLevel
{
private:

	explicit CLevel_Main(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Main() = default;

public:

	virtual HRESULT NativeConstruct(LEVEL eLevel) override;
	virtual _int    Tick	 (_double DeltaTime)  override;
	virtual _int    Late_Tick(_double DeltaTime)  override;
	virtual HRESULT Render()					  override;

private:

	HRESULT Ready_Layer_Camera();
	HRESULT Ready_Layer_Player     (wstring LayerTag);
	HRESULT Ready_Layer_Title      (wstring LayerTag);

	HRESULT Ready_Layer_Point(wstring LayerTag);

public:

	static CLevel_Main* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual void Free() override;

};

END