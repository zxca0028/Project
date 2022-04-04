#pragma once

#include "Level.h"

BEGIN(Client)

class CLevel_Result final : public CLevel
{
private:

	explicit CLevel_Result(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Result() = default;

public:

	virtual HRESULT NativeConstruct(LEVEL eLevel) override;
	virtual _int    Tick	 (_double DeltaTime)  override;
	virtual _int    Late_Tick(_double DeltaTime)  override;
	virtual HRESULT Render()					  override;

private:

	HRESULT Ready_Layer_Result(wstring LayerTag);
	HRESULT Ready_Layer_UI    (wstring LayerTag);

private:

	_float m_fTimeAcc = 0.f;

public:

	static CLevel_Result* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual void Free() override;

};

END