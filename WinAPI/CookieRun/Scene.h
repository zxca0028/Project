#pragma once

class CScene abstract
{
public:

	explicit CScene();
	virtual ~CScene();

public:

	virtual  int Ready_Scene()			= 0;
	virtual void Update_Scene()			= 0;
	virtual void Render_Scene(HDC hDC)  = 0;
	virtual void Release_Scene()		= 0;

};

