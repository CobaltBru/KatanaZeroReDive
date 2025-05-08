#include "RenderManager.h"
#include "GameObject.h"
#include "SnapShotManager.h"
#include "ReplayObjects.h"

void RenderManager::Init()
{
}

void RenderManager::Render(HDC hdc)
{
	RenderBackGround(hdc);
	RenderNonAlphaBlend(hdc);
	RenderAlphaBlend(hdc);
	RenderUI(hdc);
}

void RenderManager::AddRenderGroup(ERenderGroup InRenderGroup, GameObject* InGameObject)
{
	GameObjectList[(int)InRenderGroup].push_back(InGameObject);
}

void RenderManager::AddRenderGroupFront(ERenderGroup InRenderGroup, GameObject* InGameObject)
{
	GameObjectList[(int)InRenderGroup].push_front(InGameObject);
}

void RenderManager::Release()
{
	for (int i = 0; i < (int)ERenderGroup::End; ++i)
		GameObjectList[i].clear();

	ReleaseInstance();
}

void RenderManager::RenderBackGround(HDC hdc)
{
	for (auto& iter : GameObjectList[(int)ERenderGroup::BackGround])
		iter->Render(hdc);
	
	GameObjectList[(int)ERenderGroup::BackGround].clear();
}

void RenderManager::RenderNonAlphaBlend(HDC hdc)
{
	for (auto& iter : GameObjectList[(int)ERenderGroup::NonAlphaBlend])
	{
		if (SnapShotManager::GetInstance()->IsReplaying())
		{
			if (dynamic_cast<ReplayBase*>(iter))
			{
				iter->Render(hdc);
			}
		}
		else
		{
			iter->Render(hdc);
		}
		
	}

	GameObjectList[(int)ERenderGroup::NonAlphaBlend].clear();
}

void RenderManager::RenderAlphaBlend(HDC hdc)
{
	for (auto& iter : GameObjectList[(int)ERenderGroup::AlphaBlend])
	{
		if (SnapShotManager::GetInstance()->IsReplaying())
		{
			if (dynamic_cast<ReplayBase*>(iter))
			{
				iter->Render(hdc);
			}
		}
		else
		{
			iter->Render(hdc);
		}

	}

	GameObjectList[(int)ERenderGroup::AlphaBlend].clear();
}

void RenderManager::RenderUI(HDC hdc)
{
	for (auto& iter : GameObjectList[(int)ERenderGroup::UI])
		iter->Render(hdc);

	GameObjectList[(int)ERenderGroup::UI].clear();
}
