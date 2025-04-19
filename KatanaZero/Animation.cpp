#include "Animation.h"
#include "Image.h"

void Animation::Init(Image* image, int frameX)
{
	this->image = image;
	this->frameX = frameX;
	taskIdx = 0;
	idx = 0;

	timer = 0;
	isStart = false;
	isOn = false;
}

void Animation::Update()
{
	if (isStart)
	{
		timer += TimerManager::GetInstance()->GetDeltaTime();
		if (timer >= tasks[taskIdx].second)
		{
			taskIdx++;
			if (taskIdx >= tasks.size())taskIdx = 0;
			idx = tasks[taskIdx].first;
			timer = 0.f;
		}
	}
	
	
}

void Animation::Render(HDC hdc)
{
	if (isOn)
	{
		image->Render(hdc, pos.x, pos.y, idx, flip);
	}
	
}

void Animation::Start()
{
	taskIdx = 0;
	idx = 0;
	timer = 0;
	isStart = true;
}

void Animation::Stop()
{
	isStart = false;
}

void Animation::On()
{
	isOn = true;
	this->Start();
}

void Animation::Off()
{
	isOn = false;
	this->Stop();
}

void Animation::setPos(FPOINT pos, bool flip)
{
	this->pos = pos;
	this->flip = flip;
}

void Animation::setTask(initializer_list<pair<int, float>> lst)
{
	for (auto task : lst)
	{
		tasks.push_back(task);
	}
}
