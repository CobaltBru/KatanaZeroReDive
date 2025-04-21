#include "Bullet.h"
#include "GPImage.h"
#include "CollisionManager.h"
#include "Collider.h"

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

    isActive = true;

    image = new GPImage();
    image->AddImage(L"Image/HeadHunter/bullet.png");

    BulletCollider = new Collider(this, EColliderType::Rect, {}, 10.f, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(BulletCollider, ECollisionGroup::Bullet);
    BulletCollider->SetPos(Pos);

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
    if(isActive)
    {
        Pos.x += cosf(DEG_TO_RAD(angle));
        Pos.y += sinf(DEG_TO_RAD(angle));

    }

    Collision();
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
    // �浹 ����
    FHitResult HitResult;

    // �� �ݶ��̴��� ECollisionGroup::Enemy�� �ִ� �ݶ��̴���� �浹ó��
    if (CollisionManager::GetInstance()->CollisionAABB(BulletCollider, HitResult, ECollisionGroup::Player))
    {
        // �浹�ߴ�.

        BulletCollider->SetHit(true);	// �� �ݶ��̴� �浹
        HitResult.HitCollision->SetHit(true);// ���� �ݶ��̴� �浹

        HitResult.HitCollision->GetOwner();  // ���� ��ü ����
    }
}
