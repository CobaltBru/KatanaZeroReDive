#pragma once
#include "Scene.h"

class ObjectManager;
class RenderManager;
class CollisionManager;
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

private:
	ObjectManager* ObjectManager;
	RenderManager* RenderManager;
	CollisionManager* CollisionManager;
};

