#pragma once

class CMainApp
{
public:

	 CMainApp();
	~CMainApp();

public:

	 int Ready_MainApp();
	void Update_MainApp();
	void Render_MainApp();
	void Release_MainApp();

private:

	HDC		m_hDC;

	ULONGLONG	m_tFPS_Time;
	TCHAR		m_szFPS[32];	
	bool		m_bIs_FPS_View;
	int			m_iFPS;

};

