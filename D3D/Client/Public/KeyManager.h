#pragma once

#define		KEY_ENTER	0x00000001  
#define		KEY_SPACE	0x00000002
#define		KEY_LB		0x00000004
#define		KEY_RB		0x00000008

#define		KEY_W		0x00000010
#define		KEY_S		0x00000020
#define		KEY_A		0x00000040
#define		KEY_D		0x00000080

#define     KEY_UP      0x00000100
#define     KEY_DOWN    0x00000200
#define     KEY_LEFT    0x00000400
#define     KEY_RIGHT   0x00000800

#define		KEY_ALT		0x00001000
#define		KEY_Z		0x00002000
#define		KEY_C		0x00004000
#define		KEY_T		0x00008000

#define     KEY_F1      0x00010000
#define     KEY_F2      0x00020000
#define     KEY_F3      0x00040000
#define     KEY_F4      0x00080000

#define		KEY_F5		0x00100000
#define		KEY_1       0x00200000
#define		KEY_2       0x00400000

#define		KEY_Q		0x01000000
#define		KEY_W		0x02000000
#define		KEY_E		0x04000000
#define		KEY_R		0x08000000

#define		KEY_F		0x10000000


#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CKeyManager final : public CBase
{
	DECLARE_SINGLETON(CKeyManager)

private:

	 explicit CKeyManager();
	 virtual ~CKeyManager() = default;

public:

	void Key_Update();
	bool Key_Up(DWORD dwKey);
	bool Key_Down(DWORD dwKey);
	bool Key_Pressing(DWORD dwKey);

private:

	DWORD	m_dwKey;
	DWORD	m_dwKeyDown;
	DWORD	m_dwKeyUp;

public:

	virtual void Free() override;

};

END