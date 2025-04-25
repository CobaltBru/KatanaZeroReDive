#pragma once
#include "Scene.h"

class HeadHunter;
class ObjectManager;
class RenderManager;
class CollisionManager;
class SnapShotManager;
class ScrollManager;
class LineManager;
class ChatManager;
class ScreenEffectManager;
class EffectManager;
class Player;
class BossScene : public Scene
{
public:

	BossScene();
	virtual ~BossScene() = default;

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

	void InitBackgroundImage();
	void LoadBackground();
	void LoadObject();
	void LoadFloor();

	void InitTile();
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

