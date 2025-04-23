#pragma once
#include "Scene.h"

class ObjectManager;
class RenderManager;
class CollisionManager;
class Animation;
class Token;
class GPImage;
class ScenePsych;
class ScenePlayer;
class ScrollManager;
class LineManager;
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
	CollisionManager* CollisionManager;
	LineManager* LineManager;

	Animation* background;
	Animation* firePlace;
	ScenePsych* psych;
	ScenePlayer* player;
	FPOINT pos;

	FPOINT chairPos;
	FPOINT DoorPos;
};

