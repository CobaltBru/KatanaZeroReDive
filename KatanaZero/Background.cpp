#include "Background.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"

Background::Background()
	:Image(nullptr), ScrollPercent(1.f), Scale(1.f)
{
}

HRESULT Background::Init(string InImageKey, float InScrollPercent, float InScale)
{	
	ImageName = InImageKey;
	ScrollPercent = InScrollPercent;
	Scale = InScale;
	Image = ImageManager::GetInstance()->FindImage(InImageKey);
	return S_OK;
}

void Background::Update()
{
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::BackGround, this);
}

void Background::Render(HDC hdc)
{
	if (Image != nullptr)
	{
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		Image->Render(hdc, Pos.x + (Scroll.x * ScrollPercent), Pos.y + (Scroll.y * ScrollPercent), Scale);
	}		
}

void Background::Release()
{
}
