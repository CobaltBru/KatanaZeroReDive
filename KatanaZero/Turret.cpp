#include "Turret.h"
#include "Bullet.h"
#include "Collider.h"
#include "Image.h"
#include "GPImage.h"

#include "CollisionManager.h"
#include "ScrollManager.h"
#include "ObjectManager.h"
#include "SnapShotManager.h"

Turret::Turret()
{
}

Turret::~Turret()
{
}

HRESULT Turret::Init()
{
    Pos = { 100,100 }; // player pos player ���� pos �� �ٸ������� ���;��� > �ȱ׷��� ó������ �浹ó���Ǿ �� �ν���
    timer = 0;
    frameIndex = 0;

    state = TurretState::Spawn;

    isDead = false;
    isActive = false;
    isDestroyed = false;
    isOperated = false;

    player = SnapShotManager::GetInstance()->GetPlayer();

    turretImage = ImageManager::GetInstance()->AddImage("Turret", L"Image/HeadHunter/turret.bmp", 1600, 100, 16, 1, true, RGB(255, 0, 255));
    turretImage = ImageManager::GetInstance()->AddImage("TurretDie", L"Image/HeadHunter/turretdie.bmp", 1400, 100, 14, 1, true, RGB(255, 0, 255)); // turret���� 10 �о �����ϱ�
    
    turretImage = ImageManager::GetInstance()->AddImage("TurretHolder", L"Image/HeadHunter/turretholder.bmp", 100, 100, 1, 1, true, RGB(255, 0, 255));

    headImage = new GPImage();
    headImage->AddImage(L"Image/HeadHunter/turrethead2.png");

    backImage = new GPImage();
    backImage->AddImage(L"Image/HeadHunter/turretback.png");

    frontImage = new GPImage();
    frontImage->AddImage(L"Image/HeadHunter/turretfront.png");


    //wallImage = ImageManager::GetInstance()->AddImage(); // �ð� ������ �� �����°� ����

    turretImage = ImageManager::GetInstance()->FindImage("Turret");

    ObjectCollider = new Collider(this, EColliderType::Rect, {-25.f * ScrollManager::GetInstance()->GetScale(),+25.f * ScrollManager::GetInstance()->GetScale() }, 50.f * ScrollManager::GetInstance()->GetScale(), false, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
    ObjectCollider->SetPos(Pos);

    
    return S_OK;
}

void Turret::Release()
{
    for (auto bullet : bullets) {
        bullet->Release();
        delete bullet;
    }
    bullets.clear();
}

void Turret::Update(FPOINT pos)
{
    timer += TimerManager::GetInstance()->GetDeltaTime();
    Pos = pos;

    if (isActive)
    {
        

        if (!isDestroyed) // �̰� �ɾ���� ������ �Ⱥν�����
        {
            if (ObjectCollider->IsHitted())
            {
                frameIndex = 0;
                isDestroyed = true;
                state = TurretState::Die;
            }
        }

        switch (state)
        {
        case TurretState::Spawn:
            
            turretImage = ImageManager::GetInstance()->FindImage("Turret");
            if (frameIndex < turretImage->GetMaxFrameX() - 1)
            {
                if (timer > 0.1f)
                {
                    frameIndex++;
                    timer = 0;
                }
            }

            if (frameIndex >= turretImage->GetMaxFrameX() - 1)
            {
                frameIndex = 0;
                state = TurretState::Operate;
            }
            break;

        case TurretState::Operate:
            Collision();
            turretImage = ImageManager::GetInstance()->FindImage("TurretHolder");
            isOperated = true;

            angle = RAD_TO_DEG(atan2f(player->GetPos().y - (Pos.y + 8 * 2.5f), player->GetPos().x - (Pos.x + 15.f * 2.5f)));
            
            if (angle >= 27.f)
            {
                angle = 27.f;
            }

            if (angle <= -27.f)
            {
                angle = -27.f;
            }
            if (timer > 0.5f)
            {
                SpawnBullet({ (Pos.x - 12.f * 2.5f) + 35.f* ScrollManager::GetInstance()->GetScale() *cosf(DEG_TO_RAD(angle)),
                    (Pos.y+ 8*2.5f) + 35.f * ScrollManager::GetInstance()->GetScale()* sinf(DEG_TO_RAD(angle))}, angle);
                timer = 0;
            }

            break;
        case TurretState::Die:
            isOperated = false;
            turretImage = ImageManager::GetInstance()->FindImage("TurretDie");
            if (frameIndex < turretImage->GetMaxFrameX() - 1)
            {
                if (timer > 0.1f)
                {
                    frameIndex++;
                    timer = 0;
                }
            }

            if (frameIndex >= turretImage->GetMaxFrameX() - 1)
            {
                frameIndex >= turretImage->GetMaxFrameX() - 1;
            }
            break;
        }
    }
}

void Turret::Render(HDC hdc)
{ 
    Gdiplus::Graphics graphics(hdc);
    if (isActive)
    {  
        if (isOperated)
        {
			if (backImage)
			{
				backImage->Middle_RenderAll(&graphics, Pos, 0, 0, false, 1.0f, 1.0f, 1.0f, 1.0f, ScrollManager::GetInstance()->GetScale(),ScrollManager::GetInstance()->GetScale());
			}

			if (headImage)
			{
                headImage->Middle_RenderAll(&graphics, { Pos.x - 20 * ScrollManager::GetInstance()->GetScale(), Pos.y + 8 * ScrollManager::GetInstance()->GetScale() }, 0, angle, false, 1.0f, 1.0f, 1.0f, 1.0f, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
			}

			if (frontImage)
			{
				frontImage->Middle_RenderAll(&graphics, Pos, 0, 0, false, 1.0f, 1.0f, 1.0f, 1.0f, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
			}
        }
        

        if (turretImage)
        {
            turretImage->FrameRender(hdc, Pos.x, Pos.y, frameIndex, 0, false, true, ScrollManager::GetInstance()->GetScale());
        }

        for (auto bullet : bullets) {
            bullet->Render(hdc);
        }



    }

   

}

void Turret::Collision()
{
    // �浹 ����
    FHitResult HitResult;

    // �� �ݶ��̴��� ECollisionGroup::Enemy�� �ִ� �ݶ��̴���� �浹ó��
    if (CollisionManager::GetInstance()->CollisionAABB(ObjectCollider, HitResult, ECollisionGroup::Player)) // �÷��̾� ���� �÷��̾��� ���ݰ� ����Ϸ���?
    {
        // �浹�ߴ�.

        ObjectCollider->SetHit(true);	// �� �ݶ��̴� �浹
        HitResult.HitCollision->SetHit(true);// ���� �ݶ��̴� �浹

        HitResult.HitCollision->GetOwner();  // ���� ��ü ����

        FPOINT pos;
        pos.x = HitResult.HitCollision->GetPos().x - ObjectCollider->GetPos().x;
        pos.y = HitResult.HitCollision->GetPos().y - ObjectCollider->GetPos().y;
    }
}

void Turret::SpawnBullet(FPOINT firePos, float angle)
{
    Bullet1* newBullet = new Bullet1();
    newBullet->Init(firePos, angle);
    ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, newBullet);
    //bullets.push_back(newBullet);

}
