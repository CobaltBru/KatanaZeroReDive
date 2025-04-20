#include "Animation.h"
#include "Image.h"
#include "GPImage.h"
#include "RenderManager.h"
#include "CommonFunction.h"

void Animation::Init(Image* image, int frameX)
{
	imageFlag = false;
	this->image = image;
	this->frameX = frameX;
	anitaskIdx = 0;
	frameIdx = 0;
	flip = false;

	sStart = 0.f;
	sEnd = 1.f;

	timer = 0;
	isStart = false;
	isOn = false;
}

void Animation::Init(GPImage* image, int frameX)
{
	imageFlag = true;
	this->gpimage = image;
	this->frameX = frameX;
	anitaskIdx = 0;
	frameIdx = 0;
	flip = false;

	sStart = 0.f;
	sEnd = 1.f;

	timer = 0;
	isStart = false;
	isOn = false;
}

void Animation::Update()
{
	if (isStart)
	{
		timer += TimerManager::GetInstance()->GetDeltaTime();
		if (timer >= aniTasks[anitaskIdx].second)
		{
			anitaskIdx++;
			if (anitaskIdx >= aniTasks.size())anitaskIdx = 0;
			frameIdx = aniTasks[anitaskIdx].first;
			timer = 0.f;
		}
	}
	if (isMove)
	{
		moveTask.timer += TimerManager::GetInstance()->GetDeltaTime();
		float percent = min(moveTask.timer / moveTask.duration, 1.0f);

		float rate;
		if (moveTask.flag & (Move_SoftStart | Move_SoftEnd))rate = SoftStartEnd(percent);
		else if (moveTask.flag & Move_SoftStart) rate = SoftStart(percent);
		else if (moveTask.flag & Move_SoftEnd)rate = SoftEnd(percent);
		
		else rate = percent;

		moveTask.offset.x = moveTask.src.x + (moveTask.dest.x - moveTask.src.x) * rate;
		moveTask.offset.y = moveTask.src.y + (moveTask.dest.y - moveTask.src.y) * rate;

		if (moveTask.timer >= moveTask.duration)
		{
			if (moveTask.flag & Move_Loop)
			{
				moveTask.timer = 0.f;
			}
			else if (moveTask.flag & Move_Stop)
			{
				MoveOff();
			}
		}
	}
	
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
}

void Animation::Render(HDC hdc)
{
	if (isOn)
	{
		if (!imageFlag)
			image->SourFrameRenderWidth(hdc, pos.x + moveTask.offset.x, pos.y + moveTask.offset.y,
				frameIdx, 0, sStart, sEnd, flip, anker);
		else
		{
			Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hdc);
			if (!anker)
			{
				gpimage->RenderFrame(pGraphics, { pos.x + moveTask.offset.x, pos.y + moveTask.offset.y },
					frameIdx, flip);
			}
			else
			{
				gpimage->Middle_RenderFrame(pGraphics, { pos.x + moveTask.offset.x, pos.y + moveTask.offset.y },
					frameIdx, flip, 1.0f);
			}
			delete pGraphics;
		}
			
	}
	
}

void Animation::Start()
{
	anitaskIdx = 0;
	frameIdx = 0;
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


void Animation::MoveOn(FPOINT dest, float duration, int flag)
{
	isMove = true;
	moveTask.src = {0,0};
	moveTask.dest = dest;
	moveTask.offset = {0,0};
	moveTask.duration = duration;
	moveTask.flag = flag;
	moveTask.timer = 0;
}

void Animation::MoveOff()
{
	isMove = false;
}

void Animation::setPos(FPOINT pos, bool flip, bool anker)
{
	this->pos = pos;
	this->flip = flip;
	this->anker = anker;
}

void Animation::setSour(float start, float end)
{
	sStart = start;
	sEnd = end;
}

void Animation::setAniTask(initializer_list<pair<int, float>> lst)
{
	for (auto task : lst)
	{
		aniTasks.push_back(task);
	}
}

void Animation::setAniTask(std::vector<pair<int, float>>& lst)
{
	aniTasks.assign(lst.begin(), lst.end());
}

void Animation::Release()
{
	if (imageFlag == true)
	{
		delete gpimage;
		gpimage = nullptr;
	}
}
