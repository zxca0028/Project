#pragma once

typedef struct tagInfo
{
	float	fX;
	float	fY;

	int		iCX;
	int		iCY;

}INFO, *PINFO;


typedef struct tagFrame
{
	int			iFrameStart;
	int			iFrameEnd;
	int			iFrameState;

	ULONGLONG	dwFrameSpeed;
	ULONGLONG	dwFrameTime;

}FRAME, *PFRAME;

typedef struct tagBonusTime
{
	bool	B;
	bool	O;
	bool	N;
	bool	U;
	bool	S;
	bool	T;
	bool	I;
	bool	M;
	bool	E;

}BONUSTIME, *PBONUSTIME;

typedef struct tagCookiProfile
{
	// 쿠키가방에서 표시될 번호
	int  iIndex;

	// 쿠키가방에서 표시될 위치
	RECT tRect;

	// 정렬을 위한 키값
	const TCHAR* szCookieName;

	// 프로필 이미지 키값
	const TCHAR* szKeyValue;

	// 쿠키 스프라이트 이미지 키값
	const TCHAR* szSpriteKeyValue;

	// 선택됐는지 판단
	bool IsSelected = false;

	// 쿠키가 사용 가능한지 판단
	bool IsAvailable = false;

	// 쿠키 등급
	int iGrade;

}COOKIEINFO;