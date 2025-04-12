#include "ObjectManager.h"
#include "GameObject.h"

void ObjectManager::Init()
{
}

void ObjectManager::Update()
{
	for (int i = 0; i < (int)EObjectType::End; ++i)
	{
		for (auto iter = GameObjectList[i].begin(); iter != GameObjectList[i].end();)
		{
			(*iter)->Update();

			++iter;
		}
	}
}

void ObjectManager::Release()
{
	for (int i = 0; i < (int)EObjectType::End; ++i)
	{
		for (auto& iter : GameObjectList[i])
		{
			iter->Release();
			delete iter;
		}
		GameObjectList[i].clear();
	}
	
	ReleaseInstance();
}
