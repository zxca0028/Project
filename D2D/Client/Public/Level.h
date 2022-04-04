#pragma once

#include "Base.h"

BEGIN(Client)

class CLevel abstract : public CBase
{
protected:

	explicit CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;

public:

	virtual HRESULT NativeConstruct(LEVEL eLevel);
	virtual _int    Tick     (_double DeltaTime);
	virtual _int    Late_Tick(_double DeltaTime);
	virtual HRESULT Render();

public:

	void  OnNotify(_bool bIsGameOver) { m_bIsGameOver = bIsGameOver; }
	LEVEL CurrentLevelIndex() { return m_eLevelIndex; }

protected:

	LEVEL m_eLevelIndex = LEVEL_END;
	_bool m_bIsGameOver = false;

protected:

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:

	virtual void Free() override;
};

END