#include "Effect.h"
#include "GPImage.h"
#include "SnapShotManager.h"
#include "SnapShot.h"
#include "TimerManager.h"
#include "ScrollManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GPImageManager.h"

int Effect::nextID = 0;

void Effect::UpdateFrame()
{
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	frameTimer += dt;
	if (frameTimer > frameduration)
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
	if (owner != nullptr)
	{
		pos.x = owner->GetPos().x;
		pos.y = owner->GetPos().y;
	}
	else
	{
		float dt = TimerManager::GetInstance()->GetDeltaTime();
		pos.x += speed * dt * cosf(angle);
		pos.y += speed * dt * sinf(angle);
	}
}

HRESULT Effect::Init()
{
	return E_FAIL;
}

HRESULT Effect::Init(string key, FPOINT start, FPOINT end, float speed, float frameduration, bool bFlip, bool bMove)
{
	this->start = start;
	this->end = end;
	this->frameduration = frameduration;
	this->currAnimKey = key;
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
	if (owner)
		owner = nullptr;
}

void Effect::Update()
{
	//프레임 증가
	if (!bActive) return;
	UpdateFrame();

	//움직임
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
	fxSnapShot->pos = this->pos;
	fxSnapShot->angle = this->angle;
	fxSnapShot->bFlip = this->bFlip;
	fxSnapShot->effectKey = this->currAnimKey;
	fxSnapShot->animFrame = this->currFrameX;
	fxSnapShot->id = this->GetID();
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
	bMove = false;
	currFrameX = 0;
	currFrameY = 0;
	this->owner = nullptr;
}

void Effect::Activefx(FPOINT pos, float angle, float speed, bool bFlip)
{
	this->pos = pos;
	this->bFlip = bFlip;
	this->angle = angle;
	this->speed = speed;
	bActive = true;
	bMove = true;
	currFrameX = 0;
	currFrameY = 0;
	this->owner = nullptr;
}

void Effect::Activefx(FPOINT pos, float angle, GameObject* owner, bool bFlip)
{
	this->pos = pos;
	this->owner = owner;
	this->bFlip = bFlip;
	this->angle = angle;
	bActive = true;
	bMove = true;
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
	this->fxImage = other.fxImage;   // 이것도 포인터라 얕복이야
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
	this->bActive = false;             // 새 인스턴스는 아직 활성화 안 됐으니까
	this->bMove = other.bMove;
	this->alpha = other.alpha;
	this->frameduration = other.frameduration;
	this->currAnimKey = other.currAnimKey;

	// sour 테스트용 변수도 복사
	this->offset = other.offset;
	this->offsetTimer = other.offsetTimer;
}
