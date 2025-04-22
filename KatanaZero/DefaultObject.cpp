#include "DefaultObject.h"
#include "ImageManager.h"
#include "ScrollManager.h"
#include "Collider.h"
#include "CollisionManager.h"

DefaultObject::DefaultObject()
	:Image(nullptr), bFlip(false)
{
}

HRESULT DefaultObject::Init(string InImageKey, FPOINT InPos, bool InFlip, ERenderGroup InRenderGroup, string InClassName)
{
	Pos = InPos;
	bFlip = InFlip;
	RenderGroup = InRenderGroup;
	ClassName = InClassName;
	ImageName = InImageKey;
	Image = ImageManager::GetInstance()->FindImage(InImageKey);

	ObjectCollider = new Collider(this, EColliderType::Rect, {}, { (float)Image->GetFrameWidth() * ScrollManager::GetInstance()->GetScale(),(float)Image->GetFrameHeight() * ScrollManager::GetInstance()->GetScale() }, false, 1.f);
	CollisionManager::GetInstance()->AddCollider(ObjectCollider, ECollisionGroup::Player);

	ObjectCollider->SetPos(Pos);

	return S_OK;
}

void DefaultObject::Update()
{
	if (ObjectCollider != nullptr)
		ObjectCollider->SetDebugDraw(false);

	if (Image != nullptr)
		RenderManager::GetInstance()->AddRenderGroup(RenderGroup, this);
}

void DefaultObject::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0, bFlip,true, ScrollManager::GetInstance()->GetScale());
}

void DefaultObject::Release()
{
}
