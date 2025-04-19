#pragma once
#include "config.h"
#include "GameObject.h"
#include "Animation.h"

class UIGame : public GameObject
{
private:
	Animation ani;
public:
	void init();
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

