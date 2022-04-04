#pragma once

#define		KEY_ENTER	0x00000001  // 다음 Scene으로 넘어가기
#define		KEY_SPACE	0x00000002  // 점프, 더블점프
#define		KEY_DOWN	0x00000004  // 슬라이딩

#define		KEY_LB		0x00000008  // 마우스 우클릭

#define		KEY_F1		0x00000010  // Collision 영역 온오프
#define		KEY_F2		0x00000020  // BlastJelly 생성
#define		KEY_F3		0x00000040  // MagneticJelly 생성
#define		KEY_F4		0x00000080  // BiggestJelly 생성

#define		KEY_J		0x00000100  // Jelly 온오프
#define		KEY_O		0x00000200  // Obstacle 온오프
#define		KEY_P		0x00000400  // Pet 온오프
#define		KEY_ESC     0x00000800  // Pause

#define		KEY_B		0x00001000  // BONUSTIME 생성
#define		KEY_F5		0x00002000  // HP포션 생성
#define		KEY_F6		0x00004000  // 큰 HP포션 생성
#define	    KEY_F		0x00008000  // FPS 표시

#define		KEY_1       0x00010000
#define		KEY_2       0x00020000
#define		KEY_3       0x00040000
#define		KEY_4       0x00080000
#define		KEY_5       0x00100000
#define		KEY_6       0x00200000
#define		KEY_7       0x00400000
#define		KEY_8       0x00800000
#define		KEY_9       0x01000000


class CKeyManager
{
private:

	 CKeyManager();
	~CKeyManager();

public:

	static CKeyManager* Get_Instance();
	static void Destroy_Instance();
	void Key_Initialize() { m_dwKey = 0; }

public:

	void Key_Update();
	bool Key_Up(DWORD dwKey);
	bool Key_Down(DWORD dwKey);
	bool Key_Pressing(DWORD dwKey);

private:

	static CKeyManager* m_pInstance;

	DWORD	m_dwKey;
	DWORD	m_dwKeyDown;
	DWORD	m_dwKeyUp;
};

