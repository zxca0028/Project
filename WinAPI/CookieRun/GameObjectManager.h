#pragma once

class CGameObject;

class CGameObjectManager
{
private:

	 CGameObjectManager();
	~CGameObjectManager();

public:

	static CGameObjectManager* Get_Instance();
	static void Destroy_Instance();

	CGameObject*		Get_Player()					{ return m_listGameObject[OBJECT::TYPE::PLAYER].front(); }
	CGameObject*		Get_Pet()						{ return m_listGameObject[OBJECT::TYPE::PET].front(); }
	CGameObject*		Get_BonusTime()					{ return m_listGameObject[OBJECT::TYPE::UI].front(); }
	CGameObject*		Get_Score()						{ return m_listGameObject[OBJECT::TYPE::SCORE].front(); }
	CGameObject*		Get_Flag()						{ return m_listGameObject[OBJECT::TYPE::FLAG].front(); }
														
	list<CGameObject*>* Get_BackGround()				{ return &m_listGameObject[OBJECT::TYPE::BACKGROUND]; }
	list<CGameObject*>* Get_BackGround_2()				{ return &m_listGameObject[OBJECT::TYPE::BACKGROUND_2]; }
	list<CGameObject*>* Get_BT_BackGround()				{ return &m_listGameObject[OBJECT::TYPE::BT_BACKGROUND]; }

	list<CGameObject*>* Get_Ground()					{ return &m_listGameObject[OBJECT::TYPE::GROUND]; }
	list<CGameObject*>* Get_BT_Ground()					{ return &m_listGameObject[OBJECT::TYPE::BT_GROUND]; }

	list<CGameObject*>* Get_Obstacle()					{ return &m_listGameObject[OBJECT::TYPE::OBSTACLE]; }
	list<CGameObject*>* Get_Jelly()						{ return &m_listGameObject[OBJECT::TYPE::JELLY]; }
	list<CGameObject*>* Get_Item()						{ return &m_listGameObject[OBJECT::TYPE::ITEM]; }
	list<CGameObject*>* Get_UI()						{ return &m_listGameObject[OBJECT::TYPE::UI]; }

	list<CGameObject*>* Get_Object(int i)				{ return &m_listGameObject[i]; }

	void	Delete_All_Object()
	{
		for (int i = 0; i < OBJECT::TYPE::SCORE; ++i)
		{
			for (auto pObject : m_listGameObject[i])
			{
				Safe_Delete(pObject);					
			}

			m_listGameObject[i].clear();

		}
	}

public:

	void Register_GameObject(OBJECT::TYPE eType, CGameObject* pObject);
	void Update_GameObjectManager();
	void Render_GameObjectManager(HDC hDC);
	void Release_GameObjectManager();

private:

	static CGameObjectManager* m_pInstance;

	list<CGameObject*> m_listGameObject[OBJECT::TYPE::END];

};

