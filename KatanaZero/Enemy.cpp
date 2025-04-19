#include "Enemy.h"
#include "GPImage.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SnapShotManager.h"

Enemy::Enemy()
	:image(nullptr), ObjectCollider(nullptr), ObjectRigidBody(nullptr), eState(nullptr), currFrame(0), Speed(0.f), frameTimer(0.f), bFlip(false)
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
}

void Enemy::Render(HDC hdc)
{
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
		eState->Exit(*this);
	eState = newState;
	if (eState)
		eState->Enter(*this);
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
