#pragma once
#include "config.h"
#include "GameObject.h"
#include "Animation.h"
#include "Observer.h"
#define UISCALE (WINSIZE_X/640.f)
#define ONE 0
#define TWO 1
#define BCELLCNT 11
class UIGame : public GameObject, public Observer
{
private:
	Image* hud;

	Animation battery;
	Animation batteryCellBlue[BCELLCNT];
	Animation batteryCellRed[BCELLCNT];
	Animation shiftButton;

	Animation timerUI;
	Animation timerBarUI;

	Animation itemSlot;
	
	Image* mouseLeft;
	Image* mouseRight;
	Image* item1;
	Image* item2;

	FPOINT batteryPos;
	FPOINT batteryCellPos[BCELLCNT];

	FPOINT timerPos;
	FPOINT timerBarPos;

	FPOINT slotPos;
	FPOINT item1Pos;
	FPOINT item2Pos;

	float batteryGage;
	float timeGage;
	string leftItem, rightItem;

	float timer = 0.f;

public:
	void init();
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	virtual void EventPlayerState(const PlayerState& ps) override;
	virtual void TimerUIEvent(const float t) override;
};

