#include "HeadHunter.h"
#include "Image.h"
#include "RenderManager.h"
#include "CommonFunction.h"
#include "Lazer.h"
#include "Bullet.h"
#include "Bomb.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "Player.h"
#include "SnapShotManager.h"
#include "ObjectManager.h"
#include "ScrollManager.h"

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
    isLeft = true;
    bCanSpawnBomb = false;
    faintStart = true;

    angle = 0;
    weaponAngle = 0;
    dAngle = 0;
    timer = 0;
    moveTimer = 0;
    bulletTimer = 0;

    hp = 3;
    wave = 0;
    bulletWave = 0;
    dashWave = 0;
    lazerWave = 0;
    gunWave = 0;

    loop = 0;
    lazerLoop = 0;
    gunCount = 0;
    frameIndex = 0;
    deadCount = 0;
    dieIndex = 0;

    state = State::GroundGun;

    {
        image = ImageManager::GetInstance()->AddImage("Idle", L"Image/HeadHunter/headhunter_idle.bmp", 840, 70  , 12, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->AddImage("GroundLazer", L"Image/HeadHunter/headhunter_takeoutrifle.bmp", 560  , 70  , 8, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("RoundLazer", L"Image/HeadHunter/headhunter_lazer.bmp", 972  , 46  , 18, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Teleport", L"Image/HeadHunter/teleport1.bmp", 280  , 70  , 4, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("VerticalLazer", L"Image/HeadHunter/teleport2.bmp", 280  , 70  , 4, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("VerticalLazer_out", L"Image/HeadHunter/teleport3.bmp", 280  , 70  , 4, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->AddImage("Faint", L"Image/HeadHunter/headhunter_faint.bmp", 495  , 45  , 11, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->AddImage("GroundGun", L"Image/HeadHunter/GroundGun.bmp", 490  , 70  , 7, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("GroundGun_shoot", L"Image/HeadHunter/GroundGun_shoot.bmp", 800  , 70  , 8, 1, true, RGB(255, 0, 255));

        //BULLET
        image = ImageManager::GetInstance()->AddImage("Jump", L"Image/HeadHunter/headhunter_jump.bmp", 27  , 44  , 1, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Jumpidle", L"Image/HeadHunter/headhunter_jumpidle.bmp", 90  , 43  , 3, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Jumpto", L"Image/HeadHunter/headhunter_jumptoceil.bmp", 35  , 46  , 1, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Bullet", L"Image/HeadHunter/headhunter_shoot.bmp", 385  , 55  , 7, 1, true, RGB(255, 0, 255));

        //DASH
        image = ImageManager::GetInstance()->AddImage("Readytodash", L"Image/HeadHunter/readytodash.bmp", 432  , 70  , 9, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Dash", L"Image/HeadHunter/dash.bmp", 51  , 25  , 1, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Dashend", L"Image/HeadHunter/dashend.bmp", 500  , 70  , 10, 1, true, RGB(255, 0, 255));

        //Dead
        image = ImageManager::GetInstance()->AddImage("DeadFly", L"Image/HeadHunter/deadfly.bmp", 280  , 70  , 4, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("DeadLand", L"Image/HeadHunter/deadland.bmp", 560  , 70  , 8, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->FindImage("Idle");
    }

    ObjectCollider = new Collider(this, EColliderType::Rect, {0,10.f}, { 20.f   ,40.f   }, true, 10.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
    ObjectCollider->SetPos(Pos);

    ObjectRigidBody = new RigidBody(this);

    player = SnapShotManager::GetInstance()->GetPlayer().front();

    lazer = new Lazer();
    lazer->Init();

    return S_OK;
}

HRESULT HeadHunter::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
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
    isLeft = true;
    bCanSpawnBomb = false;
    isFired = false;
    isFlipLocked = false;

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
    deadCount = 0;
    dieIndex = 0;

    state = State::Idle;

    {
        image = ImageManager::GetInstance()->AddImage("Idle", L"Image/HeadHunter/headhunter_idle.bmp", 840, 50, 12, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->AddImage("GroundLazer", L"Image/HeadHunter/headhunter_takeoutrifle.bmp", 560, 45, 8, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("RoundLazer", L"Image/HeadHunter/headhunter_lazer.bmp", 972, 46, 18, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Teleport", L"Image/HeadHunter/teleport1.bmp", 280, 37, 4, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("VerticalLazer", L"Image/HeadHunter/teleport2.bmp", 280, 50, 4, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("VerticalLazer_out", L"Image/HeadHunter/teleport3.bmp", 280, 50, 4, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->AddImage("Faint", L"Image/HeadHunter/headhunter_faint.bmp", 495, 50, 11, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->AddImage("GroundGun", L"Image/HeadHunter/GroundGun.bmp", 490, 47, 7, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("GroundGun_shoot", L"Image/HeadHunter/GroundGun_shoot.bmp", 800, 47, 8, 1, true, RGB(255, 0, 255));

        //BULLET
        image = ImageManager::GetInstance()->AddImage("Jump", L"Image/HeadHunter/headhunter_jump.bmp", 27, 44, 1, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Jumpidle", L"Image/HeadHunter/headhunter_jumpidle.bmp", 90, 43, 3, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Jumpto", L"Image/HeadHunter/headhunter_jumptoceil.bmp", 35, 46, 1, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Bullet", L"Image/HeadHunter/headhunter_shoot.bmp", 385, 55, 7, 1, true, RGB(255, 0, 255));

        //DASH
        image = ImageManager::GetInstance()->AddImage("Readytodash", L"Image/HeadHunter/readytodash.bmp", 432, 45, 9, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Dash", L"Image/HeadHunter/dash.bmp", 51, 25, 1, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("Dashend", L"Image/HeadHunter/dashend.bmp", 500, 50, 10, 1, true, RGB(255, 0, 255));

        //Dead
        image = ImageManager::GetInstance()->AddImage("DeadFly", L"Image/HeadHunter/deadfly.bmp", 280, 30, 4, 1, true, RGB(255, 0, 255));
        image = ImageManager::GetInstance()->AddImage("DeadLand", L"Image/HeadHunter/deadland.bmp", 560, 45, 8, 1, true, RGB(255, 0, 255));

        image = ImageManager::GetInstance()->FindImage("Idle");
    }

    ObjectCollider = new Collider(this, EColliderType::Rect, InColliderOffset, InColliderSize, true, 3.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
    ObjectCollider->SetPos(Pos);

    ObjectRigidBody = new RigidBody(this);

    player = SnapShotManager::GetInstance()->GetPlayer().front();

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

    for (auto bomb : bombs) {
        bomb->Release();
        delete bomb;
    }
    bombs.clear();

    if (ObjectRigidBody != nullptr)
    {
        delete ObjectRigidBody;
        ObjectRigidBody = nullptr;
    }
}

void HeadHunter::Update()
{
    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
    timer += TimerManager::GetInstance()->GetDeltaTime();
    moveTimer += TimerManager::GetInstance()->GetDeltaTime();
    bulletTimer += TimerManager::GetInstance()->GetDeltaTime();

    Collision();

    if (state == State::Idle || state == State::Dash || state == State::GroundGun || state == State::Faint || state == State::Die || state == State::Bullet || state == State::DashDown) ObjectRigidBody->Update();

    lazer->Update(firePos, weaponAngle); 

    //for (auto bullet : bullets) {
    //    bullet->Update();
    //}

    /*for (auto bomb : bombs) {
        bomb->Update();
    }*/
    
    
   
    // 테스트
    if (KeyManager::GetInstance()->IsStayKeyDown('M'))
    {
        ObjectCollider->SetHit(true);
    }



    IsAttacked();// 맞았을 때

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
    
    case State::Die:
        Die();
        break;
    }

}

void HeadHunter::Render(HDC hdc)
{
    if (image)
    {
        image->FrameRender(hdc, Pos.x, Pos.y, frameIndex, 0, isFlip, true, ScrollManager::GetInstance()->GetScale());

        lazer->Render(hdc);

        for (auto bullet : bullets) {
            bullet->Render(hdc);
        }

        for (auto bomb : bombs) {
            bomb->Render(hdc);
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
    	HitResult.HitCollision->SetHit(false);// 상대방 콜라이더 충돌

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
            ChangeState(State::GroundLazer); // 또는 groundlazer
        }
        if (loop == 1) {
            ChangeState(State::GroundGun);
        }
        if (loop == 2) {
            ChangeState(State::GroundGun); //dash
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
        frameIndex = image->GetMaxFrameX() - 1;
        lazer->SetIsActive(true);

        if (!isFired) // 처음 바라본 곳을 고정하도록
        {
			if (Pos.x > player->GetPos().x)
			{
               
				weaponAngle = 90;
				firePos = { Pos.x - 40, Pos.y - 10 };
			}
			if (Pos.x < player->GetPos().x)
			{
                
				weaponAngle = -90;
				firePos = { Pos.x + 40, Pos.y - 10 };
			}
            
            isFired = true;
        }
        

        if (timer > 1.5f)
        {
            lazer->SetIsActive(false);
            isFired = false;
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
        image = ImageManager::GetInstance()->FindImage("GroundGun"); // 준비동작
        bCanSpawnBomb = true;
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
        image = ImageManager::GetInstance()->FindImage("GroundGun_shoot"); // 쏜다
        if (bCanSpawnBomb)
        {
            SpawnBomb(Pos, 0);
            bCanSpawnBomb = false;
        }

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
            bCanSpawnBomb = true;
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
    case 0: // 오른쪽 벽으로 점프
        image = ImageManager::GetInstance()->FindImage("Jump");
        const FLineResult Result = ObjectRigidBody->GetResult();
   
        ObjectRigidBody->SetMaxVelocity({2000.f,1000.f});
        ObjectRigidBody->SetVelocity({ 500.f, -250.f }); 

        if (Result.LineType == ELineType::Wall)
        {
            bulletWave++;
        }
        break;

    case 1: // 벽에 붙어 대기
        image = ImageManager::GetInstance()->FindImage("Jumpidle");
        isFlip = true;
        ObjectRigidBody->SetGravityVisible(false);
        ObjectRigidBody->SetVelocity({ 0.f, -0.f });
        if (timer > 0.1f)
        {
            frameIndex++;
            timer = 0;
            if (frameIndex >= image->GetMaxFrameX())
            {
                frameIndex = 0;
                ObjectRigidBody->SetGravityVisible(true);
                bulletWave++;
            }
        }
        break;

    case 2: // 천장으로 점프
        image = ImageManager::GetInstance()->FindImage("Jumpto");
        ObjectRigidBody->SetVelocity({ -700.f, -250.f }); // 왼쪽 위 점프

        Pos = GetPos();
        
        if (Pos.x <= 800)
        {
            bulletWave++;
        }
        break;

    case 3: // 포물선 + 총알 발사
        image = ImageManager::GetInstance()->FindImage("Bullet");

        ObjectRigidBody->SetVelocity({ -300.f, 500.f }); // 포물선처럼 아래로

        Pos = GetPos();

        // 착지 후 상태 종료
        const FLineResult result = ObjectRigidBody->GetResult();
        if (result.LineType == ELineType::Normal)
        {
            angle = 0;
            frameIndex = 0;
            dAngle = 0;
            
            ChangeState(State::Idle);
        }

        // 총알 발사
        if (bulletTimer > 0.01f)
        {
            if (dAngle <= 180)
            {
                FPOINT fire = { Pos.x + 40 * cosf(DEG_TO_RAD(dAngle)), Pos.y + 40 * sinf(DEG_TO_RAD(dAngle)) };
                SpawnBullet(fire, dAngle);
                dAngle += 10;
            }
            bulletTimer = 0;
        }

        // 애니메이션
        if (moveTimer > 0.1f && frameIndex < image->GetMaxFrameX())
        {
            frameIndex++;
            moveTimer = 0;
        }

        break;
    }
   // switch (bulletWave)
   // {
   // case 0: // 점프
   //     
   //     image = ImageManager::GetInstance()->FindImage("Jump");
   //     if (timer > 0.004f)
   //     {
   //         
   //       float a = 180 + angle;
   //         
   //         Pos.x = wallPos.x + cosf(DEG_TO_RAD(a)) * jumpDist1.x; 
   //         Pos.y = wallPos.y + sinf(DEG_TO_RAD(a)) * jumpDist1.y;
   //         angle += 1;

   //         timer = 0;
   //     }

   //     if (Pos.x >= wallPos.x) // 840 : 벽 x좌표 임시값
   //     {
   //         Pos.x = wallPos.x;
   //         
   //         angle = 0;
   //         bulletWave++;
   //     }

   //     break;
   // case 1: // 벽에 붙어있는 상태
   //     image = ImageManager::GetInstance()->FindImage("Jumpidle");

   //     if (timer > 0.1f)
   //     {
   //         frameIndex++;
   //         timer = 0;
   //         if (frameIndex >= image->GetMaxFrameX())
   //         {
   //             frameIndex = 0;
   //             bulletWave++;
   //         }
   //     }
   //     break;
   // case 2: // 벽에서 천장으로 점프
   //     image = ImageManager::GetInstance()->FindImage("Jumpto");
   //     
   //     if (timer > 0.004f)
   //     {   
   //         float b = 360 + angle;
   //         Pos.x = (wallPos.x - jumpDist2.x) + cosf(DEG_TO_RAD(b)) * jumpDist2.x;
   //         Pos.y = (wallPos.y-jumpDist1.y) + sinf(DEG_TO_RAD(b)) * jumpDist2.y;
   //         angle -= 1;

   //         timer = 0;
   //     }
   //     if (Pos.x <= (wallPos.x - jumpDist2.x)) // 570 은 맵의 2/3 or 3/4 지점쯤으로 변경
   //     {

   //         Pos.x = (wallPos.x - jumpDist2.x);
   //         angle = 0;
   //         bulletWave++;
   //     }
   //     break;

   // case 3: // 포물선 그리며 총알 발사 후 웨이브 전환
   //     image = ImageManager::GetInstance()->FindImage("Bullet");
   //     
   //     if (frameIndex < image->GetMaxFrameX())
   //     {
   //         if (timer > 0.1f)
   //         {
   //             frameIndex++;
   //             timer = 0;
   //         }

   //     }
   //     if (frameIndex >= image->GetMaxFrameX())
   //     {
   //         frameIndex = image->GetMaxFrameX() - 1;
   //     }           
   //     
   //     // 포물선 이동
   //     if (moveTimer > 0.004f)
   //     {
   //         float c = 270 + angle;
   //         Pos.x = (wallPos.x - jumpDist2.x) + cosf(DEG_TO_RAD(c)) * jumpDist3.x;
   //         Pos.y = wallPos.y + sinf(DEG_TO_RAD(c)) * jumpDist3.y;
   //         angle -= 1;

   //         moveTimer = 0;

   //         if (Pos.y >= 360)
   //         {
   //             Pos.y = 360;
   //             angle = 0;
   //             frameIndex = 0;
   //             dAngle = 0;
   //             loop = 1;
   //             RandomLoop();
   //             ChangeState(State::Idle);
   //         }
   //     }
   //     //총알 각도 변환, 발사
   //     if (bulletTimer > 0.01f) 
   //     {
   //         if (dAngle <= 180)
			//{
			//	FPOINT fire = { Pos.x + 40 * cosf(DEG_TO_RAD(dAngle)),Pos.y + 40 * sinf(DEG_TO_RAD(dAngle)) };
			//	SpawnBullet(fire, dAngle);
			//	dAngle += 10;
			//}
   //         bulletTimer = 0;
   //     }

   //     break;
   // }
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
    if (loop == 1)
    {
        Pos.x = 740;

        firePos.x = Pos.x + 40 * cosf(DEG_TO_RAD(angle));
        firePos.y = Pos.y + 40 * sinf(DEG_TO_RAD(angle));
        weaponAngle = -90 + angle;

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

        if (frameIndex >= image->GetMaxFrameX() - 1)
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
                IsLeft();
            }
        }
        break;
    case 1:
        image = ImageManager::GetInstance()->FindImage("Dash");
        
        //if (timer > 0.002f)
        //{
           const FLineResult Result = ObjectRigidBody->GetResult();
           if (isLeft)
           {
               
               ObjectRigidBody->SetVelocity({ 200.f,0.f });
               
                //timer = 0;

                if (Result.LineType == ELineType::Wall) // 벽에 닿거나
                {
                    gunWave++;
                }
                
           }
           if (!isLeft)
           {
               //Pos.x -= 5;
               ObjectRigidBody->AddVelocity({ -200.f,0.f });
               timer = 0;
               
               if (Result.LineType == ELineType::Wall)
               {
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
                RandomLoop();
                ChangeState(State::Idle);
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
        const FLineResult Result = ObjectRigidBody->GetResult();
    
        ObjectRigidBody->SetVelocity({ 0.f,600.f });

        if (Result.LineType == ELineType::Normal)
        {
            gunWave++;
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
                RandomLoop();
                ChangeState(State::Idle);
            }
        }
        break;
    }
}

void HeadHunter::Faint()
{
    image = ImageManager::GetInstance()->FindImage("Faint");

    if (faintStart)
    {
        // 밀리는 힘은 한 번만 적용
        if (isLeft)
        {
            ObjectRigidBody->AddVelocity({ -100.f, 0.f });
            isFlip = true;
        }
        else
        {
            ObjectRigidBody->AddVelocity({ +100.f, 0.f });
            isFlip = false;
        }

        faintStart = false;
        frameIndex = 0;
        timer = 0;
    }

    if (frameIndex < image->GetMaxFrameX()-1)
    {
        if (timer > 0.1f)
        {
            frameIndex++;
            timer = 0;
        }
    }

    if (frameIndex >= image->GetMaxFrameX()-1)
    {
        frameIndex = image->GetMaxFrameX() - 1;

        // 조금 더 누워있는 듯한 연출 후 깨어나기
        if (timer > 1.0f)
        {
            moveTimer = 0;
            hp--;
            if (hp == 3) ChangeState(State::Die);
            ChangeState(State::Idle);
            faintStart = true; // 다음에 다시 쓸 수 있게 초기화
        }
    }


}


void HeadHunter::Die()
{
    switch (dieIndex)
    {
    case 0:
        image = ImageManager::GetInstance()->FindImage("DeadFly");

        if (frameIndex < image->GetMaxFrameX() - 1)
        {
            if (timer > 0.1f)
            {
                frameIndex++;
                timer = 0;
            }
        }

        if (frameIndex > image->GetMaxFrameX() - 1)
        {
            frameIndex = 0;
        }

        //const FLineResult Result = ObjectRigidBody->GetResult();
        //if (Result.LineType == ELineType::Normal)
        //{
        //    frameIndex = 0;
        //    timer = 0;
        //    dieIndex++;
        //}

    case 1:
        image = ImageManager::GetInstance()->FindImage("DeadLand");
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
            frameIndex = image->GetMaxFrameX() - 1;
        }
    }
    
   

}

void HeadHunter::SpawnBullet(FPOINT firePos, float angle)
{
    Bullet1* newBullet = new Bullet1();
    newBullet->Init(firePos, angle);
    ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, newBullet);
    //bullets.push_back(newBullet);

}

void HeadHunter::SpawnBomb(FPOINT firePos, float angle)
{
    Bomb* newBomb = new Bomb();
    newBomb->Init(firePos, angle);
    ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, newBomb);
    //bombs.push_back(newBomb);
}

void HeadHunter::ChangeState(State newState) {
    state = newState;

    isFlip = false;

    timer = 0;
    frameIndex = 0;
    gunWave = 0;
    bulletWave = 0;
    jumpDist1.x = wallPos.x - Pos.x;
    jumpDist2.x = 250;
    jumpDist3.x = 290;

    jumpDist1.y = 100;
    jumpDist2.y = 150;
    jumpDist3.y = jumpDist1.y + jumpDist2.y;

    ObjectRigidBody->SetMaxVelocity({ 200.f,600.f });
    ObjectRigidBody->SetVelocity({ 0.f,0.f });

    lazer->SetIsActive(false);
    
}


void HeadHunter::CheckPlayerPos()
{
    const FLineResult Result = ObjectRigidBody->GetResult();

    if (state == State::GroundLazer || state == State::Bullet ||
        state == State::RoundLazer || state == State::DashDown || state == State::VerticalLazer)
    {
        if (!isFlipLocked && Result.LineType == ELineType::Normal)
        {
            isFlip = (Pos.x >= player->GetPos().x); // 바라보는 방향 고정
            isFlipLocked = true;
        }
    }
    else
    {
        // 일반 상태에서는 계속 따라감
        if (Result.LineType == ELineType::Normal)
        {
            isFlip = (Pos.x >= player->GetPos().x);
        }

        isFlipLocked = false; // 다른 상태 들어오면 다시 해제
    }

    //if (state != State::Bullet && state != State::DashDown && state != State::RoundLazer && state != State::VerticalLazer && state != State::GroundLazer && state != State::Teleport) {
    //   if (Result.LineType == ELineType::Normal) // 땅에 있을 때 기준으로 이미지전환
    //    {
    //        if (Pos.x < player->GetPos().x)
    //        {
    //            isFlip = false;
    //        }
    //        else
    //        {
    //            isFlip = true;
    //        }
    //    }
    //}
 

}

void HeadHunter::RandomLoop()
{
    loop = rand() % 3;
}

void HeadHunter::IsLeft()
{
    if (Pos.x >= player->GetPos().x)
    {
        isLeft = false;
    }
    if (Pos.x < player->GetPos().x)
    {
        isLeft = true;
    }
}

void HeadHunter::IsAttacked()
{
    if (ObjectCollider->IsHitted())
    {
        ObjectCollider->SetHit(false); // 한 번만 반응하게
        ChangeState(State::Faint);
    }

}

