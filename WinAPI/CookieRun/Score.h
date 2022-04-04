#pragma once

#include "GameObject.h"

class CScore final : public CGameObject
{
private:

	explicit CScore();

public:

	virtual ~CScore();

public:

	// CGameObject을(를) 통해 상속됨
	virtual  int Ready_GameObject() override;
	virtual  int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject() override;

public:

	void FirstCycleFinish() { m_bFirstCycle = true; }

public:

	static CGameObject* Create();

public:

	static int	   m_iHiScore;
	static int	   m_iCurrentScore;
	static int	   m_iHiScoreBuffer;
	static int	   m_iGetCoin;
	static bool	   m_bFirstCycle;

private:

	int			   m_iMoveX;
	int			   m_iMoveY;
				   
	int			   m_iJellyX;


};

