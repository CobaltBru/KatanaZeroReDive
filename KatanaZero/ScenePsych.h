#pragma once
#include "config.h"
#include "GameObject.h"
#include "Animation.h"
//¾É±â
//ÀÏ¾î³ª±â->¿À¸¥ÂÊ °È±â
//¾à²¨³»±â
//¿ÞÂÊ ¾àµé°í °È±â
//¾à³õ±â
//¿À¸¥ÂÊ ¾àµé°í °È±â
//¾É´ÂÁß->¾É±â

enum TASK{SIT,MOVE};

class Image;

class Animation;
class ScenePsych : public GameObject
{
private:
	Animation sit;
	Animation standUp;
	Animation sitDown;
	Animation walk;
	Animation desk;
	Animation walk_ch;
	Animation inject;
	TASK currentTask;
	

	Animation* MoveList[8];
	int moveIdx = 0;
	float timer;
	bool moveEnd;
public:
	ScenePsych();
	virtual ~ScenePsych() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Move();

	void MoveFunc();
	inline bool isMoveEnd() { return moveEnd; }
};

