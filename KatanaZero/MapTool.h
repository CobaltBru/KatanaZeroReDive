#pragma once
#include "Scene.h"

class ObjectManager;
class RenderManager;
class CollisionManager;
class ScrollManager;
class LineManager;
class EffectManager;
class SnapShotManager;
class MapTool : public Scene
{
public:
	MapTool();
	virtual ~MapTool() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;


protected:
	virtual HRESULT InitImage();
	virtual HRESULT InitObject();

private:
	ObjectManager* ObjectManager;
	RenderManager* RenderManager;
	CollisionManager* CollisionManager;
	ScrollManager* ScrollManager;
	LineManager* LineManager;
	EffectManager* fxManager;
	SnapShotManager* snapShotManager;

};

