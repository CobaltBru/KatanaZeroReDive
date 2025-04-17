#include "Background.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"

Background::Background()
	:Image(nullptr)
{
}

HRESULT Background::Init(string InImageKey)
{	
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
		Image->Render(hdc, Scroll.x, Scroll.y);
	}
		
}

void Background::Release()
{
}
