#pragma once
#include "config.h"
#include "GameObject.h"
#include "Animation.h"
#define UISCALE (WINSIZE_X/640.f)
#define ONE 0
#define TWO 1
#define BCELLCNT 11
class UIGame : public GameObject
{
private:
	Animation battery;
	Animation batteryCellBlue[BCELLCNT];
	Animation batteryCellRed[BCELLCNT];
	Animation shiftButton;
	Animation timerUI;
	Animation timerBarUI;
	Animation itemSlot;
	Image* hud;
	Image* mouseLeft;
	Image* mouseRight;
	Image* item1;
	Image* item2;
	vector<string> itemKey;

	FPOINT batteryPos;
	FPOINT batteryCellPos[BCELLCNT];
	FPOINT timerPos;
	FPOINT timerBarPos;
	FPOINT slotPos;
	FPOINT item1Pos;
	FPOINT item2Pos;

public:
	void init();
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

