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

	// 탄성 적용안함  0 ~ 1 사이
	ObjectRigidBody->SetElasticity(0.3f);

	// 중력 적용
	ObjectRigidBody->SetGravityVisible(true);
	// 저항 
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
	//무게
	ObjectRigidBody->SetMass(5.f);
	//최대 속도
	ObjectRigidBody->SetMaxVelocity({ 120.f,100 });
	//마찰
	ObjectRigidBody->SetFriction(50.f);
}