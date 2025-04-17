#pragma once
#include "Scene.h"

class ObjectManager;
class RenderManager;
class CollisionManager;
class SnapShotManager;
class ScrollManager;
class LineManager;
class ChatManager;
class ScreenEffectManager;
class EffectManager;
class Stage1Scene : public Scene
{
public:

	Stage1Scene();
	virtual ~Stage1Scene() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

protected:
	virtual HRESULT InitImage();
	virtual HRESULT InitObject();
	virtual HRESULT InitEffects();

private:
	void TestCode();
private:
	ObjectManager* ObjectManager;
	RenderManager* RenderManager;
	CollisionManager* CollisionManager;
	SnapShotManager* snapShotManager;
	float elapsedTime;
	ScrollManager* ScrollManager;
	LineManager* LineManager;
	ScreenEffectManager* screenEffectManager;
	EffectManager* fxManager;

	ChatManager* chatManager;
};

