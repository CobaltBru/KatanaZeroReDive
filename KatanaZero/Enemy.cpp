#include "Enemy.h"
#include "GPImage.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SnapShotManager.h"
#include "TaeKyungObject.h"
#include "RenderManager.h"

Enemy::Enemy()
	:image(nullptr), eState(nullptr), currFrame(0), Speed(0.f), frameTimer(0.f), bFlip(false), bJump(false), dY(-10.f), 
	Gravity(0.1f), bFalling(true), bDown(false), dir(1), detectRange(0.f), attackRange(0.f), eType(EType::None)
{
}

Enemy::~Enemy()
{
}

HRESULT Enemy::Init(FPOINT InPos)
{
	return E_NOTIMPL;
}

void Enemy::InitImages()
{
}

void Enemy::Release()
{
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
	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
}

void Enemy::Update()
{
	if (eState)
	{
		eState->Update(*this);
		EnemyState* newState = eState->CheckTransition(this);
		if (newState && newState != eState)
		{
			ChangeState(newState);
		}
	}
		
	UpdateAnimation();

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Enemy::Render(HDC hdc)
{
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
	return images[(int)EImageType::Attack]->getMaxFrame();
}

void Enemy::UpdateAnimation()
{
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
	currFrame = 0;
	if (image == images[(int)newImage]) return;
	image = images[(int)newImage];
}

bool Enemy::Detecting()
{
	return false;
}

bool Enemy::IsInAttackRange()
{
	float dist = fabs(Pos.x - SnapShotManager::GetInstance()->GetPlayer().front()->GetPos().x);
	return dist < attackRange;
}
