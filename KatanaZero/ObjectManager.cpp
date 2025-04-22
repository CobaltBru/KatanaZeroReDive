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
			if ((*iter)->IsDead())
			{
				(*iter)->Release();
				delete (*iter);
				iter = GameObjectList[i].erase(iter);
			}
			else
			{
				(*iter)->Update();
				++iter;
			}			
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
	player = nullptr;
	ReleaseInstance();
}
