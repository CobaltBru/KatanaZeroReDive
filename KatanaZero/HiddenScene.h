#pragma once
#include "Scene.h"

class ObjectManager;
class RenderManager;
class CollisionManager;
class SnapShotManager;
class ScrollManager;
class LineManager;
class ScreenEffectManager;
class EffectManager;
class GPImageManager;
class Background;
class HiddenScene : public Scene
{
public:
	HiddenScene();
	virtual ~HiddenScene() = default;

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

	void LoadBackground();
	void LoadObject();
private:
	ObjectManager* ObjectManager;
	RenderManager* RenderManager;
	CollisionManager* CollisionManager;
	SnapShotManager* snapShotManager;
	ScrollManager* ScrollManager;
	LineManager* LineManager;
	ScreenEffectManager* screenEffectManager;
	EffectManager* fxManager;
	GPImageManager* gpImageManager;

	vector<Background*> MoveBackGrounds;

};

