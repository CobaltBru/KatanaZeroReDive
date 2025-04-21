#pragma once
#include "Scene.h"

class ObjectManager;
class RenderManager;
class Animation;
class Token;
class GPImage;
class HomeScene : public Scene
{
public:
	HomeScene();
	virtual ~HomeScene() = default;

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
	Animation* fence;
	Animation* grass1;
	Animation* grass2;
	Animation* zero_zer;
	Animation* zero_o;
	Animation* katana;
	Animation* wall;
	Animation* select_bg;

	bool timerStart;
	float sceneChangeTimer;

	int cursor;

	Animation* buttons[3];

	FPOINT selectBoxPos;
	void SelectBox(HDC& hdc, FPOINT bpos);
	void reset();

	void nextSceneEffect();
};

