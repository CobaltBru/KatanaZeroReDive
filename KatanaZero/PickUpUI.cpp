#include "PickUpUI.h"
#include "Image.h"
#include "ImageManager.h"
#include "ScrollManager.h"

PickUpUI::PickUpUI()
	: Image(nullptr), CurrentTime(0.f), FlashTime(0.f), IsShow(false), TargetPos(nullptr), Offset({0.f,0.f})
{
}

HRESULT PickUpUI::Init(string InImageKey, FPOINT* InPos, FPOINT InOffset, bool InFlip)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey);

	TargetPos = InPos;
	Offset = InOffset;
	bFlip = InFlip;
	return S_OK;
}

void PickUpUI::Update()
{
	if (IsShow)
	{
		CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
	
		if (CurrentTime <= 3.f)
		{
			FlashTime += TimerManager::GetInstance()->GetDeltaTime();
			if (FlashTime <= 0.25f)
				RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
			else if (FlashTime >= 0.5f)
				FlashTime = 0.f;				
		}
		else
			IsShow = false;
	}
}

void PickUpUI::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	const float NewScale = Scale* ScrollManager::GetInstance()->GetScale();
	Image->FrameRender(hdc, (TargetPos->x + Offset.x) + Scroll.x, (TargetPos->y + Offset.y - (Image->GetFrameHeight() * NewScale) * 0.75f) + Scroll.y, 1, 0, bFlip, true, NewScale);
}

void PickUpUI::Release()
{
	TargetPos = nullptr;
}

void PickUpUI::SetImage(string InImageKey)
{
	Image = ImageManager::GetInstance()->FindImage(InImageKey);
}

void PickUpUI::Start()
{
	CurrentTime = 0.f;
	FlashTime = 0.f;
	IsShow = true;
}
