#include "RigidBody.h"
//#include "GameObject.h"
#include "CommonFunction.h"
#include "LineManager.h"
#include "TaeKyungObject.h"
#include "Collider.h"

RigidBody::RigidBody()
	: Owner(nullptr), Mass(1.f), Velocity({ 0.f,0.f }), Force({ 0.f,0.f }), FrictionCoefficient(0.f), MaxVelocity({ 100.f ,300.f }), bGravity(true), Gravity(9.8f),
	bGround(false), AccelerationAlpha({0.f,0.f}), Acceleration({0.f,0.f})
{
}
RigidBody::RigidBody(GameObject* InOwner)
	:Owner(InOwner), Mass(1.f), Velocity({ 0.f,0.f }), Force({ 0.f,0.f }), FrictionCoefficient(100.f), MaxVelocity({ 100.f ,300.f}), bGravity(true), Gravity(9.8f),
	bGround(false), AccelerationAlpha({ 0.f,0.f }), Acceleration({ 0.f,0.f })
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update()
{
	GravityUpdate();

	// 힘크기
	float ForceLength = GetLength(Force);
	if (ForceLength != 0.f)
	{
		// 힘방향
		Normalize(Force);

		//가속도의 크기
		float Accel = ForceLength / Mass;

		//가속도
		Acceleration = Force * Accel;		
	}
	
	Acceleration += AccelerationAlpha;

	//속도
	Velocity += Acceleration * TimerManager::GetInstance()->GetDeltaTime();

	//마찰
	FPOINT tempVelocity = Velocity;
	Normalize(tempVelocity);
	FPOINT friction = -tempVelocity * FrictionCoefficient * TimerManager::GetInstance()->GetDeltaTime();

	if (GetLength(Velocity) <= GetLength(friction))
		Velocity = { 0.f,0.f };
	else
		Velocity += friction;

	if (MaxVelocity.x < abs(Velocity.x))
	{
		Velocity.x = (Velocity.x / abs(Velocity.x))* MaxVelocity.x;
	}
	if (MaxVelocity.y < abs(Velocity.y))
	{
		Velocity.y = (Velocity.y / abs(Velocity.y)) * MaxVelocity.y;
	}

	Move();
	CollisionLine();

	Force = { 0.f,0.f };
	Acceleration = { 0.f,0.f };
	AccelerationAlpha = { 0.f,0.f };
}

void RigidBody::GravityUpdate()
{
	if (!bGravity)
		return;

	AccelerationAlpha = { 0.f,800.f };
}

void RigidBody::Move()
{
	float Speed = GetLength(Velocity);

	if (Speed != 0.f)
	{
		FPOINT Dir = Velocity;
		Normalize(Dir);

		FPOINT Pos = Owner->GetPos();

		Pos += Velocity * TimerManager::GetInstance()->GetDeltaTime();

		Owner->SetPos(Pos);
	}
}

void RigidBody::CollisionLine()
{
	if (!bGravity)
		return;

	TaeKyungObject* OwnerObj = static_cast<TaeKyungObject*>(Owner);
	FLineResult Result;
	if (LineManager::GetInstance()->CollisionWallLine(OwnerObj->GetPos(), Result, OwnerObj->GetCollider()->GetSize()))
	{
		FPOINT ObjPos = OwnerObj->GetPos();
		ObjPos.x = Result.OutPos.x;
		OwnerObj->SetPos(ObjPos);
	}
	// 땅
	if (LineManager::GetInstance()->CollisionLine(OwnerObj->GetPos(), Result, OwnerObj->GetCollider()->GetSize().y))
	{
		FPOINT ObjPos = OwnerObj->GetPos();
		ObjPos.y = Result.OutPos.y;
		OwnerObj->SetPos(ObjPos);

		bGround = true;
		Velocity.y = 0.f;
	}
	else
		bGround = false;
	//  천장
	if (!bGround && LineManager::GetInstance()->CollisionCeilingLine(OwnerObj->GetPos(), Result, OwnerObj->GetCollider()->GetSize().y))
	{
		FPOINT ObjPos = OwnerObj->GetPos();
		ObjPos.y = Result.OutPos.y;
		OwnerObj->SetPos(ObjPos);
	}
}
