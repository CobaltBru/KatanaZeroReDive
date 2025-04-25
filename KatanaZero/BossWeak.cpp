#include "BossWeak.h"
#include "Image.h"
#include "ImageManager.h"
#include "ScrollManager.h"
#include "RenderManager.h"
#include "CommonFunction.h"
#include "Collider.h"
#include "CollisionManager.h"

BossWeak::BossWeak()
	:Image(nullptr), frameX(0), bHit(false)
{
}

BossWeak::~BossWeak()
{
}

HRESULT BossWeak::Init(FPOINT InPos, FPOINT InTargetPos)
{
	Image = ImageManager::GetInstance()->FindImage("spr_psychboss_tentacle_idle_");
	TargetPos = InTargetPos;
	this->Pos = InPos;

	ObjectCollider = new Collider(this, EColliderType::Rect, {0.f,(float) -Image->GetFrameWidth()}, Image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale(), true, 0.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Projectile);
	ObjectCollider->SetPos(Pos);

	return S_OK;
}

void BossWeak::Update()
{
	if (!bHit && TargetPos.y < Pos.y + Image->GetFrameHeight() / 2)
		Pos.y -= 50.f * TimerManager::GetInstance()->GetDeltaTime();
	else if (bHit)
	{
		ObjectCollider->SetVisible(false);
		Pos.y += 200.f * TimerManager::GetInstance()->GetDeltaTime();
		if (OutOfScreen(Pos, Image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale(), Image->GetFrameHeight() * ScrollManager::GetInstance()->GetScale()))
			bDead = true;
	}

	FHitResult HitResult;
	if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
	{
		bHit = true;
	}

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void BossWeak::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, frameX, 0, false, true, ScrollManager::GetInstance()->GetScale());
}

void BossWeak::Release()
{
}
