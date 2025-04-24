#include "Bullet.h"
#include "GPImage.h"
#include "ObjectManager.h"

Bullet1::Bullet1()
{
}

Bullet1::~Bullet1()
{
}

HRESULT Bullet1::Init(FPOINT pos, float angle)
{
    Pos = pos;
    this->angle = angle;
    this->dir = dir;
    speed = 1.f;
    targetType = ECollisionGroup::Player;
    isActive = true;

    image = new GPImage();
    image->AddImage(L"Image/HeadHunter/bullet.png");

    ObjectCollider = new Collider(this, EColliderType::Rect, {}, 10.f, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Bullet);
    ObjectCollider->SetPos(Pos);
    ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, this);

    return S_OK;
}

HRESULT Bullet1::Init(FPOINT pos, float angle, float speed, int dir)
{
    Pos = pos;
    this->angle = angle;
    this->speed = speed;
    this->dir = dir;
    targetType = ECollisionGroup::Player;
    isActive = true;


    image = new GPImage();
    image->AddImage(L"Image/HeadHunter/bullet.png");

    ObjectCollider = new Collider(this, EColliderType::Rect, {}, 10.f, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Bullet);
    ObjectCollider->SetPos(Pos);
    ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, this);

    return S_OK;
}

void Bullet1::Release()
{
    if (image)
    {
        image->Release();
        delete image;
        image = nullptr;
    }
}

void Bullet1::Update()
{
    float dt = TimerManager::GetInstance()->GetDeltaTime();
    if(isActive)
    {
        if (speed != 1.f)
        {
            Pos.x += speed * cosf(DEG_TO_RAD(angle)) * dt * dir;
            Pos.y += speed * sinf(DEG_TO_RAD(angle)) * dt * dir;
        }
        else
        {
            Pos.x += cosf(DEG_TO_RAD(angle));
            Pos.y += sinf(DEG_TO_RAD(angle));
        }
    }

    if (this->ObjectCollider->IsHitted())
    {
        if (this->targetType == ECollisionGroup::Player)
            targetType = ECollisionGroup::Enemy;
    }

    Collision();
    if (Pos.x > WINSIZE_X || Pos.x < 0 || Pos.y > WINSIZE_Y || Pos.y < 0)
        this->SetDead(true);
}

void Bullet1::Render(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    if (image)
    {
        if (isActive)
        {
            image->Middle_RenderFrameAngle(&graphics, Pos, 0, angle, false, 1.0f);
        }

    }
}

void Bullet1::Collision()
{
    // 충돌 정보
    FHitResult HitResult;

    // 내 콜라이더와 ECollisionGroup::Enemy에 있는 콜라이더들과 충돌처리
    if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, targetType))
    {
        // 충돌했다.

        ObjectCollider->SetHit(true);	// 내 콜라이더 충돌
        HitResult.HitCollision->SetHit(true);// 상대방 콜라이더 충돌

        HitResult.HitCollision->GetOwner();  // 상대방 객체 접근
    }
}
