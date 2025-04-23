#pragma once
#include "Singleton.h"
#include "config.h"
#include <list>

enum class ERenderGroup : uint8_t
{
	BackGround,
	NonAlphaBlend,
	AlphaBlend,
	UI,
	End
};

class GameObject;
class RenderManager : public Singleton<RenderManager>
{
public:
	void Init();
	void Render(HDC hdc);
	void Release();

	void AddRenderGroup(ERenderGroup InRenderGroup, GameObject* InGameObject);
	void AddRenderGroupFront(ERenderGroup InRenderGroup, GameObject* InGameObject);

public:
	void RenderBackGround(HDC hdc);
	void RenderNonAlphaBlend(HDC hdc);
	void RenderAlphaBlend(HDC hdc);
	void RenderUI(HDC hdc);	

private:
	list<GameObject*> GameObjectList[(int)ERenderGroup::End];
};

