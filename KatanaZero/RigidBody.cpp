#include "RigidBody.h"
//#include "GameObject.h"
#include "CommonFunction.h"
#include "LineManager.h"
#include "TaeKyungObject.h"
#include "SimpleObject.h"
#include "Collider.h"

RigidBody::RigidBody()
	: Owner(nullptr), Mass(0.f), Velocity({ 0.f,0.f }), Force({ 0.f,0.f }), FrictionCoefficient(0.f), MaxVelocity({ 0.f,0.f }), bGravity(false), Gravity(0.f),
	bGround(false), AccelerationAlpha({ 0.f,0.f }), Acceleration({ 0.f,0.f }), bDown(false), bDiagonalLine(false), Elasticity(1.f)
{
}
RigidBody::RigidBody(GameObject* InOwner)
	:Owner(InOwner), Mass(1.f), Velocity({ 0.f,0.f }), Force({ 0.f,0.f }), FrictionCoefficient(50.f), MaxVelocity({ 200.f ,300.f }), bGravity(true), Gravity(9.8f),
	bGround(false), AccelerationAlpha({ 0.f,0.f }), Acceleration({ 0.f,0.f }), bDown(false), bDiagonalLine(false), Elasticity(0.3f)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update()
{
	GravityUpdate();

	// ��ũ��
	float ForceLength = GetLength(Force);
	if (ForceLength != 0.f)
	{
		// ������
		Normalize(Force);

		//���ӵ��� ũ��
		float Accel = ForceLength / Mass;

		//���ӵ�
		Acceleration = Force * Accel;
	}

	Acceleration += AccelerationAlpha;

	//�ӵ�
	Velocity += Acceleration * TimerManager::GetInstance()->GetDeltaTime();

	//����
	FPOINT tempVelocity = Velocity;
	Normalize(tempVelocity);
	FPOINT friction = -tempVelocity * FrictionCoefficient * TimerManager::GetInstance()->GetDeltaTime();

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

	//SimpleObject* OwnerObj = static_cast<SimpleObject*>(Owner);
	FLineResult Result;
	Result.IsDiagonalLine = bDiagonalLine;

	if (LineManager::GetInstance()->CollisionWallLine(Owner->GetPos(), Result, Owner->GetCollider()->GetSize()))
	{
		FPOINT ObjPos = Owner->GetPos();
		ObjPos.x = Result.OutPos.x;
		Owner->SetPos(ObjPos);

		Velocity.x = -Velocity.x * Elasticity;

		if (abs(Velocity.x) < 1.f)
			Velocity.x = 0.f;
	}


	// ��
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
		// ���� �̹� ���� ��				// �밢�� ó���� ��� �ұ� �ϴ� ���ΰ� ���ڷ� �޾ƾ��ϴ��� ���� �����غ���.
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

	//  õ��
	if (!bGround && LineManager::GetInstance()->CollisionCeilingLine(Owner->GetPos(), Result, Owner->GetCollider()->GetSize().y))
	{
		FPOINT ObjPos = Owner->GetPos();
		//ObjPos.y = Result.OutPos.y;
		//Owner->SetPos(ObjPos);

		Velocity.y = -Velocity.y * Elasticity;

		if (abs(Velocity.y) < 1.f)
			Velocity.y = 0.f;
	}
}
