#include "pch.h"
#include "GameObjectManager.h"

#include "GameObject.h"
#include "Player.h"
#include "Ground.h"
#include "Ground_BT.h"
#include "BackGround.h"
#include "BackGround_BT.h"
#include "Obstacles.h"

CGameObjectManager* CGameObjectManager::m_pInstance = nullptr;

CGameObjectManager::CGameObjectManager()
{}

CGameObjectManager::~CGameObjectManager()
{
	Release_GameObjectManager();
}

CGameObjectManager* CGameObjectManager::Get_Instance()
{
	if (nullptr == m_pInstance) { m_pInstance = new CGameObjectManager; }

	return m_pInstance;
}

void CGameObjectManager::Destroy_Instance()
{
	if (nullptr != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void CGameObjectManager::Register_GameObject(OBJECT::TYPE eType, CGameObject* pObject)
{
	m_listGameObject[eType].emplace_back(pObject);
}

void CGameObjectManager::Update_GameObjectManager()
{
	for (int i = 0; i < OBJECT::TYPE::END; ++i)
	{
		for (auto iter = m_listGameObject[i].begin(); iter != m_listGameObject[i].end();)
		{
			if ((*iter)->IsActive())
			{
				int iEvent = (*iter)->Update_GameObject();

				if (OBJ_DEAD == iEvent)
				{
					Safe_Delete(*iter);
					iter = m_listGameObject[i].erase(iter);
				}
				else
				{
					(*iter)->Update_Rect_GameObject();
					++iter;
				}
			}
			else
			{
				++iter;
			}					
		}
	}	

	for (int i = 0; i < OBJECT::TYPE::END; ++i)
	{
		for (auto pObject : m_listGameObject[i])
		{
			if (pObject->IsActive())
			{
				pObject->Update_Rect_GameObject();
				pObject->Late_Update_GameObject();
			}			
		}
	}
}

void CGameObjectManager::Render_GameObjectManager(HDC hDC)
{
	for (int i = 0; i < OBJECT::TYPE::END; ++i)
	{
		for (auto& pObject : m_listGameObject[i])
		{
			if (pObject->IsActive())
			{
				pObject->Render_GameObject(hDC);
			}			
		}
	}
}

void CGameObjectManager::Release_GameObjectManager()
{
	for (int i = 0; i < OBJECT::TYPE::END; ++i)
	{
		for (auto& pObject : m_listGameObject[i])
		{
			Safe_Delete(pObject);
		}

		m_listGameObject[i].clear();

	}
}