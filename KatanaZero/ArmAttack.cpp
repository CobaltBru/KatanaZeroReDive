#include "ArmAttack.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "Image.h"
#include "ImageManager.h"
#include "ScrollManager.h"
#include "LineManager.h"
#include "SoundManager.h"

ArmAttack::ArmAttack()
	:Target(nullptr), Image(nullptr), State(EArmState::Start), CurrentTime(0.f), AttackCount(0), bFinish(false)
{
}

HRESULT ArmAttack::Init()
{
	Image = ImageManager::GetInstance()->FindImage("spr_psychboss_stabber_0");

	ObjectCollider = new Collider(this, EColliderType::Rect, {}, { (float)Image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale(),
		(float)Image->GetFrameHeight() * ScrollManager::GetInstance()->GetScale() }, true, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Projectile);

	Target = CollisionManager::GetInstance()->GetTargetCollider(ECollisionGroup::Player)->GetOwner();
	Pos.x = 0.f;
	Pos.y = -Image->GetFrameHeight();
	return S_OK;
}

void ArmAttack::Update()
{
	switch (State)
	{
	case EArmState::Start:
		Pos.x += (Target->GetPos().x - Pos.x) * 5 * TimerManager::GetInstance()->GetDeltaTime();
		Pos.y += 100 * TimerManager::GetInstance()->GetDeltaTime();
		CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
		if (CurrentTime >= 2.f)
		{
			CurrentTime = 0.f;
			State = EArmState::Attack;
		}
		break;
	case EArmState::Attack:
		Pos.y += 2000 * TimerManager::GetInstance()->GetDeltaTime();
		
		FHitResult HitResult;
		/*if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
		{
			HitResult.HitCollision->GetOwner()->SetDead(true);
		}*/

		
		FLineResult Result;
		if (LineManager::GetInstance()->CollisionLine(Pos, LastPos, Result, false, ObjectCollider->GetSize().y, true))
		{
			++AttackCount;
			ScrollManager::GetInstance()->CameraShake(5.f, 0.5f);
			SoundManager::GetInstance()->PlaySounds("sound_cutblack", EChannelType::Effect);
			if (AttackCount > 3)
			{
				State = EArmState::High;
			}
			else
			{
				State = EArmState::Follow;
				uniform_int_distribution<int> uidTime(2, 4);
				FollowTime = uidTime(dre) / 10.f;
			}
		}
		break;
	case EArmState::Follow:
		Pos.x += (Target->GetPos().x - Pos.x) * 5 * TimerManager::GetInstance()->GetDeltaTime();
		Pos.y -= 1500 * TimerManager::GetInstance()->GetDeltaTime();
		CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
		if (CurrentTime >= FollowTime)
		{
			CurrentTime = 0.f;
			State = EArmState::Cooldown;
		}
		break;
	case EArmState::High:
		Pos.y -= 1500 * TimerManager::GetInstance()->GetDeltaTime();
		if (0 > Pos.y + Image->GetFrameHeight() + 150.f)
		{
			State = EArmState::End;
			bFinish = true;
		}
		break;
	case EArmState::Cooldown:
		CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
		if (CurrentTime >= 0.2f)
		{
			CurrentTime = 0.f;
			State = EArmState::Attack;
		}
		break;
	case EArmState::End:
		break;
	default:
		break;
	}





	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void ArmAttack::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0, false, true, ScrollManager::GetInstance()->GetScale());
}

void ArmAttack::Release()
{
}
