#include "HeadHunter.h"
#include "Image.h"
#include "RenderManager.h"
#include "CommonFunction.h"
#include "Lazer.h"
#include "Bullet.h"

HeadHunter::HeadHunter()
{
}

HeadHunter::~HeadHunter()
{
}

HRESULT HeadHunter::Init()
{
    pos = { 460, 360 };
    firePos = { 0,0 };

    isFlip = false;
    isAttacked = false;
    isDead = false;

    angle = 0;
    weaponAngle = 0;
    timer = 0;
    indexTimer = 0;
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

    lazer = new Lazer();
    lazer->Init();

    bullet = new Bullet1();
    bullet->Init();

    return S_OK;
}

void HeadHunter::Release()
{
    lazer->Release();
    bullet->Release();
}

void HeadHunter::Update()
{
    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
    timer += TimerManager::GetInstance()->GetDeltaTime();
    indexTimer += TimerManager::GetInstance()->GetDeltaTime();

    lazer->Update(firePos, weaponAngle);

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
        image->FrameRender(hdc, pos.x, pos.y, frameIndex, 0, isFlip);
        lazer->Render(hdc);
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
        firePos = { pos.x - 40, pos.y + 10 };
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
        if (timer > 0.01f)
        {
            pos.x += cosf(DEG_TO_RAD(0)) * 5; 
            pos.y -= sinf(DEG_TO_RAD(angle + 90)) * 5;
            angle -= 5;


            timer = 0;
        }

        if (pos.x >= 840) // 840 : 벽 x좌표 임시값
        {
            pos.x = 840;
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

        if (timer > 0.01f)
        {
            pos.x -= cosf(DEG_TO_RAD(0)) * 5;
            pos.y -= sinf(DEG_TO_RAD(angle + 90)) * 5;
            angle -= 2;

            timer = 0;
        }
        if (pos.x <= 570)
        {

            pos.x = 570;
            bulletWave++;
        }
        break;

    case 3: // 포물선 그리며 총알 발사 후 웨이브 전환
        image = ImageManager::GetInstance()->FindImage("Bullet");
        if (indexTimer > 0.1f)
        {
            frameIndex++;
            indexTimer = 0;
        }
        if (timer > 0.01f)
        {
            pos.x -= cosf(DEG_TO_RAD(0)) * 5;
            pos.y += abs(sinf(DEG_TO_RAD(angle + 90))) * 5;
            angle -= 2;

            timer = 0;

            if (frameIndex >= image->GetMaxFrameX())
            {
                frameIndex = image->GetMaxFrameX();
            }
            if (pos.y >= 360)
            {
                pos.y = 360;
                angle = 0;
                frameIndex = 0;
                loop = 1;
                ChangeState(State::Idle);
            }

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
    firePos = { pos.x, pos.y + 35 };
    if (lazerLoop == 0) pos.x = 340;
    if (lazerLoop == 1) pos.x = 740;
    if (lazerLoop == 2) pos.x = 440;
    if (lazerLoop == 3) pos.x = 640;
    if (lazerLoop == 4) pos.x = 540;
    switch (gunWave)
    {
    case 0:
        pos.y = 200;

        
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
        pos.x = 340;
        
        firePos.x = pos.x + 40 * cosf(DEG_TO_RAD(a));
        firePos.y = pos.y + 40 * sinf(DEG_TO_RAD(a));
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
        pos.x = 740;

        firePos.x = pos.x + 40 * cosf(DEG_TO_RAD(angle));
        firePos.y = pos.y + 40 * sinf(DEG_TO_RAD(angle));
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
            pos.x += 5;
            timer = 0;

            if (pos.x > playerPos.x + 50)
            {
                pos.x = playerPos.x + 50;
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
            pos.y += 5;
            timer = 0;

            if (pos.y > 360)
            {
                pos.y = 360;
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
            pos.x -= 10;
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
    if (pos.y == 360 && abs(pos.x - playerPos.x) <= 100)
    {
        if (timer > 0.05f)
        {
            pos.x -= 3;
            timer = 0;
        }
    }
}

void HeadHunter::ChangeState(State newState) {
    state = newState;
    timer = 0;
    frameIndex = 0;
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
    if (pos.y >= 360) // 땅에 있을 때 기준으로 전환
    {
        if (pos.x < playerPos.x)
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
