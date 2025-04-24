#include "PickUp.h"
#include "Image.h"
#include "ImageManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "ScrollManager.h"
#include "RenderManager.h"
#include "GPImage.h"
#include "LineManager.h"

PickUp::PickUp()
	:gpImage(nullptr), Image(nullptr), FrameX(0), IsVisible(true), Angle(-1.f), Speed(0.f), SpinAlngle(0.f)
{
}

HRESULT PickUp::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	gpImage = new GPImage();
	//gpImage->AddImage()

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
	LastPos = Pos;
	// �浹 ����
	FHitResult HitResult;
	// �� �ݶ��̴��� ECollisionGroup::Enemy�� �ִ� �ݶ��̴���� �浹ó��
	if (Angle < 0.f)
	{
		if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
			FrameX = 1;
		else
			FrameX = 0;
	}
	else
	{
		SpinAlngle += 30.f * TimerManager::GetInstance()->GetDeltaTime();
		Pos.x += cosf(DEG_TO_RAD(Angle)) * Speed;
		Pos.y += sinf(DEG_TO_RAD(Angle)) * Speed;

		Collision();
	}

	if(IsVisible)
		RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void PickUp::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	if(Angle < 0.f)
		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, FrameX, 0, bFlip, true, Scale * ScrollManager::GetInstance()->GetScale());
	else
	{
		Gdiplus::Graphics graphics(hdc);
		gpImage->Middle_RenderFrameAngle(&graphics, Pos + Scroll, FrameX, DEG_TO_RAD(SpinAlngle), bFlip);
	}	
}

void PickUp::Shoot(float InAngle,float InSpeed)
{
	Angle = InAngle;
	Speed = InSpeed;	
}

void PickUp::Collision()
{
	FrameX = 0;

	FLineResult Result;
	ZeroMemory(&Result, sizeof(FLineResult));

	bool bCollision = false;
	// ��
	if (LineManager::GetInstance()->CollisionLine(Pos, LastPos, Result, false, ObjectCollider->GetSize().y, true))
	{
		bCollision = true;
	}
	//  õ��
	else if (LineManager::GetInstance()->CollisionCeilingLine(Pos, LastPos, Result, ObjectCollider->GetSize().y))
	{
		bCollision = true;
	}
	// ��
	else if (LineManager::GetInstance()->CollisionWallLine(Pos, LastPos, Result, ObjectCollider->GetSize()))
	{
		bCollision = true;
	}

	FHitResult HitResult;
	if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Enemy))
	{
		bCollision = true;
	}


	// �ε��� �ݴ� ������ 45�� �������� min ~ max �Ÿ����� ���� �����ϰ� ���� �������� �ش�. 
	if (bCollision)
	{
		int a = 0;
	}
}

void PickUp::CreateBrokeParticle()
{
}


void PickUp::Release()
{
	if (gpImage != nullptr)
	{
		gpImage->Release();
		delete gpImage;
		gpImage = nullptr;
	}	
}
