#include "Enemy.h"
#include "GPImage.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SnapShotManager.h"
#include "TaeKyungObject.h"
#include "RenderManager.h"
#include "ScrollManager.h"
#include "CommonFunction.h"

Enemy::Enemy()
	:image(nullptr), eState(nullptr), currFrame(0), Speed(0.f), frameTimer(0.f), bFlip(false), bJump(false), dY(-10.f), 
	Gravity(0.1f), bFalling(true), bDown(false), dir(1), detectRange(0.f), attackRange(0.f), eType(EType::None), targetFloor(-1),
	bReachedTargetFloor(false), attackDuration(0.f), meleeAttackRange(0.f), currAnimKey(""), HitAngle(0.f)
{
}

Enemy::~Enemy()
{
}

HRESULT Enemy::Init(FPOINT InPos)
{
	return E_NOTIMPL;
}

HRESULT Enemy::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
	return E_NOTIMPL;
}

void Enemy::InitImages()
{
}

void Enemy::InitRigidBodySetting()
{
	if (ObjectRigidBody == nullptr)
		return;

	// 탄성 적용안함  0 ~ 1 사이
	ObjectRigidBody->SetElasticity(0.3f);

	// 중력 적용
	ObjectRigidBody->SetGravityVisible(true);
	// 저항 
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
	//무게
	ObjectRigidBody->SetMass(1.f);
	//최대 속도
	ObjectRigidBody->SetMaxVelocity({ Speed * 2.f , 400.f });
	
	//마찰
	ObjectRigidBody->SetFriction(50.f);

	//밑으로 내려가고 싶을 때
	//ObjectRigidBody->SetDown(true);
}

void Enemy::Release()
{
	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
	if (eState)
	{
		delete eState;
		eState = nullptr;
	}
	image = nullptr;
	for (auto& img : images)
	{
		if (img)
		{
			img = nullptr;
		}
	}
	images.clear();

	if (root)
	{
		delete root;
		root = nullptr;
	}
}

void Enemy::Update()
{
	LastPos = Pos;
	/*switch (eType)
	{
	case EType::Gangster:
		AttackCollider->SetPivot({ meleeAttackRange / 2.f * dir, 0.f });
		break;
	case EType::Grunt:
	case EType::Pomp:
		AttackCollider->SetPivot({ attackRange / 2.f * dir, 0.f });
		break;
	}
	UpdateAnimation();
	if (eState)
	{
		eState->Update(*this);
		EnemyState* newState = eState->CheckTransition(this);
		if (newState && newState != eState)
		{
			ChangeState(newState);
		}
	}*/
	if (!bDead)
	{
		ObjectRigidBody->SetVelocity({ 0.f, 0.f });
		AttackCollider->SetPivot({ AttackCollider->GetPivot().x * dir, AttackCollider->GetPivot().y });
		float dt = TimerManager::GetInstance()->GetDeltaTime();
		attackTimer -= dt;
		if (attackTimer <= 0.f)
		{
			attackTimer = 0.f;
		}
		NodeStatus nodeStatus = root->tick();
	}

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Enemy::Render(HDC hdc)
{
	if (image)
	{
		Gdiplus::Graphics graphics(hdc);
		image->Middle_RenderFrameScale(&graphics, { Pos.x + ScrollManager::GetInstance()->GetScroll().x, Pos.y + ScrollManager::GetInstance()->GetScroll().y }, currFrame, bFlip, 1.0f, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
	}
}

void Enemy::MakeSnapShot(void* out)
{
	EnemySnapShot* eSnapShot = static_cast<EnemySnapShot*>(out);
	eSnapShot->pos = this->GetPos();
	eSnapShot->animKey = currAnimKey;
	eSnapShot->animFrame = currFrame;
	eSnapShot->isDead = this->bDead;
	eSnapShot->bFlip = this->bFlip;
}

int Enemy::GetMaxAttackFrame() const
{
	if (image == nullptr) return 0;
	return images[(int)EImageType::Attack]->getMaxFrame();
}

void Enemy::UpdateAnimation()
{
	if (image == nullptr) return;
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	frameTimer += dt;
	if (frameTimer > 0.1f)
	{
		currFrame++;
		if (currFrame >= image->getMaxFrame())
		{
			currFrame = 0;
		}
		frameTimer = 0.f;
	}
	if (dir == -1)
	{
		bFlip = true;
	}
	else if(dir == 1)
	{
		bFlip = false;
	}
}

void Enemy::ChangeState(EnemyState* newState)
{
	if (eState)
	{
		eState->Exit(*this);
		delete eState;
	}
	eState = newState;
	if (eState)
	{
		eState->Enter(*this);
	}
		
}

void Enemy::ChangeAnimation(EImageType newImage)
{
	if (image == nullptr) return;
	
	if (image == images[(int)newImage]) return;
	
	currFrame = 0;
	image = images[(int)newImage];
	SetAnimKey(newImage);
}

bool Enemy::Detecting()
{
	if (!SnapShotManager::GetInstance()->GetPlayer()) return false;
	FHitResult Result;
	if (CollisionManager::GetInstance()->LineTraceByObject(Result, ECollisionGroup::Player, this->Pos, { Pos.x + detectRange * dir, Pos.y }))
	{
		return true;
	}
	return false;
}

bool Enemy::IsInAttackRange()
{
	if (!SnapShotManager::GetInstance()->GetPlayer()) return false;
	FHitResult Result; // 충돌 대상
	if (CollisionManager::GetInstance()->LineTraceByObject(Result, ECollisionGroup::Player, this->Pos, { Pos.x + attackRange * dir, Pos.y }))
	{
		return true;
	}
		
	return false;
}

bool Enemy::IsInMeleeAttackRange()
{
	if (!SnapShotManager::GetInstance()->GetPlayer()) return false;
	FHitResult Result;
	if (CollisionManager::GetInstance()->LineTraceByObject(Result, ECollisionGroup::Player, this->Pos, { Pos.x + meleeAttackRange * dir, Pos.y }))
		return true;

	return false;
}

bool Enemy::IsInSameFloor()
{
	auto player = SnapShotManager::GetInstance()->GetPlayer();
	if (!player) return true;

	int myFloor = this->GetFloorIndex(g_FloorZones);
	int playerFloor = player->GetFloorIndex(g_FloorZones);

	return myFloor == playerFloor;
}

bool Enemy::IsOnDownLine()
{
	return GetRigidBody()->GetResult().LineType == ELineType::DownLine;
}

bool Enemy::IsHitted()
{
	if (ObjectCollider->IsHitted()) return true;
	return false;
}

void Enemy::Collision()
{
	if (!bCanAttack) return;
	
	FHitResult HitResult;
	if (CollisionManager::GetInstance()->CollisionAABB(AttackCollider, HitResult, ECollisionGroup::Player))
	{
		HitResult.HitCollision->SetHit(true);
		HitResult.HitCollision->GetOwner();
		FPOINT pos;
		pos.x = HitResult.HitCollision->GetPos().x - AttackCollider->GetPos().x;
		pos.y = HitResult.HitCollision->GetPos().y - AttackCollider->GetPos().y;
		Normalize(pos);
		HitResult.HitCollision->GetOwner()->GetRigidBody()->AddVelocity(pos * 500.f);
	}
}
