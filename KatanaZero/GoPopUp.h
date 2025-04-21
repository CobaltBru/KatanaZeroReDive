#pragma once
#include "config.h"
#include "GameObject.h"


class Animation;
class GPImage;
class GoPopUp :public GameObject
{
private:
	Animation* goImage;
	Animation* arrowImage;
	GPImage* angleArrow;


	float imageAngle;
	float posAngle;
	FPOINT anglePos;

	FPOINT* playerPos;
	FPOINT* destPos;
	FPOINT popUpPos;

	bool angleMode;
	bool isActive;
	bool flip;
	bool oldFlip;

public:
	GoPopUp();
	virtual ~GoPopUp() = default;

	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void On(FPOINT* player, FPOINT* dest);
	void Off();

	void calcPos();
	void ArrowCalc();
};

