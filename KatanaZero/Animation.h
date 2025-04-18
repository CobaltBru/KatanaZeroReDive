#pragma once
#include "config.h"
#include <array>

class Image;
class Animation
{
private:
	Image* image;
	FPOINT pos;
	//idx, time
	std::vector<pair<int,float>> tasks;
	int frameX;

	int taskIdx;
	int idx;
	bool flip;

	float timer;
	bool isStart;
	bool isOn;

public:
	void Init(Image* image, int frameX);
	void Update();
	void Render(HDC hdc);

	void Start();
	void Stop();
	void On();
	void Off();

	void setPos(FPOINT pos, bool flip);
	void setTask(initializer_list<pair<int, float>>);
};

