#include "PickUp.h"
#include "Image.h"
#include "ImageManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "ScrollManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "GPImage.h"
#include "LineManager.h"
#include "CommonFunction.h"
#include "Broken.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "SnapShotManager.h"

PickUp::PickUp()
	:gpImage(nullptr), Image(nullptr), IsVisible(true), Angle(-1.f), Speed(0.f), SpinAlngle(0.f)
{
}

HRESULT PickUp::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	ImageKey = InImageKey;
	wstring str = Utf8ToWstring("Image/Bottle/" + InImageKey + ".png");
	gpImage = new GPImage();
	gpImage->AddImage(str.c_str(),1,1);
	
	Image = ImageManager::GetInstance()->FindImage(InImageKey);
	Pos = InPos;

	ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 0.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Item);
	ObjectCollider->SetPos(Pos);
	bFlip = InFlip;

	return S_OK;
}

void PickUp::Update()
{
	LastPos = Pos;
	
	if (!IsVisible)
		ObjectCollider->SetVisible(false);
	else
		ObjectCollider->SetVisible(true);

	if (Angle >= 0.f)
	{
		SpinAlngle += 1000.f * TimerManager::GetInstance()->GetDeltaTime();
		Pos.x += cosf(DEG_TO_RAD(Angle)) * Speed;
		Pos.y += sinf(DEG_TO_RAD(Angle)) * Speed;

		Collision();
	}

	if (IsVisible)
		RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void PickUp::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	if (Angle < 0.f)
		Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0, bFlip, true, Scale);
	else
	{
		Gdiplus::Graphics graphics(hdc);
		gpImage->Middle_RenderFrameAngle(&graphics, Pos + Scroll, 0, SpinAlngle, bFlip);
	}
}

void PickUp::Shoot(FPOINT InPos,float InAngle, float InSpeed)
{
	Pos = InPos;
	Angle = InAngle;
	Speed = InSpeed;
	IsVisible = true;
	ObjectCollider->SetSize(ObjectCollider->GetSize() / 2);
}

void PickUp::Collision()
{
	FPOINT NormalVector = { 0.f,0.f };

	FLineResult Result;
	ZeroMemory(&Result, sizeof(FLineResult));

	bool bCollision = false;
	FHitResult HitResult;
	if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Enemy))
	{
		bCollision = true;
		NormalVector = { HitResult.HitCollision->GetPos().x - LastPos.x,HitResult.HitCollision->GetPos().y - LastPos.y};

		float speed = sqrt(Pos.x * Pos.x + Pos.y * Pos.y);

		//EffectManager::GetInstance()->Activefx("hitslash", Pos, 0.f, false);
		//EffectManager::GetInstance()->Activefx("normalslash",Pos, Angle, SnapShotManager::GetInstance()->GetPlayer(), bFlip);
	}
	// 땅
	else if (LineManager::GetInstance()->CollisionLine(Pos, LastPos, Result, false, ObjectCollider->GetSize().y, true))
	{
		bCollision = true;
		if(Result.IsDiagonalLine)
			NormalVector = { Result.OutPos.x - LastPos.x,Result.OutPos.y - LastPos.y };
		else
			NormalVector = { Result.OutPos.x,Result.OutPos.y - LastPos.y };
	}
	//  천장
	else if (LineManager::GetInstance()->CollisionCeilingLine(Pos, LastPos, Result, ObjectCollider->GetSize().y))
	{
		bCollision = true;
		NormalVector = { Result.OutPos.x,Result.OutPos.y - LastPos.y};
	}
	// 벽
	else if (LineManager::GetInstance()->CollisionWallLine(Pos, LastPos, Result, ObjectCollider->GetSize()))
	{
		bCollision = true;
		NormalVector = { Result.OutPos.x - LastPos.x,Result.OutPos.y};
	}


	// 부딪힌 반대 방향의 45도 각도에서 min ~ max 거리에서 랜덤 생성하고 힘도 랜덤으로 준다. 
	if (bCollision)
	{
	

		bDead = true;
		SoundManager::GetInstance()->PlaySounds("sound_enemy_death_bottle", EChannelType::Effect);

		Normalize(NormalVector);

		float RadianAngle = RAD_TO_DEG(atan2f(-NormalVector.y, -NormalVector.x));
		if (RadianAngle < 0.f)
			RadianAngle += 360.f;		

		Angle += 180.f;
		if (Angle >= 360.f)
			Angle -= 360.f;
		
		const float Range = 90.f;

		const int MinRangeAngle = RadianAngle - (Range / 2);
		const int MaxRangeAngle = RadianAngle + (Range / 2);

		const int newA = MaxRangeAngle - Angle;

		uniform_int_distribution<int> uidAngle(MinRangeAngle + newA, MaxRangeAngle + newA);
		uniform_int_distribution<int> uidMass(2, 5);
		uniform_int_distribution<int> uidDistance(10, 20);
		uniform_int_distribution<int> uidForce(200, 2000);

		for (int i = 0; i < 20; ++i)
		{
			const float newAngle = uidAngle(dre);
			const float newMass = uidMass(dre);
			const float newDistance = uidDistance(dre);
			const float NewForce = uidForce(dre);

			FPOINT XY = { cosf(DEG_TO_RAD(newAngle)),sinf(DEG_TO_RAD(newAngle)) };
			Normalize(XY);

			Broken* newBroken = new Broken();
			newBroken->Init(ImageKey, Pos + (XY * newDistance), XY * NewForce, newMass);
			newBroken->SetScale(Scale);
			ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, newBroken);
		}
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
