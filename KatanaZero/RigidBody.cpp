#include "RigidBody.h"
//#include "GameObject.h"
#include "CommonFunction.h"
#include "LineManager.h"
#include "TaeKyungObject.h"
#include "Collider.h"

RigidBody::RigidBody()
	: Owner(nullptr), Mass(1.f), Velocity({ 0.f,0.f }), Force({ 0.f,0.f }), FrictionCoefficient(0.f), MaxSpeed(100.f), bGravity(true), Gravity(9.8f), bGround(false)
{
}

RigidBody::RigidBody(GameObject* InOwner)
	:Owner(InOwner), Mass(1.f), Velocity({ 0.f,0.f }), Force({0.f,0.f}), FrictionCoefficient(80.f), MaxSpeed(100.f), bGravity(true), Gravity(9.8f), bGround(false)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update()
{
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

		//�ӵ�
		Velocity += Acceleration * TimerManager::GetInstance()->GetDeltaTime();
	}
	
	//����
	FPOINT tempVelocity = Velocity;
	Normalize(tempVelocity);
	FPOINT friction = -tempVelocity * FrictionCoefficient * TimerManager::GetInstance()->GetDeltaTime();

	if (GetLength(Velocity) <= GetLength(friction))
		Velocity = { 0.f,0.f };
	else
		Velocity += friction;

	if (MaxSpeed < GetLength(Velocity))
	{
		Normalize(Velocity);
		Velocity *= MaxSpeed;
	}

	Move();
	CollisionLine();

	Force = { 0.f,0.f };
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
	// ��
	if (LineManager::GetInstance()->CollisionLine(OwnerObj->GetPos(), Result, OwnerObj->GetCollider()->GetSize().y))
	{
		FPOINT ObjPos = OwnerObj->GetPos();
		ObjPos.y = Result.OutPos.y;
		OwnerObj->SetPos(ObjPos);
	}
	//  õ��
	else if (LineManager::GetInstance()->CollisionCeilingLine(OwnerObj->GetPos(), Result, OwnerObj->GetCollider()->GetSize().y))
	{
		FPOINT ObjPos = OwnerObj->GetPos();
		ObjPos.y = Result.OutPos.y;
		OwnerObj->SetPos(ObjPos);
	}
}
