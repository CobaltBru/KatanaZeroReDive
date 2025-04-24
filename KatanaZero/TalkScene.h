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
class ChatManager;
class TalkScene : public Scene
{
public:
	TalkScene();
	virtual ~TalkScene() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	void renderEffect(HDC hdc);
protected:
	virtual HRESULT InitImage();
	virtual HRESULT InitObject();

private:
	ObjectManager* ObjectManager;
	RenderManager* RenderManager;
	CollisionManager* CollisionManager;
	LineManager* LineManager;
	ChatManager* chatManager;
	Animation* background;
	Animation* firePlace;
	ScenePsych* psych;
	ScenePlayer* player;
	FPOINT pos;

	FPOINT chairPos;
	FPOINT DoorPos;

	bool inChat;
	bool badChat;
	bool chatDone;
	bool firstdone;
	float timer = 0;

	

	bool drugTimerOn;
	float drugTimer;
	bool effectOn;
	float effectTimer = 0;
};

