#include "RigidBody.h"
#include "GameObject.h"
#include "CommonFunction.h"

RigidBody::RigidBody()
	: Owner(nullptr), Mass(1.f), Velocity({0.f,0.f})
{
}

RigidBody::RigidBody(GameObject* InOwner)
	:Owner(InOwner), Mass(1.f), Velocity({ 0.f,0.f })
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update()
{
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

		//속도
		Velocity += Acceleration * TimerManager::GetInstance()->GetDeltaTime();
	}
	
	Move();

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
