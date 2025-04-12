#pragma once
#include "Singleton.h"
#include "config.h"
#include <list>

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

	void AddGameObject(EObjectType  InObjectType, GameObject* InGameObject) { GameObjectList[(int)InObjectType].push_back(InGameObject); }
private:
	list<GameObject*> GameObjectList[(int)EObjectType::End];
};

