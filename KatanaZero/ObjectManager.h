#pragma once
#include "Singleton.h"
#include "config.h"
#include <list>
#include "Player.h"
#include "SimpleObject.h"

enum class EObjectType : uint8_t
{
	GameObject,
	End
};

class GameObject;
class ObjectManager : public Singleton<ObjectManager>
{
public:
	void Init();	
	void Update();
	void Release();

	void AddGameObject(EObjectType  InObjectType, GameObject* InGameObject) 
	{ 
		GameObjectList[(int)InObjectType].push_back(InGameObject);
		if (auto* p = dynamic_cast<SimpleObject*>(InGameObject))
			player = p;
	}

	const list<GameObject*>& GetObjects() const { return GameObjectList[(int)EObjectType::GameObject]; }
	GameObject* GetPlayer() const { 
		if (!player) return nullptr;
		return player; }
private:
	list<GameObject*> GameObjectList[(int)EObjectType::End];
	GameObject* player{ nullptr };
};

