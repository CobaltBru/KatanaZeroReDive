#include "GangsterBullet.h"
#include "Image.h"
#include "ImageManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "SnapShotManager.h"
#include "CommonFunction.h"
#include "ScrollManager.h"
#include "ObjectManager.h"

GangsterBullet::GangsterBullet()
    : image(nullptr), dir(0), speed(0.f), force(0.f)
{
}

GangsterBullet::~GangsterBullet()
{
}

HRESULT GangsterBullet::Init(FPOINT pos, int dir)
{
    Pos = pos;
    this->dir = dir;
    image = ImageManager::GetInstance()->AddImage("GangsterBullet", L"Image/Enemy/Gangster/Gangster_Bullet.bmp", 17, 1, 1, 1, true, RGB(255, 0, 255));
    ObjectCollider = new Collider(this, EColliderType::Rect, {}, {(float)image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale(), (float)image->GetFrameHeight() * ScrollManager::GetInstance()->GetScale() }, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Bullet);
    ObjectCollider->SetPos(Pos);
    ObjectRigidBody = new RigidBody(this);
	speed = 1000.f;
	force = 500.f;
	InitRigidBodySetting();
	ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, this);
    return S_OK;
}

void GangsterBullet::Release()
{
	if (ObjectRigidBody)
	{
		delete ObjectRigidBody;
		ObjectRigidBody = nullptr;
	}
		
}

void GangsterBullet::Update()
{
	LastPos = Pos;
	Fire();
	Collision();
	if (Pos.x > WINSIZE_X || Pos.x < 0 || Pos.y > WINSIZE_Y || Pos.y < 0)
		bDead = true;

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void GangsterBullet::Render(HDC hdc)
{
	if (image)
	{
		image->FrameRender(hdc, Pos.x, Pos.y, false, true, ScrollManager::GetInstance()->GetScale());
	}
}

void GangsterBullet::MakeSnapShot(void* out)
{

}

void GangsterBullet::InitRigidBodySetting()
{
	if (ObjectRigidBody == nullptr)
		return;

	// 탄성 적용안함  0 ~ 1 사이
	ObjectRigidBody->SetElasticity(0.f);

	// 중력 적용
	ObjectRigidBody->SetGravityVisible(false);
	// 저항 
	ObjectRigidBody->SetAccelerationAlpha({ 0.f,800.f });
	//무게
	ObjectRigidBody->SetMass(1.f);
	//최대 속도
	ObjectRigidBody->SetMaxVelocity({ speed , 400.f });

	//마찰
	ObjectRigidBody->SetFriction(50.f);
}

void GangsterBullet::Collision()
{
	FHitResult HitResult;
	if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
	{
		ObjectCollider->SetHit(true);
		HitResult.HitCollision->SetHit(true);

		HitResult.HitCollision->GetOwner();

		FPOINT pos;
		pos.x = HitResult.HitCollision->GetPos().x - ObjectCollider->GetPos().x;
		pos.y = HitResult.HitCollision->GetPos().y - ObjectCollider->GetPos().y;
		Normalize(pos);

		HitResult.HitCollision->GetOwner()->GetRigidBody()->AddForce(pos * force);
	}
}

void GangsterBullet::Fire()
{
	if (!ObjectRigidBody) return;
	ObjectRigidBody->AddVelocity({ dir * speed, 0.f });
	ObjectRigidBody->Update();
}
