#include "Turret.h"
#include "Collider.h"
#include "Image.h"
#include "CollisionManager.h"

Turret::Turret()
{
}

Turret::~Turret()
{
}

HRESULT Turret::Init(FPOINT pos)
{
    timer = 0;
    frameIndex = 0;

    isDead = false;

    //turretImage = ImageManager::GetInstance()->AddImage();
    //wallImage = ImageManager::GetInstance()->AddImage();

    ObjectCollider = new Collider(this, EColliderType::Rect, {}, 50.f, true, 1.f);
    CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Enemy);
    ObjectCollider->SetPos(Pos);

    
    return S_OK;
}

void Turret::Release()
{
    if (turretImage)
    {
        turretImage->Release();
        delete turretImage;
        turretImage = nullptr;
    }

	if (wallImage)
	{
        wallImage->Release();
		delete wallImage;
        wallImage = nullptr;
	}
    
}

void Turret::Update()
{
}

void Turret::Render(HDC hdc)
{
}
