#pragma once
#include "Score.h"

// 장면
const	int		SEQUENCE = 0;     // 0 : 기본 
							      // 1 : 렌더 테스트
							      // 2 : 스테이지	
								  // 3 : 스테이지 로딩
								  // 4 : 결과창
								  // 5 : 로비
// 플레이어 속도
const	float	PLAYER_SPEED = 8.f;


// 젤리 점수
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

// 테스트 스테이지 온오프
const	int		TestOnOff = 0;   // 0 : Off
								 // 1 : On


// 체력 감소 속도 ( 게이지 크기 = 915 ), 0.5초마다 HP만큼 크기가 줄어듦
//const	int		HP = 5;		     // 46 : 10초
								 // 16 : 30초
								 // 8  :  1분
								 // 4  :  2분	
								 // 5  : 정규

// 아이템 지속시간
const	int		BLAST   = 2000;
const	int		MAGNET  = 5000;
const	int		BIGGEST = 5000;

// HP관련
const	int		PLUSHP_SMALL = 30;       // 작은 HP포션
const   int		PLUSHP_BIG   = 90;       // 큰 HP포션
const	int		MINUSHP      = 50;       // 장애물에 닿았을 때
