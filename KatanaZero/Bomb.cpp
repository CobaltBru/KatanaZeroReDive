#include "Bomb.h"
#include "Collider.h"
#include "Image.h"
#include "CollisionManager.h"
#include "RigidBody.h"

Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}

HRESULT Bomb::Init(FPOINT pos, float angle)
{
	
    Pos = pos;
    this->angle = angle;

    isActive = true;

    image = ImageManager::GetInstance()->AddImage("wBomb", L"Image/HeadHunter/wBomb.bmp", 28, 26, 1, 1, true, RGB(255, 0, 255));
    explosionImage = ImageManager::GetInstance()->AddImage("explosion", L"Image/HeadHunter/explosion.bmp", 770, 70, 11, 1, true, RGB(255, 0, 255));
    

    image = ImageManager::GetInstance()->FindImage("wBomb");
    explosionImage = ImageManager::GetInstance()->FindImage("explosion");
    
    BombCollider = new Collider(this, EColliderType::Rect, {}, 10.f, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(BombCollider, ECollisionGroup::Bullet);
    BombCollider->SetPos(Pos);

    ObjectRigidBody = new RigidBody(this);

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
    //ObjectRigidBody->Update();
    //ObjectRigidBody->AddForce({200,200});

    //ObjectRigidBody->AddVelocity({ 200,200 });
    if (KeyManager::GetInstance()->IsStayKeyDown('L'))
    {
        BombCollider->SetHit(true);
    }

    Collision();
}

void Bomb::Render(HDC hdc)
{
    if (image)
    {
        if (isActive)
        {
            image->FrameRender(hdc, Pos.x, Pos.y, 0,0);
        }
    }
    if (explosionImage)
    {
        if (BombCollider->IsHitted())
        {
            explosionImage->FrameRender(hdc, Pos.x, Pos.y, 0, 0);
            explosionImage->FrameRender(hdc, Pos.x + 20, Pos.y + 20, 5, 0);
        }
    }
}

void Bomb::Collision()
{
    // �浹 ����
    FHitResult HitResult;

    // �� �ݶ��̴��� ECollisionGroup::Enemy�� �ִ� �ݶ��̴���� �浹ó��
    if (CollisionManager::GetInstance()->CollisionAABB(BombCollider, HitResult, ECollisionGroup::Player))
    {
        // �浹�ߴ�.

        BombCollider->SetHit(true);	// �� �ݶ��̴� �浹
        HitResult.HitCollision->SetHit(true);// ���� �ݶ��̴� �浹

        HitResult.HitCollision->GetOwner();  // ���� ��ü ����
    }
}
