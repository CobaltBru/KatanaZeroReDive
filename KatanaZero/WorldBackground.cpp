#include "WorldBackground.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"

HRESULT WorldBackground::Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup)
{
    //여긴 콜라이더,리지드바디 없다.

	Image = ImageManager::GetInstance()->FindImage(InImageKey);
	Pos = InPos;

	RenderGroup = InRenderGroup;
	bFlip = InFlip;

    return S_OK;
}

void WorldBackground::Update()
{
	RenderManager::GetInstance()->AddRenderGroupFront(ERenderGroup::NonAlphaBlend, this);
}

void WorldBackground::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + Scroll.y, 0, 0, false, true, ScrollManager::GetInstance()->GetScale());
}
