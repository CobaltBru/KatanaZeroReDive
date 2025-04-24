#include "ParticleEffect.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "ObjectManager.h"

Particle::Particle()
{
	ObjectRigidBody = new RigidBody(this);
	InitRigidBodySetting();
}

HRESULT Particle::Init()
{
	timer = 0;
	scale = 4.f;
	/*ObjectCollider = new Collider(this, EColliderType::Rect, {}, { scale,
		scale }, false, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	ObjectCollider->SetPos(Pos);*/
	
	FPOINT dest;
	angle = rand() % 360;
	force = rand() % 400 + 100.f;
	dest.x = cos(DEG_TO_RAD(angle));
	dest.y = sin(DEG_TO_RAD(angle));
	Normalize(dest);
	ObjectRigidBody->SetVelocity(dest * force);

	return S_OK;
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
	timer += TimerManager::GetInstance()->GetDeltaTime(false);
	ObjectRigidBody->Update();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
}

void Particle::Render(HDC hdc)
{
	float percent = (100.f - timer * 200.f) / 100.f;
	HBRUSH hBrush = CreateSolidBrush(RGB(127 * percent, 212 * percent, 255 * percent));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	// 2) 테두리 없애려면 NULL_PEN 선택 (있어도 상관없다면 생략 가능)
	HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));

	// 3) 사각형 그리기 (Rectangle은 내부를 브러시로 채우고 테두리를 펜으로 그림)
	Rectangle(hdc, Pos.x, Pos.y, Pos.x + scale, Pos.y + scale);

	// 4) 원래 브러시·펜 복구
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	// 5) 생성한 브러시 해제
	DeleteObject(hBrush);
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
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,500.f });
	//무게
	ObjectRigidBody->SetMass(3.f);
	//최대 속도
	ObjectRigidBody->SetMaxVelocity({ 1000.f,1000.f });
	//마찰
	ObjectRigidBody->SetFriction(50.f);
}





ParticleEffect::ParticleEffect()
{
}

HRESULT ParticleEffect::Init()
{
	for (int i = 0; i < 40; i++)
	{
		Particle* particle = new Particle();
		particle->Init();
		freeList.push_back(particle);
		
	}
	timer = 0;
	delay = 0.05f;
	return S_OK;
}

void ParticleEffect::Release()
{
	list<Particle*>::iterator iter = activeList.begin();
	for (iter = activeList.begin(); iter != activeList.end(); iter)
	{
		(*iter)->Release();
		delete (*iter);
		*iter = nullptr;
		iter = activeList.erase(iter);
	}
	iter = freeList.begin();
	for (iter = freeList.begin(); iter != freeList.end(); iter)
	{
		(*iter)->Release();
		delete (*iter);
		*iter = nullptr;
		iter = freeList.erase(iter);
	}
	freeList.clear();
	activeList.clear();
	
}

void ParticleEffect::Update()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
	if (timer > delay)
	{
		timer = 0;
		activeParticle();
	}
	freeParticle();
	list<Particle*>::iterator iter = activeList.begin();
	for (iter = activeList.begin(); iter != activeList.end(); iter++)
	{
		if ((*iter) != nullptr)
		{
			(*iter)->Update();
		}
	}
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
}

void ParticleEffect::Render(HDC hdc)
{
	list<Particle*>::iterator iter = activeList.begin();
	for (iter = activeList.begin(); iter != activeList.end(); iter++)
	{
		if ((*iter) != nullptr)
		{
			(*iter)->Render(hdc);
		}
	}
}

void ParticleEffect::activeParticle()
{
	list<Particle*>::iterator iter = freeList.begin();
	int cnt = 0;
	int dest = 2;
	for (iter = freeList.begin(); iter != freeList.end(); iter)
	{
		if (freeList.empty()) return;
		(*iter)->Init();
		(*iter)->SetPos(Pos);
		activeList.push_back(*iter);
		iter = freeList.erase(iter);
		++cnt;
		if (cnt >= dest) return;
	}
}

void ParticleEffect::freeParticle()
{
	list<Particle*>::iterator iter = activeList.begin();
	for (iter = activeList.begin(); iter != activeList.end(); iter)
	{
		if (activeList.empty()) return;
		if ((*iter)->getTimer() >= 0.4f)
		{
			freeList.push_back(*iter);
			iter = activeList.erase(iter);
		}
		else
		{
			++iter;
		}
		
		
	}
}
