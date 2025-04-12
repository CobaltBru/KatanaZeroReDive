#include "Background.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"

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
		Image->Render(hdc);
}

void Background::Release()
{
}
