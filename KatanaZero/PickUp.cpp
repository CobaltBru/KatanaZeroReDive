#include "PickUp.h"
#include "Image.h"
#include "ImageManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "ScrollManager.h"
#include "RenderManager.h"

PickUp::PickUp()
	:Image(nullptr), FrameX(0)
{
}

HRESULT PickUp::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey);
	Pos = InPos;

	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, false, 0.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Item);
	ObjectCollider->SetPos(Pos);
	bFlip = InFlip;

	return S_OK;
}

void PickUp::Update()
{
	// 충돌 정보
	FHitResult HitResult;

	// 내 콜라이더와 ECollisionGroup::Enemy에 있는 콜라이더들과 충돌처리
	if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
	{
		FrameX = 1;
	}
	else
		FrameX = 0;

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void PickUp::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, FrameX, 0, bFlip, true, Scale * ScrollManager::GetInstance()->GetScale());
}


void PickUp::Release()
{
}
