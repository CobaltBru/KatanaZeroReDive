#include "Enemy.h"
#include "GPImage.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SnapShotManager.h"
#include "TaeKyungObject.h"
#include "RenderManager.h"
#include "ScrollManager.h"

Enemy::Enemy()
	:image(nullptr), eState(nullptr), currFrame(0), Speed(0.f), frameTimer(0.f), bFlip(false), bJump(false), dY(-10.f), 
	Gravity(0.1f), bFalling(true), bDown(false), dir(1), detectRange(0.f), attackRange(0.f), eType(EType::None), targetFloor(-1),
	bReachedTargetFloor(false), attackDuration(0.f), meleeAttackRange(0.f)
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
	ObjectRigidBody->SetMaxVelocity({ Speed , 400.f });
	
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
			img->Release();
			delete img;
			img = nullptr;
		}
	}
	images.clear();
}

void Enemy::Update()
{
	UpdateAnimation();
	if (eState)
	{
		eState->Update(*this);
		EnemyState* newState = eState->CheckTransition(this);
		if (newState && newState != eState)
		{
			ChangeState(newState);
		}
	}

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Enemy::Render(HDC hdc)
{
	if (image)
	{
		Gdiplus::Graphics graphics(hdc);
		image->Middle_RenderFrameScale(&graphics, Pos, currFrame, bFlip, 1.0f, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
	}
}

void Enemy::MakeSnapShot(void* out)
{
	EnemySnapShot* eSnapShot = static_cast<EnemySnapShot*>(out);
	eSnapShot->pos = this->GetPos();
	eSnapShot->ID = 0;
	eSnapShot->animFrame = currFrame;
	eSnapShot->isDead = this->bDead;
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
	currFrame = 0;
	if (image == images[(int)newImage]) return;
	image = images[(int)newImage];
}

bool Enemy::Detecting()
{
	if (SnapShotManager::GetInstance()->GetPlayer().empty()) return false;
	FPOINT playerPos = SnapShotManager::GetInstance()->GetPlayer().front()->GetPos();
	int playerFloor = SnapShotManager::GetInstance()->GetPlayer().front()->GetFloorIndex(g_FloorZones);
	int myFloor = this->GetFloorIndex(g_FloorZones);
	
	float dx = playerPos.x - Pos.x;
	float dist = fabs(dx);

	if (playerFloor == myFloor && ((dx > 0 && dir == 1) || (dx < 0 && dir == -1)))
	{
		return dist < detectRange;
	}

	return false;
}

bool Enemy::IsInAttackRange()
{
	if (SnapShotManager::GetInstance()->GetPlayer().empty()) return false;
	FPOINT playerPos = SnapShotManager::GetInstance()->GetPlayer().front()->GetPos();
	float dx = playerPos.x - Pos.x;
	float dist = fabs(dx);

	if ((dx > 0 && dir == 1) || (dx < 0 && dir == -1))
	{
		return dist < attackRange;
	}

	return false;
}

bool Enemy::IsInMeleeAttackRange()
{
	if (SnapShotManager::GetInstance()->GetPlayer().empty()) return false;
	FPOINT playerPos = SnapShotManager::GetInstance()->GetPlayer().front()->GetPos();
	float dx = playerPos.x - Pos.x;
	float dist = fabs(dx);

	if ((dx > 0 && dir == 1) || (dx < 0 && dir == -1))
	{
		return dist < meleeAttackRange;
	}

	return false;
}

bool Enemy::IsInSameFloor()
{
	auto player = SnapShotManager::GetInstance()->GetPlayer();
	if (player.empty()) return true;

	int myFloor = this->GetFloorIndex(g_FloorZones);
	int playerFloor = player.front()->GetFloorIndex(g_FloorZones);

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
