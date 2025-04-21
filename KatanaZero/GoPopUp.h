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

	FPOINT* playerPos;
	FPOINT* destPos;

	bool isActive;

public:
	GoPopUp();
	virtual ~GoPopUp() = default;

	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void On(FPOINT& player, FPOINT& dest);
	void Off();
};

