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
	Animation Timer;
	Image* hud;

	FPOINT batteryPos;
	FPOINT batterCellPos[BCELLCNT];
	FPOINT TimerPos;

public:
	void init();
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

