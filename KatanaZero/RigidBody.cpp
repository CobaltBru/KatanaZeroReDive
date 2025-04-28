#include "RigidBody.h"
#include "GameObject.h"
#include "CommonFunction.h"
#include "Collider.h"
#include "LineManager.h"

RigidBody::RigidBody()
	: Owner(nullptr), Mass(0.f), Velocity({ 0.f,0.f }), Force({ 0.f,0.f }), FrictionCoefficient(0.f), MaxVelocity({ 0.f,0.f }), bGravity(false), Gravity(0.f),
	bGround(false), AccelerationAlpha({ 0.f,0.f }), Acceleration({ 0.f,0.f }), bDown(false), bDiagonalLine(false), Elasticity(0.f), SaveAccelerationAlpha({ 0.f,0.f })
{
	CanSlowSystem = true;
	ZeroMemory(&Result, sizeof(FLineResult));
}
RigidBody::RigidBody(GameObject* InOwner, bool CanSlowSystem)
	:Owner(InOwner), Mass(1.f), Velocity({ 0.f,0.f }), Force({ 0.f,0.f }), FrictionCoefficient(50.f), MaxVelocity({ 200.f ,600.f }), bGravity(true), Gravity(9.8f),
	bGround(false), AccelerationAlpha({ 0.f,0.f }), Acceleration({ 0.f,0.f }), bDown(false), bDiagonalLine(false), Elasticity(0.f), SaveAccelerationAlpha({ 0.f,800.f })
{
	this->CanSlowSystem = CanSlowSystem;
	ZeroMemory(&Result, sizeof(FLineResult));
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
	Velocity += Acceleration * TimerManager::GetInstance()->GetDeltaTime(CanSlowSystem);

	//마찰
	FPOINT tempVelocity = Velocity;
	Normalize(tempVelocity);
	FPOINT friction = -tempVelocity * FrictionCoefficient * TimerManager::GetInstance()->GetDeltaTime(CanSlowSystem);

	if (GetLength(Velocity) <= GetLength(friction))
		Velocity = { 0.f,0.f };
	else
		Velocity += friction;

	if (MaxVelocity.x < abs(Velocity.x))
	{
		Velocity.x = (Velocity.x / abs(Velocity.x)) * MaxVelocity.x;
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
	if (!bGravity || bGround)
		return;

	AccelerationAlpha = SaveAccelerationAlpha;
}

void RigidBody::Move()
{
	float Speed = GetLength(Velocity);

	if (Speed != 0.f)
	{
		FPOINT Dir = Velocity;
		Normalize(Dir);

		FPOINT Pos = Owner->GetPos();

		Pos += Velocity * TimerManager::GetInstance()->GetDeltaTime(CanSlowSystem);

		Owner->SetPos(Pos);
	}
}

void RigidBody::CollisionLine()
{
	if (!bGravity || Owner->GetCollider()==nullptr)
		return;

	// 사실 각 라인 충돌이 필요한 클래스에서 충돌됐을 때의 로직을 맡기는게 맞는데, 일단 모두 관리하게끔 여기서 관리함.

	ZeroMemory(&Result, sizeof(FLineResult));
	Result.IsDiagonalLine = bDiagonalLine;
	Result.LineType = ELineType::End;
	// 땅
	if (Velocity.y > 0.f && LineManager::GetInstance()->CollisionLine(Owner->GetPos(), Owner->GetLastPos(), Result, bGround, Owner->GetCollider()->GetSize().y, bDown))
	{
		FPOINT ObjPos = Owner->GetPos();
		ObjPos.y = Result.OutPos.y;
		Owner->SetPos(ObjPos);

		bDiagonalLine = Result.IsDiagonalLine;

		bGround = true;


		//Velocity.y = 0.f;
		Velocity.y = -Velocity.y * Elasticity;

		if (abs(Velocity.y) < 1.f)
			Velocity.y = 0.f;
	}
	else
	{
		// 땅에 이미 있을 때				// 대각선 처리를 어떻게 할까 일단 땅인걸 인자로 받아야하는지 부터 생각해보자.
		if (bGround && Velocity.y == 0.f && LineManager::GetInstance()->CollisionLine(Owner->GetPos(), Owner->GetLastPos(), Result, bGround, Owner->GetCollider()->GetSize().y, bDown))
		{
			FPOINT ObjPos = Owner->GetPos();
			ObjPos.y = Result.OutPos.y;
			Owner->SetPos(ObjPos);

			bDiagonalLine = Result.IsDiagonalLine;
		}
		else
			bGround = false;
	}


	//  천장
	if (!bGround && LineManager::GetInstance()->CollisionCeilingLine(Owner->GetPos(), Owner->GetLastPos(), Result, Owner->GetCollider()->GetSize().y))
	{
		FPOINT ObjPos = Owner->GetPos();
		//ObjPos.y = Result.OutPos.y;
		//Owner->SetPos(ObjPos);

		Velocity.y = -Velocity.y * Elasticity;

		if (abs(Velocity.y) < 1.f)
			Velocity.y = 0.f;
	}
	// 벽
	else if (LineManager::GetInstance()->CollisionWallLine(Owner->GetPos(), Owner->GetLastPos(), Result, Owner->GetCollider()->GetSize()))
	{
		FPOINT ObjPos = Owner->GetPos();
		ObjPos.x = Result.OutPos.x;
		Owner->SetPos(ObjPos);

		Velocity.x = -Velocity.x * Elasticity;

		if (abs(Velocity.x) < 1.f)
			Velocity.x = 0.f;
	}


}
