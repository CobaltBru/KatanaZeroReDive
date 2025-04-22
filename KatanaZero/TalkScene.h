#pragma once
#include "Scene.h"

class ObjectManager;
class RenderManager;
class Animation;
class Token;
class GPImage;
class TalkScene : public Scene
{
public:
	TalkScene();
	virtual ~TalkScene() = default;

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

	Animation* background;
	
};

