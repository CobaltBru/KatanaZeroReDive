#include "ArmAttack.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "Image.h"
#include "ImageManager.h"
#include "ScrollManager.h"

ArmAttack::ArmAttack()
	:Target(nullptr), Image(nullptr)
{
}

HRESULT ArmAttack::Init()
{
	Image = ImageManager::GetInstance()->FindImage("spr_psychboss_stabber_0");
	Target = CollisionManager::GetInstance()->GetTargetCollider(ECollisionGroup::Player)->GetOwner();
	Pos.x = WINSIZE_X / 2;
	Pos.y = WINSIZE_Y / 2;
	return S_OK;
}

void ArmAttack::Update()
{
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void ArmAttack::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0, false, true, ScrollManager::GetInstance()->GetScale());
}

void ArmAttack::Release()
{
}
