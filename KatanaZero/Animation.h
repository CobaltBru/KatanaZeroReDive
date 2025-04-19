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
	bool anker;
	bool isSour;
	float sStart;
	float sEnd;

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

	void setPos(FPOINT pos, bool flip, bool anker = false);
	void setSour(float start, float end);
	inline void setFrame(int idx) { this->idx = idx; }
	void setTask(initializer_list<pair<int, float>> lst);
	void setTask(std::vector < pair<int, float>>& lst);
};

