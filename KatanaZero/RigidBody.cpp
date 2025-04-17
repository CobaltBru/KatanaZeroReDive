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
