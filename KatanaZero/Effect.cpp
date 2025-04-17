#include "Effect.h"
#include "GPImage.h"
#include "SnapShotManager.h"
#include "SnapShot.h"
#include "TimerManager.h"

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
			this->bActive = false;
		}
		frameTimer = 0.0f;
	}
}

HRESULT Effect::Init()
{
	return E_FAIL;
}

HRESULT Effect::Init(const wchar_t* AType)
{
	pos = { 0.0f, 0.0f };
	currFrameX = 0;
	currFrameY = 0;
	angle = 0.0f;
	frameTimer = 0.0f;
	bActive = false;
	alpha = 1.0f;
	fxImage = new GPImage();
	fxImage->AddImage(AType);
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
		fxImage->Middle_RenderFrameAngle(&graphics, pos, currFrameX, angle, bFlip, alpha);
	}
}

void Effect::MakeSnapShot(void* out)
{
	EffectSnapShot* fxSnapShot = static_cast<EffectSnapShot*>(out);
	fxSnapShot->pos = this->pos;
	fxSnapShot->animFrame = this->currFrameX;
	fxSnapShot->isActive = bActive;
	fxSnapShot->ID = 0;
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
