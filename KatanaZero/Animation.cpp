#include "Animation.h"
#include "Image.h"
#include "GPImage.h"
#include "RenderManager.h"
#include "CommonFunction.h"

void Animation::Init(Image* image, int frameX, float scale)
{
	this->scale = scale;
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
	isMoveComplete = false;
	isComplete = false;
	loopflag = true;

	moveTask.dest = { 0,0 };
	moveTask.duration = 0.f;
	moveTask.flag = 0.f;
	moveTask.offset = { 0,0 };
	moveTask.src = { 0,0 };
	moveTask.timer = 0.f;
}

void Animation::Init(GPImage* image, int frameX, float scale)
{
	this->scale = scale;
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
	isMoveComplete = false;
	isComplete = false;
	loopflag = true;

	moveTask.dest = { 0,0 };
	moveTask.duration = 0.f;
	moveTask.flag = 0.f;
	moveTask.offset = { 0,0 };
	moveTask.src = { 0,0 };
	moveTask.timer = 0.f;
}

void Animation::Update()
{
	if (isStart)
	{
		timer += TimerManager::GetInstance()->GetDeltaTime();
		if (timer >= aniTasks[anitaskIdx].second)
		{
			anitaskIdx++;
			if (anitaskIdx >= aniTasks.size())
			{
				if (!loopflag)
				{
					isComplete = true;
					setFrame(frameX - 1);
					Stop();
					return;
				}
				else anitaskIdx = 0;
				
			}
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
				isMoveComplete = true;
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
				frameIdx, 0, sStart, sEnd, flip, anker, scale);
		else
		{
			Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hdc);
			if (!anker)
			{
				gpimage->RenderFrameScale(pGraphics, { pos.x + moveTask.offset.x, pos.y + moveTask.offset.y },
					frameIdx, flip , 1.0f, scale);
			}
			else
			{
				gpimage->Middle_RenderFrameScale(pGraphics, { pos.x + moveTask.offset.x, pos.y + moveTask.offset.y },
					frameIdx, flip, 1.0f, scale);
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
	isMoveComplete = false;
	if (flag & POS_Update)
	{
		pos.x += moveTask.offset.x;
		pos.y += moveTask.offset.y;
	}
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

void Animation::setloopFlag(bool flag)
{
	if (flag == true)
	{
		isComplete = false;
		loopflag = true;
	}
	else
	{
		isComplete = false;
		loopflag = false;
	}
}
