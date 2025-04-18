#pragma once
#include "config.h"
#include <array>

class Image;
class Animation
{
private:
	Image* image;
	std::vector<pair<int,float>> times;
	int frameX;

	int timeIdx;
	int idx;

	float timer;
	bool isOn;
public:
	void Init(Image* image, int frameX);
	void Update();
	void Render(HDC hdc);

	void On();
	void Off();
};

