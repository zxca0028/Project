#pragma once

class CGameObject abstract
{
public:

	explicit CGameObject();
	virtual ~CGameObject();

public:

	virtual  int Ready_GameObject()		    = 0;
	virtual  int Update_GameObject()	    = 0;
	virtual void Late_Update_GameObject()	= 0;
	virtual void Render_GameObject(HDC hDC)	= 0;
	virtual void Release_GameObject()	    = 0;

	void		 Update_Rect_GameObject();

	///////////////////////////////////////////

	INFO&	 Get_Info()					   { return m_tInfo; }
	void	 Set_Info(float fX, float fY)  { m_tInfo.fX = fX; m_tInfo.fY = fY; }
										   
	RECT&    Get_Rect()					   { Update_Rect_GameObject(); return m_tRect; }
	RECT&	 Get_CollisionRect()		   { return m_tCollisionRect; }
										   
	float&	 Get_Speed()				   { return m_fSpeed; }
	void	 Set_Speed(float fSpeed)	   { m_fSpeed *= fSpeed; }
										   
	void	 Set_Dead()					   { m_bIsDead = true; }
										   
	bool	 Get_CollisionView()		   { return m_bCollisionView; }
										   
	bool	 IsActive()					   { return m_bIsActive; }
	void	 Active()					   { m_bIsActive = true; }
	void	 Passive()					   { m_bIsActive = false; }

	void	 Set_Finish()				   { m_bFinish = true; }

public:	

	INFO			m_tCollisionInfo;
	RECT			m_tCollisionRect;
	bool			m_bCollisionView;
					
	INFO			m_tInfo;
	RECT			m_tRect;
					
	float			m_fSpeed;
					
	bool			m_bIsDead;
	
	bool			m_bIsActive;

	bool			m_bFinish = false;
};

