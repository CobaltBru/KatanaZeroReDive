#include "SpriteAnimation.h"
#include "Image.h"
#include "GPImage.h"
#include "ScrollManager.h"

void SpriteAnimation::Init(Image* image, float frameTime, bool isLoop)
{
	ImageType = false;
	this->image = image;
	maxFrameX = image->GetMaxFrameX();
	pos = { 0,0 };
	flip = false;
	currentFrame = 0;
	this->frameTime = frameTime;

	timer = 0;
	this->isLoop = isLoop;
	isEffect = false;
}

void SpriteAnimation::Init(GPImage* image, float frameTime, bool isLoop)
{
	ImageType = true;
	this->gpImage = image;
	maxFrameX = gpImage->getMaxFrame();
	pos = { 0,0 };
	flip = false;
	currentFrame = 0;
	this->frameTime = frameTime;

	timer = 0;
	this->isLoop = isLoop;
	isEffect = false;
}

void SpriteAnimation::Update(FPOINT pos,bool flip, bool isEffect)
{
	this->pos = pos;
	this->flip = flip;
	this->isEffect = isEffect;
	/*if (ImageType)
	{
		this->pos.x -= gpImage->getFrameWidth() / 2.0f;
		this->pos.y -= gpImage->getFrameHeight() / 2.0f;
	}
	else
	{
		this->pos.x -= image->GetFrameWidth() / 2.0f;
		this->pos.y -= image->GetFrameHeight() / 2.0f;
	}*/
	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (timer >= frameTime)
	{
		timer = 0;
		currentFrame++;
		if (currentFrame >= maxFrameX)
		{
			if (isLoop)
			{
				currentFrame = 0;
			}
			else
			{
				currentFrame = maxFrameX - 1;
			}
		}
	}
}

void SpriteAnimation::Render(HDC hdc)
{
	float scale = ScrollManager::GetInstance()->GetScale();
	if (ImageType)
	{
		Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hdc);
		if (isEffect)
		{
			gpImage->Middle_RenderAll(pGraphics, pos, currentFrame, 0.f, flip, 1.0f, 2.f, 2.f, 5.0f, scale, scale);
		}
		else
		{
			gpImage->Middle_RenderFrameScale(pGraphics, pos, currentFrame, flip, 1.0f, scale, scale);
		}
		
		
		delete pGraphics;
	}
	else
	{
		image->FrameRender(hdc, pos.x, pos.y, currentFrame, 0, flip, true, scale);
	}
}

bool SpriteAnimation::isFinish()
{
	if (!isLoop)
	{
		if (currentFrame >= maxFrameX - 1)
		{
			return true;
		}
	}
	return false;
}

void SpriteAnimation::reset()
{
	currentFrame = 0;
	timer = 0.f;
	isEffect = false;
}
