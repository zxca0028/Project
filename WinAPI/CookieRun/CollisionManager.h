#pragma once

class CGameObject;

class CCollisionManager
{
public:

	CCollisionManager();
	~CCollisionManager();

public:

	static void Collision_Rect(list<CGameObject*>& PlayerList, list<CGameObject*>& JellyList);

};

