#pragma once
#include "Score.h"

// ���
const	int		SEQUENCE = 0;     // 0 : �⺻ 
							      // 1 : ���� �׽�Ʈ
							      // 2 : ��������	
								  // 3 : �������� �ε�
								  // 4 : ���â
								  // 5 : �κ�
// �÷��̾� �ӵ�
const	float	PLAYER_SPEED = 8.f;


// ���� ����
//namespace SCORE
//{
//	const	int		NORMAL_JELLY    = 9444    * CScore::m_fScale;
//
//	const	int		GOLD_BEAR       = 20555   * CScore::m_fScale;
//	const	int		BIG_GOLD_BEAR   = 66666   * CScore::m_fScale;
//															   
//	const	int		PINK_BEAR       = 29888   * CScore::m_fScale;
//	const	int		BIG_PINK_BEAR   = 77777   * CScore::m_fScale;
//										     				   
//	const	int		RAINBOW_BEAR    = 135555  * CScore::m_fScale;
//										     				   
//	const	int		SILVER_COIN     = 2333    * CScore::m_fScale;
//	const	int		BIG_SILVER_COIN = 13333   * CScore::m_fScale;
//										     				   
//	const	int		GOLD_COIN       = 3222    * CScore::m_fScale;
//	const	int		BIG_GOLD_COIN   = 26666   * CScore::m_fScale;
//}

// �׽�Ʈ �������� �¿���
const	int		TestOnOff = 0;   // 0 : Off
								 // 1 : On


// ü�� ���� �ӵ� ( ������ ũ�� = 915 ), 0.5�ʸ��� HP��ŭ ũ�Ⱑ �پ��
//const	int		HP = 5;		     // 46 : 10��
								 // 16 : 30��
								 // 8  :  1��
								 // 4  :  2��	
								 // 5  : ����

// ������ ���ӽð�
const	int		BLAST   = 2000;
const	int		MAGNET  = 5000;
const	int		BIGGEST = 5000;

// HP����
const	int		PLUSHP_SMALL = 30;       // ���� HP����
const   int		PLUSHP_BIG   = 90;       // ū HP����
const	int		MINUSHP      = 50;       // ��ֹ��� ����� ��
