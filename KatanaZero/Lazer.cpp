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
        
        if (timer > warningTime) // 경고 레이저 1.0f 재생 후 레이저 재생 // 처음이후로는 경고 안할거이므로 timer 
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
    // 충돌 정보
    FHitResult HitResult;

    // 내 콜라이더와 ECollisionGroup::Enemy에 있는 콜라이더들과 충돌처리
    if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
    {
        // 충돌했다.

        ObjectCollider->SetHit(false);	// 내 콜라이더 충돌
        HitResult.HitCollision->SetHit(true);// 상대방 콜라이더 충돌

        HitResult.HitCollision->GetOwner();  // 상대방 객체 접근adaad

        FPOINT pos;
        pos.x = HitResult.HitCollision->GetPos().x - ObjectCollider->GetPos().x;
        pos.y = HitResult.HitCollision->GetPos().y - ObjectCollider->GetPos().y;
        

        // 상대방의 리지드바디에 힘을 전달
        //HitResult.HitCollision->GetOwner()->GetRigidBody()->AddVelocity(pos * 500.f);


    }
    //라인 트레이스
    FHitResult Hitresult;
    if (CollisionManager::GetInstance()->LineTraceByObject(Hitresult, ECollisionGroup::Player, Pos, { Pos.x + 700 * cosf(DEG_TO_RAD(angle)), Pos.y + 700 * sinf(DEG_TO_RAD(angle))}, true, 0.f,RGB(255,0,255)))
    {
        // 라인 트레이스 맞은 대상의 콜라이더
        Hitresult.HitCollision->SetHit(true);
    }
}