#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{
private:

	explicit CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:

	HRESULT NativeConstruct(LEVEL eLevel);

public:

	static unsigned __stdcall Thread_Main(void* pArg);

private:

	_bool m_bIsFinish = false;

public:

	const _bool IsFinished() const { return m_bIsFinish; }

private:

	HANDLE			  m_hThread = nullptr;
	CRITICAL_SECTION  m_CS	    = {};
	LEVEL			  m_eNextLevel = LEVEL::LEVEL_END;

	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

private:

	HRESULT Ready_Level_Main();

public:

	static  CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void Free() override;

};

END