#include "Bomb.h"
#include "Collider.h"
#include "Image.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "RenderManager.h"

Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}

HRESULT Bomb::Init(FPOINT pos, float speed, float speed2)
{
	
    Pos = pos;
    this->speed = speed;
    this->speed2 = speed2;

    isActive = true;
    timer = 0;
    frameIndex = 0;
    timer2 = 0;
    

    image = ImageManager::GetInstance()->AddImage("wBomb", L"Image/HeadHunter/wBomb.bmp", 14, 10, 1, 1, true, RGB(255, 0, 255));
    image = ImageManager::GetInstance()->AddImage("explosion", L"Image/HeadHunter/explosion.bmp", 770 * 5, 70 * 5, 11, 1, true, RGB(255, 0, 255)); // 터지는 모션

    image = ImageManager::GetInstance()->FindImage("wBomb");

    
    ObjectCollider = new Collider(this, EColliderType::Rect, {}, { 10.f,10.f }, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Bullet);
    ObjectCollider->SetPos(Pos);

    ObjectRigidBody = new RigidBody(this); // this = Bomb 자체
    ObjectRigidBody->SetMass(1.0f); // 질량
    ObjectRigidBody->SetFriction(0.0f); // 공중에서는 마찰 없음
    ObjectRigidBody->SetGravityVisible(true);  // 중력 적용
    ObjectRigidBody->SetElasticity(3.0f); // 튕기는 정도

    // 초기 발사 속도 (포물선)
    ObjectRigidBody->SetMaxVelocity({ 300.f, 1000.f });
    ObjectRigidBody->SetVelocity({ speed, speed2 }); // → 방향, ↑ 방향 // 나오는 각도를 매번 다르게 하려면? > 지피티야..

    return S_OK; 
}

HRESULT Bomb::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
    Pos = InPos;


    isActive = true;
    timer = 0;
    frameIndex = 0;
    timer2 = 0;

    image = ImageManager::GetInstance()->AddImage("wBomb", L"Image/HeadHunter/wBomb.bmp", 14, 10, 1, 1, true, RGB(255, 0, 255));
    image = ImageManager::GetInstance()->AddImage("explosion", L"Image/HeadHunter/explosion.bmp", 770 * 5, 70 * 5, 11, 1, true, RGB(255, 0, 255)); // 터지는 모션

    image = ImageManager::GetInstance()->FindImage("wBomb");


    ObjectCollider = new Collider(this, EColliderType::Rect, {}, { 10.f,10.f }, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Bullet);
    ObjectCollider->SetPos(Pos);

    ObjectRigidBody = new RigidBody(this); 
    ObjectRigidBody->SetMass(1.0f); // 질량
    ObjectRigidBody->SetFriction(0.0f); // 공중에서는 마찰 없음
    ObjectRigidBody->SetGravityVisible(true);  // 중력
    ObjectRigidBody->SetElasticity(3.0f); // 튕기는 정도

    // 초기 발사 속도 (포물선)
    ObjectRigidBody->SetMaxVelocity({ 300.f, 600.f });
    ObjectRigidBody->SetVelocity({ +300.f, -600.f + 100.f * (rand() % 3)}); // → 방향, ↑ 방향 // 나오는 각도를 매번 다르게 하려면? > 지피티야..

    return S_OK;
}

void Bomb::Release()
{
    if (ObjectRigidBody != nullptr)
    {
        delete ObjectRigidBody;
        ObjectRigidBody = nullptr;
    }
}

void Bomb::Update()
{   
    LastPos = Pos;
    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
    timer += TimerManager::GetInstance()->GetDeltaTime();

    ObjectRigidBody->Update(); // 리지드 바디 먼저 하고 콜리전 할 것
    Collision();

    

    // 시간이 지나면 멈추고 터짐
    if (ObjectCollider->IsHitted() || timer > 5.0f)
    {   
        ObjectCollider->SetSize({ 150,150 });
        image = ImageManager::GetInstance()->FindImage("explosion");
        ObjectRigidBody->SetGravityVisible(false); // 멈추고
        ObjectRigidBody->SetVelocity({ 0,0 });

        timer2 += TimerManager::GetInstance()->GetDeltaTime();
        if (timer2 > 0.1f)
        {
            frameIndex++;
            timer2 = 0;
        }
        if (frameIndex >= image->GetMaxFrameX())
        {
            frameIndex = image->GetMaxFrameX();
            bDead = true;
        }
    }
}

void Bomb::Render(HDC hdc)
{
    if (image)
    {
        if (isActive)
        {
            image->FrameRender(hdc, Pos.x, Pos.y, frameIndex,0);
        }
    }
}

void Bomb::Collision()
{
    // 충돌 정보
    FHitResult HitResult;

    // 내 콜라이더와 ECollisionGroup::Enemy에 있는 콜라이더들과 충돌처리
    if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
    {
        // 충돌했다.

        ObjectCollider->SetHit(true);	// 내 콜라이더 충돌
        HitResult.HitCollision->SetHit(true);// 상대방 콜라이더 충돌

        HitResult.HitCollision->GetOwner();  // 상대방 객체 접근
    }
}
