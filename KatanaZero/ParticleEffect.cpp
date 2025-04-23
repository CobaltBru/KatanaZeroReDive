#include "ParticleEffect.h"
#include "RigidBody.h"

Particle::Particle()
{
}

HRESULT Particle::Init()
{
	ObjectRigidBody = new RigidBody(this);
	InitRigidBodySetting();
	return E_NOTIMPL;
}

void Particle::Release()
{
	if (ObjectRigidBody != nullptr)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
}

void Particle::Update()
{
	ObjectRigidBody->Update();
}

void Particle::Render(HDC hdc)
{
}

void Particle::InitRigidBodySetting()
{
	if (ObjectRigidBody == nullptr)
		return;

	// ź�� �������  0 ~ 1 ����
	ObjectRigidBody->SetElasticity(0.3f);

	// �߷� ����
	ObjectRigidBody->SetGravityVisible(true);
	// ���� 
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
	//����
	ObjectRigidBody->SetMass(5.f);
	//�ִ� �ӵ�
	ObjectRigidBody->SetMaxVelocity({ 120.f,100 });
	//����
	ObjectRigidBody->SetFriction(50.f);
}