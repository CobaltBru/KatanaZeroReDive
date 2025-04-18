#include "Effect.h"
#include "GPImage.h"
#include "SnapShotManager.h"
#include "SnapShot.h"
#include "TimerManager.h"
#include "ScrollManager.h"

void Effect::UpdateFrame()
{
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	frameTimer += dt;
	if (frameTimer > 0.1f)
	{
		currFrameX++;
		if (currFrameX >= fxImage->getMaxFrame())
		{
			currFrameX = 0;
			offset = 0;
			this->bActive = false;
		}
		frameTimer = 0.0f;
	}
	offsetTimer += dt;
	if (offsetTimer > 0.008f)
	{
		offset++;
		offsetTimer = 0.0f;
	}
}

HRESULT Effect::Init()
{
	return E_FAIL;
}

HRESULT Effect::Init(const wchar_t* AType, int maxframeX, int maxframeY)
{
	pos = { 0.0f, 0.0f };
	currFrameX = 0;
	currFrameY = 0;
	maxFrameX = maxframeX;
	maxFrameY = maxframeY;
	angle = 0.0f;
	frameTimer = 0.0f;
	bActive = false;
	bFlip = false;
	alpha = 1.0f;
	fxImage = new GPImage();
	fxImage->AddImage(AType, maxframeX, maxframeY);
	if (!fxImage)
		return E_FAIL;
	return S_OK;
}

void Effect::Release()
{
	if (fxImage)
	{
		delete fxImage;
		fxImage = nullptr;
	}
}

void Effect::Update()
{
	//프레임 증가
	if (!bActive) return;
	UpdateFrame();
}

void Effect::Render(HDC hdc)
{
	if (!bActive) return;
	if (fxImage)
	{
		Gdiplus::Graphics graphics(hdc);
		FPOINT scroll = ScrollManager::GetInstance()->GetScroll();
		fxImage->Middle_RenderFrameAngle(&graphics, { pos.x + scroll.x, pos.y + scroll.y }, currFrameX, angle, bFlip, alpha);
		fxImage->SourRender(&graphics, { pos.x + scroll.x, pos.y + 100.0f + scroll.y }, offset, 0, bFlip, alpha, 0.01f, 0.01f, 0.01f, 1.5f, 1.5f, 0.5f);
	}
}

void Effect::MakeSnapShot(void* out)
{
	EffectSnapShot* fxSnapShot = static_cast<EffectSnapShot*>(out);
	fxSnapShot->pos = this->pos;
	fxSnapShot->animFrame = this->currFrameX;
	fxSnapShot->isActive = bActive;
}

void Effect::ApplySnapShot(const EffectSnapShot& fxSnapShot)
{
	this->pos = fxSnapShot.pos;
	this->bActive = fxSnapShot.isActive;
	this->currFrameX = fxSnapShot.animFrame;
}

void Effect::Activefx(FPOINT pos, float angle, bool bFlip)
{
	this->pos = pos;
	this->bFlip = bFlip;
	this->angle = angle;
	bActive = true;
	currFrameX = 0;
	currFrameY = 0;
}
