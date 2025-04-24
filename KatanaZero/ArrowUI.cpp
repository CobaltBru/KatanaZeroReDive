#include "ArrowUI.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"

ArrowUI::ArrowUI()
	:Image(nullptr), CurrentTime(0.f), FrameX(0), bVisible(false), Angle(0.f), MoveY(0.f)
{
}

HRESULT ArrowUI::Init()
{
	Image = ImageManager::GetInstance()->FindImage("spr_pickuparrow_anim");

	return S_OK;
}

void ArrowUI::Update()
{
	if (bVisible)
	{
		Angle += 300.f * TimerManager::GetInstance()->GetDeltaTime();
		MoveY += 50.f * sinf(DEG_TO_RAD(Angle)) * TimerManager::GetInstance()->GetDeltaTime();
	}

	CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
	if (CurrentTime >= 0.1f)
	{
		CurrentTime = 0.f;

		++FrameX;
		if (Image->GetMaxFrameX() <= FrameX)
			FrameX = 0;
	}

	if (bVisible)
		RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
}

void ArrowUI::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	Image->FrameRender(hdc, Pos.x + Scroll.x, Pos.y + MoveY + Scroll.y, FrameX, 0,false,true, ScrollManager::GetInstance()->GetScale());
}

void ArrowUI::Release()
{
	Image = nullptr;
}
