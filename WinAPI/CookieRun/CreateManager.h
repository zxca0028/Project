
class CCreateManager
{
private:

	CCreateManager();
	~CCreateManager();

public:

	static CCreateManager* Get_Instance();
	static void Destroy_Instance();

public:

	 int Create_Objects();
	void ReCreate_All_Objects();
	void ReCreate_BG_Ground(int iFinal);
	void Create_BonusTime_Objects();
	void Create_FeverTime_Object();

	void Set_Cookie(const TCHAR* pCookie) { m_pCookie = pCookie; }
	const TCHAR* Get_Cookie() { return m_pCookie; }

	const TCHAR* SentenceList();

	void ReCreateOnlyGround();

private:

	void CreateObstacles();
	void CreateObstaclesWithoutJ2();
	void StartJelly();	

public:

	static CCreateManager* m_pInstance;

	void     Set_Phase(PHASE::TYPE ePhase) { m_ePhase = ePhase; }
	PHASE::TYPE     Get_Phase() { return m_ePhase; }

private:

	PHASE::TYPE		m_ePhase = PHASE::TYPE::_1ST;

	const TCHAR* m_pCookie = L"BraveCookie";

	bool				   m_bCreateLock;
	bool				   m_bCreateScoreLock;

	bool				   m_bCreateJellyLock;
	ULONGLONG			   m_tCreateJellyLock;


	bool				   m_bCreateJellyLock_BonusTime;
	bool				   m_bCreateJellyLock_FeverTime;

	bool				   m_bCreateBlastLock;
	int					   m_iCreateBlastLock;
	ULONGLONG			   m_tBlastDistance;

};