#include "Syringe.h"
#include "GPImage.h"
#include "ScrollManager.h"
#include "RenderManager.h"
#include "CommonFunction.h"
#include "Collider.h"
#include "CollisionManager.h"

Syringe::Syringe()
	:gpImage(nullptr)
{
}

HRESULT Syringe::Init(FPOINT InPos, float InAngle)
{
	wstring str = L"Image/Hidden/spr_psychboss_attack_knife_0.png";
	gpImage = new GPImage();
	gpImage->AddImage(str.c_str(), 1, 1);
	
	Pos = InPos;
	Angle = InAngle;

	ObjectCollider = new Collider(this, EColliderType::Rect, {},45.f, true, 0.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Projectile);
	ObjectCollider->SetPos(Pos);
	return S_OK;
}



void Syringe::Update()
{
	Pos.x += cosf(DEG_TO_RAD(Angle)) * 900.f * TimerManager::GetInstance()->GetDeltaTime();
	Pos.y += sinf(DEG_TO_RAD(Angle)) * 900.f * TimerManager::GetInstance()->GetDeltaTime();

	if (OutOfScreen(Pos, gpImage->getWidth() * 2.f, gpImage->getHeight() * 2.f))
		bDead = true;

	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Syringe::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	Gdiplus::Graphics graphics(hdc);
	gpImage->Middle_RenderAll(&graphics, Pos + Scroll, 0, Angle,false,1.f,1.f,1.f,1.f,2.f,2.f);
}

void Syringe::Release()
{
	if (gpImage != nullptr)
	{
		gpImage->Release();
		delete gpImage;
		gpImage = nullptr;
	}
}
