#include "pch.h"
#include "CollisionManager.h"

#include "Player.h"
#include "GameObject.h"

CCollisionManager::CCollisionManager()
{}

CCollisionManager::~CCollisionManager()
{}

void CCollisionManager::Collision_Rect(list<CGameObject*>& PlayerList, list<CGameObject*>& JellyList)
{
	RECT	rc = {};
	RECT	PlayerRect = PlayerList.front()->Get_CollisionRect();
	RECT	JellyRect = JellyList.front()->Get_Rect();

	if (IntersectRect(&rc, &PlayerRect, &JellyRect))
	{
		JellyList.front()->Set_Dead();
	}
}
