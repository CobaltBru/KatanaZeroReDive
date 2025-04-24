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
    image = ImageManager::GetInstance()->AddImage("explosion", L"Image/HeadHunter/explosion.bmp", 770 * 5, 70 * 5, 11, 1, true, RGB(255, 0, 255)); // ������ ���

    image = ImageManager::GetInstance()->FindImage("wBomb");

    
    ObjectCollider = new Collider(this, EColliderType::Rect, {}, { 10.f,10.f }, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Bullet);
    ObjectCollider->SetPos(Pos);

    ObjectRigidBody = new RigidBody(this); // this = Bomb ��ü
    ObjectRigidBody->SetMass(1.0f); // ����
    ObjectRigidBody->SetFriction(0.0f); // ���߿����� ���� ����
    ObjectRigidBody->SetGravityVisible(true);  // �߷� ����
    ObjectRigidBody->SetElasticity(3.0f); // ƨ��� ����

    // �ʱ� �߻� �ӵ� (������)
    ObjectRigidBody->SetMaxVelocity({ 300.f, 1000.f });
    ObjectRigidBody->SetVelocity({ speed, speed2 }); // �� ����, �� ���� // ������ ������ �Ź� �ٸ��� �Ϸ���? > ����Ƽ��..

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
    image = ImageManager::GetInstance()->AddImage("explosion", L"Image/HeadHunter/explosion.bmp", 770 * 5, 70 * 5, 11, 1, true, RGB(255, 0, 255)); // ������ ���

    image = ImageManager::GetInstance()->FindImage("wBomb");


    ObjectCollider = new Collider(this, EColliderType::Rect, {}, { 10.f,10.f }, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Bullet);
    ObjectCollider->SetPos(Pos);

    ObjectRigidBody = new RigidBody(this); 
    ObjectRigidBody->SetMass(1.0f); // ����
    ObjectRigidBody->SetFriction(0.0f); // ���߿����� ���� ����
    ObjectRigidBody->SetGravityVisible(true);  // �߷�
    ObjectRigidBody->SetElasticity(3.0f); // ƨ��� ����

    // �ʱ� �߻� �ӵ� (������)
    ObjectRigidBody->SetMaxVelocity({ 300.f, 600.f });
    ObjectRigidBody->SetVelocity({ +300.f, -600.f + 100.f * (rand() % 3)}); // �� ����, �� ���� // ������ ������ �Ź� �ٸ��� �Ϸ���? > ����Ƽ��..

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

    ObjectRigidBody->Update(); // ������ �ٵ� ���� �ϰ� �ݸ��� �� ��
    Collision();

    

    // �ð��� ������ ���߰� ����
    if (ObjectCollider->IsHitted() || timer > 5.0f)
    {   
        ObjectCollider->SetSize({ 150,150 });
        image = ImageManager::GetInstance()->FindImage("explosion");
        ObjectRigidBody->SetGravityVisible(false); // ���߰�
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
    // �浹 ����
    FHitResult HitResult;

    // �� �ݶ��̴��� ECollisionGroup::Enemy�� �ִ� �ݶ��̴���� �浹ó��
    if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
    {
        // �浹�ߴ�.

        ObjectCollider->SetHit(true);	// �� �ݶ��̴� �浹
        HitResult.HitCollision->SetHit(true);// ���� �ݶ��̴� �浹

        HitResult.HitCollision->GetOwner();  // ���� ��ü ����
    }
}
