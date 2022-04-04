#pragma once

#include "Level.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
public:

	typedef struct tagDesc
	{
		_float3 vInitPos;
		_float3 vScale;

		_float fDeletePos;
		_float fSpeed;

		_int iState;

	}DESC;

private:

	explicit CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:

	virtual HRESULT NativeConstruct(LEVEL eLevel) override;
	virtual _int    Tick	 (_double DeltaTime)  override;
	virtual _int    Late_Tick(_double DeltaTime)  override;
	virtual HRESULT Render()					  override;

private:

	HRESULT Add_Object(_float fGap, _float fGround, _float fSpeed);

private:

	HRESULT Ready_Layer_UI(wstring LayerTag);
	HRESULT Ready_Layer_BoundingBox();

private:

	DESC   m_Desc;
	_float m_fTimeAcc = 0.f;
	
private:

	_float m_fInterval = 2.5f;
	_float m_fGap      = 300.f;
	_float m_fGround   = 180.f;
	_float m_fSpeed    = 250;

public:

	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual void Free() override;

};

END