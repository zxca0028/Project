#pragma once

class CScene;

class CSceneManager final
{
private:

	 CSceneManager();
	~CSceneManager();

public:

	static CSceneManager* Get_Instance();
	static void Destroy_Instance();

public:

	void Scene_Change(SCENE::TYPE eNextID);
	void Update_Scene_Manager();
	void Render_Scene_Manager(HDC hDC);
	void Release_Scene_Manager();

public:

	CScene* Get_Scene() { return m_pScene; }

private:

	static CSceneManager* m_pInstance;

	CScene* m_pScene;

	SCENE::TYPE	m_eCurrentScene;
	SCENE::TYPE	m_eNextScene;
};

