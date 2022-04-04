#pragma once

#define		KEY_ENTER	0x00000001  // ���� Scene���� �Ѿ��
#define		KEY_SPACE	0x00000002  // ����, ��������
#define		KEY_DOWN	0x00000004  // �����̵�

#define		KEY_LB		0x00000008  // ���콺 ��Ŭ��

#define		KEY_F1		0x00000010  // Collision ���� �¿���
#define		KEY_F2		0x00000020  // BlastJelly ����
#define		KEY_F3		0x00000040  // MagneticJelly ����
#define		KEY_F4		0x00000080  // BiggestJelly ����

#define		KEY_J		0x00000100  // Jelly �¿���
#define		KEY_O		0x00000200  // Obstacle �¿���
#define		KEY_P		0x00000400  // Pet �¿���
#define		KEY_ESC     0x00000800  // Pause

#define		KEY_B		0x00001000  // BONUSTIME ����
#define		KEY_F5		0x00002000  // HP���� ����
#define		KEY_F6		0x00004000  // ū HP���� ����
#define	    KEY_F		0x00008000  // FPS ǥ��

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

