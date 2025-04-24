#include "Effect.h"
#include "GPImage.h"
#include "SnapShotManager.h"
#include "SnapShot.h"
#include "TimerManager.h"
#include "ScrollManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GPImageManager.h"

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

void Effect::Move()
{
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	pos.x += speed * dt * cosf(angle);
	pos.y += speed * dt * sinf(angle);
}

HRESULT Effect::Init()
{
	return E_FAIL;
}

HRESULT Effect::Init(const wchar_t* AType, int maxframeX, int maxframeY, bool bFlip, bool bMove)
{
	pos = { 0.0f, 0.0f };
	start = { 0.0f, 0.0f };
	end = { 0.0f, 0.0f };
	currFrameX = 0;
	currFrameY = 0;
	maxFrameX = maxframeX;
	maxFrameY = maxframeY;
	speed = 0.0f;
	angle = 0.0f;
	frameTimer = 0.0f;
	bActive = false;
	this->bFlip = bFlip;
	this->bMove = bMove;
	alpha = 1.0f;
	fxImage = new GPImage();
	fxImage->AddImage(AType, maxframeX, maxframeY);
	if (!fxImage)
		return E_FAIL;
	return S_OK;
}

HRESULT Effect::Init(const wchar_t* AType, int maxframeX, int maxframeY, FPOINT start, FPOINT end, float speed, bool bFlip, bool bMove)
{
	this->start = start;
	this->end = end;
	pos = this->start;
	currFrameX = 0;
	currFrameY = 0;
	maxFrameX = maxframeX;
	maxFrameY = maxframeY;
	this->speed = speed;
	angle = atan2f(end.y - start.y, end.x - start.x) * 180 / M_PI; // degree(radian x)
	frameTimer = 0.0f;
	bActive = false;
	this->bFlip = bFlip;
	this->bMove = bMove;
	alpha = 1.0f;
	fxImage = new GPImage();
	fxImage->AddImage(AType, maxframeX, maxframeY);
	if (!fxImage)
		return E_FAIL;
	return S_OK;
}

HRESULT Effect::Init(string key, FPOINT start, FPOINT end, float speed, bool bFlip, bool bMove)
{
	this->start = start;
	this->end = end;
	pos = this->start;
	currFrameX = 0;
	currFrameY = 0;
	fxImage = GPImageManager::GetInstance()->FindImage(key);
	if (fxImage == nullptr) return E_FAIL;
	maxFrameX = fxImage->getMaxFrame();
	maxFrameY = 1;
	this->speed = speed;
	angle = atan2f(end.y - start.y, end.x - start.x) * 180 / M_PI;
	frameTimer = 0.f;
	bActive = false;
	this->bFlip = bFlip;
	this->bMove = bMove;
	alpha = 1.f;
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
	//������ ����
	if (!bActive) return;
	UpdateFrame();
	//������
	if (!bMove) return;
	Move();
}

void Effect::Render(HDC hdc)
{
	if (!bActive) return;
	if (fxImage)
	{
		Gdiplus::Graphics graphics(hdc);
		FPOINT scroll = ScrollManager::GetInstance()->GetScroll();
		
		fxImage->Middle_RenderAll(&graphics, { pos.x + scroll.x, pos.y + scroll.y }, currFrameX, angle, bFlip, alpha, 1.f, 1.f, 1.f,
			ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());

		//fxImage->SourRender(&graphics, { pos.x + scroll.x, pos.y + 100.0f + scroll.y }, offset, 0, bFlip, alpha, 0.01f, 0.01f, 0.01f, 1.5f, 1.5f, 0.5f);
	}
}

void Effect::MakeSnapShot(void* out)
{
	EffectSnapShot* fxSnapShot = static_cast<EffectSnapShot*>(out);
	fxSnapShot->startpos = this->start;
	fxSnapShot->destpos = this->end;
	fxSnapShot->angle = this->angle;
	fxSnapShot->bFlip = this->bFlip;
	fxSnapShot->effectKey = 
	fxSnapShot->animFrame = this->currFrameX;
	fxSnapShot->isActive = bActive;
}

void Effect::ApplySnapShot(const EffectSnapShot& fxSnapShot)
{
	
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

void Effect::Activefx(FPOINT pos, FPOINT dest, float speed, bool bFlip)
{
	this->pos = pos;
	this->end = dest;
	this->angle = atan2f(dest.y - pos.y, dest.x - pos.x) * 180 / M_PI;
	this->speed = speed;
	this->bFlip = bFlip;
	bMove = true;
	bActive = true;
	currFrameX = 0;
	currFrameY = 0;

}

void Effect::Activefx(FPOINT pos, float angle, float speed, float scale, bool bFlip)
{
}


Effect::Effect(const Effect& other)
{
	this->fxImage = other.fxImage;   // �̰͵� �����Ͷ� �躹�̾�
	this->start = other.start;
	this->end = other.end;
	this->pos = other.pos;
	this->currFrameX = other.currFrameX;
	this->currFrameY = other.currFrameY;
	this->maxFrameX = other.maxFrameX;
	this->maxFrameY = other.maxFrameY;
	this->angle = other.angle;
	this->frameTimer = other.frameTimer;
	this->speed = other.speed;
	this->bFlip = other.bFlip;
	this->bActive = false;             // �� �ν��Ͻ��� ���� Ȱ��ȭ �� �����ϱ�
	this->bMove = other.bMove;
	this->alpha = other.alpha;

	// sour �׽�Ʈ�� ������ ����
	this->offset = other.offset;
	this->offsetTimer = other.offsetTimer;
}
