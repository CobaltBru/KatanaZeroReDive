#include "Bullet.h"
#include "GPImage.h"

#include "CollisionManager.h"
#include "Collider.h"
#include "ScrollManager.h"
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
    this->dir = 0;
    speed = 1500.f;
    targetType = ECollisionGroup::Player;
    isActive = true;

    image = new GPImage();
    image->AddImage(L"Image/HeadHunter/bullet.png");

    ObjectCollider = new Collider(this, EColliderType::Rect, {}, 10.f, false, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Bullet);
    ObjectCollider->SetPos(Pos);


    return S_OK;
}

HRESULT Bullet1::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
    Pos = InPos;
    this->angle = angle;

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

    ObjectCollider = new Collider(this, EColliderType::Rect, {}, 10.f, false, 1.f);
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

    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);

    float dt = TimerManager::GetInstance()->GetDeltaTime();
    if(isActive)
    {
        if (dir != 0)
        {
            Pos.x += speed * cosf(DEG_TO_RAD(angle)) * dt * dir;
            Pos.y += speed * sinf(DEG_TO_RAD(angle)) * dt * dir;
        }
        else
        {
            Pos.x += speed * cosf(DEG_TO_RAD(angle)) * dt; // ������ �������� �ӵ� ���� ��������.
            Pos.y += speed * sinf(DEG_TO_RAD(angle)) * dt;
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
            image->Middle_RenderAll(&graphics, Pos, 0, angle, false, 1.0f, 1.0f,1.0f,1.0f, ScrollManager::GetInstance()->GetScale());
        }

    }
}

void Bullet1::Collision()
{
    // �浹 ����
    FHitResult HitResult;

    // �� �ݶ��̴��� ECollisionGroup::Enemy�� �ִ� �ݶ��̴���� �浹ó��

    if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, targetType))
    {
        // �浹�ߴ�.

        ObjectCollider->SetHit(true);	// �� �ݶ��̴� �浹
        HitResult.HitCollision->SetHit(true);// ���� �ݶ��̴� �浹

        HitResult.HitCollision->GetOwner();  // ���� ��ü ����
    }
}
