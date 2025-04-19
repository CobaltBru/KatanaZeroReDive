#include "Animation.h"
#include "Image.h"

void Animation::Init(Image* image, int frameX)
{
	this->image = image;
	this->frameX = frameX;
	taskIdx = 0;
	idx = 0;
	flip = false;

	isSour = false;
	sStart = 0.f;
	sEnd = 100.f;

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
		if (!isSour)
			image->FrameRender(hdc, pos.x, pos.y, idx, 0, flip, anker);
		else
			image->SourFrameRenderWidth(hdc, pos.x, pos.y, idx, 0, sStart, sEnd, flip, anker);
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

void Animation::setPos(FPOINT pos, bool flip, bool anker)
{
	this->pos = pos;
	this->flip = flip;
	this->anker = anker;
}

void Animation::setSour(float start, float end)
{
	isSour = true;
	sStart = start;
	sEnd = end;
}

void Animation::setTask(initializer_list<pair<int, float>> lst)
{
	for (auto task : lst)
	{
		tasks.push_back(task);
	}
}

void Animation::setTask(std::vector<pair<int, float>>& lst)
{
	tasks.assign(lst.begin(), lst.end());
}
