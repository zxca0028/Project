#pragma once

#include "Base.h"

BEGIN(Client)

class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager)

public:

	enum class MOUSE { X, Y, WHEEL, MOUSE_END };

private:

	explicit CKey_Manager();
	virtual ~CKey_Manager() = default;

public:

	_int  Key_Update();
	_bool Key_Up      (DWORD dwKey);
	_bool Key_Down    (DWORD dwKey);
	_bool Key_Pressing(DWORD dwKey);

private:

	DWORD	m_dwKey		 = 0;
	DWORD	m_dwKeyDown	 = 0;
	DWORD	m_dwKeyUp	 = 0;

public:

	virtual void Free() override;

};

END