#pragma once
#include "config.h"
#include "GameObject.h"
#include "Animation.h"
#include "Observer.h"
#define UISCALE (WINSIZE_X/640.f)
#define ONE 0
#define TWO 1
#define BCELLCNT 11
class ParticleEffect;
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

	int Item2FrameX;
	float Item2Scale;
	bool isSlow;
	float batteryTimer;
	float batteryGage;
	float timeTimer;
	float timeMax;
	float timeGage;
	string leftItem, rightItem;

	float timer = 0.f;
	ParticleEffect* part1;
	ParticleEffect* part2;
public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	virtual void EventPlayerState(const ObsPlayerState& ps) override;
	virtual void TimerUIEvent(const float t) override;

	void SetRightItem(string InImageKey, FPOINT InOffset, float InFrameX, float InScale);
	
	void UpdateSlow(bool buttonDown);
	inline void setInfo(float timer)
	{
		batteryTimer = 0.f;
		batteryGage = 1.0f;
		timeTimer = 0.f;
		timeMax = timer;
		timeGage = 1.0f;
	}
	inline void setTimebar(float percent) { timeGage = percent; }
	inline float getBattery() { return batteryGage; }
	inline float getTimer() { return timeGage; }
};

