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
	// ��Ű���濡�� ǥ�õ� ��ȣ
	int  iIndex;

	// ��Ű���濡�� ǥ�õ� ��ġ
	RECT tRect;

	// ������ ���� Ű��
	const TCHAR* szCookieName;

	// ������ �̹��� Ű��
	const TCHAR* szKeyValue;

	// ��Ű ��������Ʈ �̹��� Ű��
	const TCHAR* szSpriteKeyValue;

	// ���õƴ��� �Ǵ�
	bool IsSelected = false;

	// ��Ű�� ��� �������� �Ǵ�
	bool IsAvailable = false;

	// ��Ű ���
	int iGrade;

}COOKIEINFO;