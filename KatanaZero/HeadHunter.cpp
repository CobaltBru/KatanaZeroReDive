#include "HeadHunter.h"
#include "Image.h"
#include "RenderManager.h"
#include "CommonFunction.h"
#include "Lazer.h"
#include "Bullet.h"
#include "Collider.h"
#include "CollisionManager.h"

HeadHunter::HeadHunter()
{
}

HeadHunter::~HeadHunter()
{
}

HRESULT HeadHunter::Init(FPOINT InPos)
{
    Pos = InPos;
    firePos = { 0,0 };
    wallPos = { 840, 360 };
    jumpDist1 = { 0,0 };
    jumpDist2 = { 0,0 };
    jumpDist3 = { 0,0 };

    isFlip = false;
    isAttacked = false;
    isDead = false;

    angle = 0;
    weaponAngle = 0;
    dAngle = 0;
    timer = 0;
    moveTimer = 0;
    bulletTimer = 0;
    hp = 4;
    wave = 0;
    bulletWave = 0;
    dashWave = 0;
    lazerWave = 0;
    gunWave = 0;

    loop = 0;
    lazerLoop = 0;
    gunCount = 0;
    frameIndex = 0;

    state = State::Idle;

    {
        image = ImageManager::GetInstance()->AddImage("Idle", L"Image/HeadHunter/headhunter_idle.bmp", 840, 70, 12, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->AddImage("GroundLazer", L"Image/HeadHunter/headhunter_takeoutrifle.bmp", 560, 70, 8, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("RoundLazer", L"Image/HeadHunter/headhunter_lazer.bmp", 972, 46, 18, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Teleport", L"Image/HeadHunter/teleport1.bmp", 280, 70, 4, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("VerticalLazer", L"Image/HeadHunter/teleport2.bmp", 280, 70, 4, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("VerticalLazer_out", L"Image/HeadHunter/teleport3.bmp", 280, 70, 4, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->AddImage("Faint", L"Image/HeadHunter/headhunter_faint.bmp", 495, 45, 11, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->AddImage("GroundGun", L"Image/HeadHunter/GroundGun.bmp", 490, 70, 7, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("GroundGun_shoot", L"Image/HeadHunter/GroundGun_shoot.bmp", 800, 70, 8, 1, true, RGB(255, 0, 255));

        //BULLET
        image = ImageManager::GetInstance()->AddImage("Jump", L"Image/HeadHunter/headhunter_jump.bmp", 27, 44, 1, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Jumpidle", L"Image/HeadHunter/headhunter_jumpidle.bmp", 90, 43, 3, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Jumpto", L"Image/HeadHunter/headhunter_jumptoceil.bmp", 35, 46, 1, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Bullet", L"Image/HeadHunter/headhunter_shoot.bmp", 385, 55, 7, 1, true, RGB(255, 0, 255));

        //DASH
        image = ImageManager::GetInstance()->AddImage("Readytodash", L"Image/HeadHunter/readytodash.bmp", 432, 45, 9, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Dash", L"Image/HeadHunter/dash.bmp", 51, 25, 1, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Dashend", L"Image/HeadHunter/dashend.bmp", 500, 50, 10, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->FindImage("Idle");
    }
    // test 
    playerPos = { 500 , 360 };

    ObjectCollider = new Collider(this, EColliderType::Rect, {0,10.f}, { 20.f ,40.f}, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
    ObjectCollider->SetPos(Pos);

    lazer = new Lazer();
    lazer->Init();

    return S_OK;
}

void HeadHunter::Release()
{
    if (lazer)
    {
        lazer->Release();
        delete lazer;
        lazer = nullptr;
    }

    for (auto bullet : bullets) {
        bullet->Release();
        delete bullet;
    }
    bullets.clear();
}

void HeadHunter::Update()
{
    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
    timer += TimerManager::GetInstance()->GetDeltaTime();
    moveTimer += TimerManager::GetInstance()->GetDeltaTime();
    bulletTimer += TimerManager::GetInstance()->GetDeltaTime();

    Collision();

    lazer->Update(firePos, weaponAngle);

    for (auto bullet : bullets) {
        bullet->Update();
    }
    

    CheckPlayerPos();

    switch (state) {
    case State::Idle:
        Idle();
        break;

    case State::GroundLazer:
        GroundLazer();
        break;

    case State::GroundGun:
        GroundGun();
        break;

    case State::Bullet:
        Bullet();
        break;

    case State::Teleport:
        Teleport();
        break;

    case State::VerticalLazer:
        VerticalLazer();
        break;

    case State::RoundLazer:
        RoundLazer();
        break;

    case State::Dash:
        Dash();
        break;

    case State::DashDown:
        DashDown();
        break;

    case State::Faint:
        Faint();
        break;
    }
}

void HeadHunter::Render(HDC hdc)
{
    if (image)
    {
        //RenderRectAtCenter(hdc, WINSIZE_X/2,WINSIZE_Y/2, 640, 260);
        RenderRectAtCenter(hdc, playerPos.x, playerPos.y, 20, 40);

        image->FrameRender(hdc, Pos.x, Pos.y, frameIndex, 0, isFlip);

        lazer->Render(hdc);

        for (auto bullet : bullets) {
            bullet->Render(hdc);
        }
    }
}

void HeadHunter::Collision()
{
    // 충돌 정보
    FHitResult HitResult;

    // 내 콜라이더와 ECollisionGroup::Player에 있는 콜라이더들과 충돌처리
    if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player))
    {
    	// 충돌했다.

    	ObjectCollider->SetHit(true);	// 내 콜라이더 충돌
    	HitResult.HitCollision->SetHit(true);// 상대방 콜라이더 충돌

    	HitResult.HitCollision->GetOwner();  // 상대방 객체 접근
    }
}

void HeadHunter::Idle()
{
    image = ImageManager::GetInstance()->FindImage("Idle");
    if (frameIndex < image->GetMaxFrameX() - 1)
    {
          
        if (timer > 0.1f)
        {
            frameIndex++;
            timer = 0;
        }
    }


    if (frameIndex >= image->GetMaxFrameX() - 1)
    {
        frameIndex = 0;
        
        if (loop == 0) {
            ChangeState(State::GroundGun); // 또는 groundGun
        }
        if (loop == 1) {
            ChangeState(State::Dash);
        }
        
    }
}

void HeadHunter::GroundLazer()
{ 
    image = ImageManager::GetInstance()->FindImage("GroundLazer");
    if (frameIndex < image->GetMaxFrameX() - 1)
    {
        if (timer > 0.1f)
        {
            frameIndex++;
            timer = 0;
        }
    }


    if (frameIndex >= image->GetMaxFrameX() - 1)
    {
        frameIndex = image->GetMaxFrameX();
        lazer->SetIsActive(true);
        weaponAngle = 90;
        firePos = { Pos.x - 40, Pos.y + 10 };
        if (timer > 1.5f)
        {
            lazer->SetIsActive(false);
            weaponAngle = 0;
            frameIndex = 0;
            ChangeState(State::Teleport);
        }
        
    }
}

void HeadHunter::GroundGun()
{
    switch (gunWave)
    {
    case 0:
        image = ImageManager::GetInstance()->FindImage("GroundGun");
        if (frameIndex < image->GetMaxFrameX() - 1)
        {
            if (timer > 0.1f)
            {
                frameIndex++;
                timer = 0;
            }
        }

        if (frameIndex >= image->GetMaxFrameX() - 1)
        {
            frameIndex = 0;
            gunWave++;
        }
        break;

    case 1:
        image = ImageManager::GetInstance()->FindImage("GroundGun_shoot");

        if (frameIndex < image->GetMaxFrameX() - 1)
        {
            if (timer > 0.1f)
            {
                frameIndex++;
                timer = 0;
            }
        }
        else
        {
            // 애니메이션 한 번 끝났을 때
            gunCount++;

            if (gunCount < 3)
            {
                frameIndex = 0;  // 다시 처음부터 재생
            }
            else
            {
                frameIndex = image->GetMaxFrameX() - 1;
                gunCount = 0;  // 초기화
                ChangeState(State::Bullet);
            }
        }
        break;
    }
}

void HeadHunter::Bullet()
{
    switch (bulletWave)
    {
    case 0: // 점프
        
        image = ImageManager::GetInstance()->FindImage("Jump");
        if (timer > 0.004f)
        {
            
          float a = 180 + angle;
            
            Pos.x = wallPos.x + cosf(DEG_TO_RAD(a)) * jumpDist1.x; 
            Pos.y = wallPos.y + sinf(DEG_TO_RAD(a)) * jumpDist1.y;
            angle += 1;

            timer = 0;
        }

        if (Pos.x >= wallPos.x) // 840 : 벽 x좌표 임시값
        {
            Pos.x = wallPos.x;
            
            angle = 0;
            bulletWave++;
        }

        break;
    case 1: // 벽에 붙어있는 상태
        image = ImageManager::GetInstance()->FindImage("Jumpidle");

        if (timer > 0.1f)
        {
            frameIndex++;
            timer = 0;
            if (frameIndex >= image->GetMaxFrameX())
            {
                frameIndex = 0;
                bulletWave++;
            }
        }
        break;
    case 2: // 벽에서 천장으로 점프
        image = ImageManager::GetInstance()->FindImage("Jumpto");
        
        if (timer > 0.004f)
        {   
            float b = 360 + angle;
            Pos.x = (wallPos.x - jumpDist2.x) + cosf(DEG_TO_RAD(b)) * jumpDist2.x;
            Pos.y = (wallPos.y-jumpDist1.y) + sinf(DEG_TO_RAD(b)) * jumpDist2.y;
            angle -= 1;

            timer = 0;
        }
        if (Pos.x <= (wallPos.x - jumpDist2.x)) // 570 은 맵의 2/3 or 3/4 지점쯤으로 변경
        {

            Pos.x = (wallPos.x - jumpDist2.x);
            angle = 0;
            bulletWave++;
        }
        break;

    case 3: // 포물선 그리며 총알 발사 후 웨이브 전환
        image = ImageManager::GetInstance()->FindImage("Bullet");

        if (frameIndex < image->GetMaxFrameX())
        {
            if (timer > 0.1f)
            {
                frameIndex++;
                timer = 0;
            }

        }
        if (frameIndex >= image->GetMaxFrameX())
        {
            frameIndex = image->GetMaxFrameX() - 1;
        }           
        
        // 포물선 이동
        if (moveTimer > 0.004f)
        {
            float c = 270 + angle;
            Pos.x = (wallPos.x - jumpDist2.x) + cosf(DEG_TO_RAD(c)) * jumpDist3.x;
            Pos.y = wallPos.y + sinf(DEG_TO_RAD(c)) * jumpDist3.y;
            angle -= 1;

            moveTimer = 0;

            if (Pos.y >= 360)
            {
                Pos.y = 360;
                angle = 0;
                frameIndex = 0;
                dAngle = 0;
                loop = 1;
                ChangeState(State::Idle);
            }
        }
        //총알 각도 변환, 발사
        if (bulletTimer > 0.01f) 
        {
            if (dAngle <= 180)
			{
				FPOINT fire = { Pos.x + 40 * cosf(DEG_TO_RAD(dAngle)),Pos.y + 40 * sinf(DEG_TO_RAD(dAngle)) };
				SpawnBullet(fire, dAngle);
				dAngle += 10;
			}
            bulletTimer = 0;
        }

        break;
    }
}

void HeadHunter::Teleport()
{
    image = ImageManager::GetInstance()->FindImage("Teleport");
    if (timer > 0.1f)
    {
        frameIndex++;
        timer = 0;
    }

    if (frameIndex >= image->GetMaxFrameX() - 1)
    {
        frameIndex = image->GetMaxFrameX();
        ChangeState(State::VerticalLazer);
    }
}

void HeadHunter::VerticalLazer()
{
    firePos = { Pos.x, Pos.y + 35 };
    if (lazerLoop == 0) Pos.x = 340;
    if (lazerLoop == 1) Pos.x = 740;
    if (lazerLoop == 2) Pos.x = 440;
    if (lazerLoop == 3) Pos.x = 640;
    if (lazerLoop == 4) Pos.x = 540;
    switch (gunWave)
    {
    case 0:
        Pos.y = 200;

        
        image = ImageManager::GetInstance()->FindImage("VerticalLazer");
        if (frameIndex < image->GetMaxFrameX() - 1)
        {
            if (timer > 0.1f)
            {
                frameIndex++;
                timer = 0;
            }
        }

        if (frameIndex >= image->GetMaxFrameX() - 1)
        {   
            lazer->SetIsActive(true);
            frameIndex = image->GetMaxFrameX() - 1;
            if (timer > 0.1f)
            {
                lazer->SetIsActive(false);
                frameIndex = 0;
                gunWave++;
            }
        }
        break;

    case 1:
        image = ImageManager::GetInstance()->FindImage("VerticalLazer_out");

        if (frameIndex < image->GetMaxFrameX() - 1)
        {
            if (timer > 0.1f)
            {
                frameIndex++;
                timer = 0;
            }
        }
        
        if (frameIndex >= image->GetMaxFrameX() - 1)
        {
            frameIndex = -1;
            if (lazerLoop < 4)
            {
                gunWave = 0;
                lazerLoop++;
            }
            else
            {   
                lazer->SetIsActive(false);
                lazerLoop = 0;
                gunWave = 0;
                loop = 0;
                if (rand() % 5 < 3) // rand() 값이 고정적인데 어떡하지 
                {
                    
                    ChangeState(State::RoundLazer);
                }
                else
                {
                    ChangeState(State::RoundLazer);
                }

            }
        }
        break;
    }
}

void HeadHunter::RoundLazer()
{   
    float a = 180 + angle;
    float b = angle;
    image = ImageManager::GetInstance()->FindImage("RoundLazer");
    // 340으로 먼저 이동 후, 740으로 이동
    //340 일때 isFlip = true;
    if (loop == 0)
    {
        Pos.x = 340;
        
        firePos.x = Pos.x + 40 * cosf(DEG_TO_RAD(a));
        firePos.y = Pos.y + 40 * sinf(DEG_TO_RAD(a));
        weaponAngle = 90 + angle;
        
        isFlip = true;
        lazer->SetIsActive(true);
        if (frameIndex < image->GetMaxFrameX() - 1)
        {
		    if (timer > 0.05f)
		    {
			    frameIndex++;  
                angle -= 10;
                timer = 0;
		    }
        }

        if (frameIndex >= image->GetMaxFrameX() -1)
        {
            angle = 0;
            frameIndex = 0;
            loop = 1;
        }
    }
    if(loop == 1)
    {
        Pos.x = 740;

        firePos.x = Pos.x + 40 * cosf(DEG_TO_RAD(angle));
        firePos.y = Pos.y + 40 * sinf(DEG_TO_RAD(angle));
        weaponAngle = - 90 + angle;

        isFlip = false;
        if (frameIndex < image->GetMaxFrameX() - 1)
        {
            if (timer > 0.05f)
            {
                frameIndex++;
                angle += 10;
                timer = 0;
            }
        }

        if (frameIndex >= image->GetMaxFrameX()-1)
        {
                lazer->SetIsActive(false);
                angle = 0;
                weaponAngle = 0; //******
                ChangeState(State::DashDown);
        }
            
        
    }


}


void HeadHunter::Dash()
{
    switch (gunWave)
    {
    case 0:
        image = ImageManager::GetInstance()->FindImage("Readytodash");
        loop = 0;
        if (timer > 0.1f)
        {
            frameIndex++;
            timer = 0;
            if (frameIndex >= image->GetMaxFrameX())
            {
                frameIndex = 0;
                gunWave++;
            }
        }
        break;
    case 1:
        image = ImageManager::GetInstance()->FindImage("Dash");
        if (timer > 0.002f)
        {
            Pos.x += 5;
            timer = 0;

            if (Pos.x > playerPos.x + 50)
            {
                Pos.x = playerPos.x + 50;
                gunWave++;
            }
        }
        break;
    case 2:
        image = ImageManager::GetInstance()->FindImage("Dashend");
        if (timer > 0.1f)
        {

            frameIndex++;
            timer = 0;

            if (frameIndex >= image->GetMaxFrameX())
            {
                gunWave = 0;
                ChangeState(State::GroundLazer);
            }
        }
        break;
    }

}

void HeadHunter::DashDown()
{
    switch (gunWave)
    {
    case 0:
        image = ImageManager::GetInstance()->FindImage("Dash");

        if (timer > 0.005f)
        {
            Pos.y += 5;
            timer = 0;

            if (Pos.y > 360)
            {
                Pos.y = 360;
                gunWave++;
            }
        }
        break;
    case 1:
        image = ImageManager::GetInstance()->FindImage("Dashend");
        if (timer > 0.1f)
        {
            frameIndex++;
            timer = 0;

            if (frameIndex >= image->GetMaxFrameX())
            {
                gunWave = 0;
                bulletWave = 0;
                ChangeState(State::Bullet);
            }
        }
        break;
    }
}

void HeadHunter::Faint()
{
    image = ImageManager::GetInstance()->FindImage("Faint");
    if (frameIndex < image->GetMaxFrameX() - 1)
    {
        if (timer > 0.1f)
        {
            frameIndex++;
            Pos.x -= 10;
            timer = 0;
        }
    }

    if (frameIndex >= image->GetMaxFrameX() - 1)
    {
        frameIndex = image->GetMaxFrameX() - 1;
        if (timer > 0.5f)
        {
            frameIndex = image->GetMaxFrameX();
        }
    }


}

void HeadHunter::Run()
{
    if (Pos.y == 360 && abs(Pos.x - playerPos.x) <= 100)
    {
        if (timer > 0.05f)
        {
            Pos.x -= 3;
            timer = 0;
        }
    }
}

void HeadHunter::SpawnBullet(FPOINT firePos, float angle)
{
    Bullet1* newBullet = new Bullet1();
    newBullet->Init(firePos, angle);
    bullets.push_back(newBullet);

}

void HeadHunter::ChangeState(State newState) {
    state = newState;
    timer = 0;
    frameIndex = 0;
    jumpDist1.x = wallPos.x - Pos.x;
    jumpDist2.x = 250;
    jumpDist3.x = 290;

    jumpDist1.y = 100;
    jumpDist2.y = 150;
    jumpDist3.y = jumpDist1.y + jumpDist2.y;
    // 필요하면 이미지 변경, 방향 리셋 등도 여기에
}

void HeadHunter::PlayAnimation(string key)
{

    image = ImageManager::GetInstance()->FindImage(key);
    if (timer > 0.1f)
    {
        frameIndex++;
        timer = 0;
    }

    if (frameIndex >= image->GetMaxFrameX() - 1)
    {
        frameIndex = -1;
        NextWave();
    }
}

void HeadHunter::CheckPlayerPos()
{
    if (Pos.y >= 360) // 땅에 있을 때 기준으로 전환
    {
        if (Pos.x < playerPos.x)
        {
            isFlip = false;
        }
        else
        {
            isFlip = true;
        }
    }

}

void HeadHunter::NextWave()
{
    wave++;
    timer = 0;
    frameIndex = 0;

    switch (wave) {
    case 1:
        ChangeState(State::GroundLazer);
        break;
    case 2:
        ChangeState(State::Teleport /*Dash*/);
        break;
    case 3:
        ChangeState(State::GroundGun);
        break;
    case 4:
        ChangeState(State::Bullet);
        break;
    case 5:
        ChangeState(State::Faint);
        break;
    }
}
