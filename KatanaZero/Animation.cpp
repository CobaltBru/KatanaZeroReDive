#include "Animation.h"
#include "Image.h"

void Animation::Init(Image* image, int frameX)
{
	this->image = image;
	this->frameX = frameX;
	timeIdx = 0;
	idx = 0;

	timer = 0;
	isOn = false;
}

void Animation::Update()
{
	if (isOn)
	{
		timer += TimerManager::GetInstance()->GetDeltaTime();
	}
}

void Animation::Render(HDC hdc)
{
}

void Animation::On()
{
	timeIdx = 0;
	idx = 0;
	timer = 0;
	isOn = true;
}

void Animation::Off()
{
	isOn = false;
}
