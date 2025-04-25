#include "Lazer.h"
#include "GPImage.h"
#include "ScrollManager.h"
#include "Collider.h"
#include "CollisionManager.h"
Lazer::Lazer()
{
}

Lazer::~Lazer()
{
}

HRESULT Lazer::Init()
{
	Pos = { 0,0 };
	angle = 0;
	isActive = false;
    bCanCollide = false;
    frameIndex = 0;
    timer = 0;
    timer2 = 0;
    
    warningTime = 1.0f;

	image = new GPImage();
	image->AddImage(L"Image/HeadHunter/lazer_frame.png", 4, 1);
    
    
    ObjectCollider = new Collider(this, EColliderType::Rect, {}, 10.f, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Bullet);
    ObjectCollider->SetPos(Pos);
    
    return S_OK;
}

void Lazer::Release()
{
    if (image)
    {
        image->Release();
        delete image;
        image = nullptr;
       
    }
}

void Lazer::Update(FPOINT pos, float angle)
{

    if (isActive)
    {
        timer += TimerManager::GetInstance()->GetDeltaTime();
        
        if (timer > warningTime) // ��� ������ 1.0f ��� �� ������ ��� // ó�����ķδ� ��� ���Ұ��̹Ƿ� timer 
        {
            timer2 += TimerManager::GetInstance()->GetDeltaTime();
            if (frameIndex < 3)
            {
                if (timer2 > 0.05f)
                {
                    frameIndex++;
                    timer2 = 0;
                }
            }
            if (frameIndex >= 3)
            {
                frameIndex = 3;
            }
            
        }
        
        this->angle = angle;
        Pos = pos;     

        if (bCanCollide)
        {
		    if (frameIndex == 3)
		    {
			    Collision();
		    }
        }

    }
    if(!isActive)
    {
        frameIndex = 0;
        timer = 0;
    }
}

void Lazer::Render(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    if (image)
    {
        if (isActive)
        {
            image->Middle_RenderAll(&graphics, Pos, frameIndex, angle, false, 1.0f, 1.f, 1.f, 1.f,
                ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale() * 1.5);   
        }
        
    }
    
}

void Lazer::Collision()
{
    // �浹 ����
    FHitResult HitResult;

    // �� �ݶ��̴��� ECollisionGroup::Enemy�� �ִ� �ݶ��̴���� �浹ó��
    if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
    {
        // �浹�ߴ�.

        ObjectCollider->SetHit(false);	// �� �ݶ��̴� �浹
        HitResult.HitCollision->SetHit(true);// ���� �ݶ��̴� �浹

        HitResult.HitCollision->GetOwner();  // ���� ��ü ����adaad

        FPOINT pos;
        pos.x = HitResult.HitCollision->GetPos().x - ObjectCollider->GetPos().x;
        pos.y = HitResult.HitCollision->GetPos().y - ObjectCollider->GetPos().y;
        

        // ������ ������ٵ� ���� ����
        //HitResult.HitCollision->GetOwner()->GetRigidBody()->AddVelocity(pos * 500.f);


    }
    //���� Ʈ���̽�
    FHitResult Hitresult;
    if (CollisionManager::GetInstance()->LineTraceByObject(Hitresult, ECollisionGroup::Player, Pos, { Pos.x + 700 * cosf(DEG_TO_RAD(angle)), Pos.y + 700 * sinf(DEG_TO_RAD(angle))}, true, 0.f,RGB(255,0,255)))
    {
        // ���� Ʈ���̽� ���� ����� �ݶ��̴�
        Hitresult.HitCollision->SetHit(true);
    }
}