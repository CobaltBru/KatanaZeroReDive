#include "GoPopUp.h"
#include "ScrollManager.h"
#include "GPImage.h"
#include "Animation.h"
#include "Image.h"
#include "RenderManager.h"

GoPopUp::GoPopUp()
{
}

HRESULT GoPopUp::Init()
{
	Image* tmp = ImageManager::GetInstance()->AddImage("go_text", L"Image/UI/spr_go_text1.bmp",97,50,1,1,true,RGB(255,0,255));
	goImage = new Animation();
	goImage->Init(tmp, 1);
	goImage->setAniTask({ {0,10.f} });
	goImage->MoveOn({ 30.f,0.f }, 0.7f, Move_SoftEnd | Move_Loop);
	goImage->setPos({ 100.f,150.f }, false, true);
	goImage->On();

	tmp = ImageManager::GetInstance()->AddImage("go_arrow", L"Image/UI/spr_go_arrow1.bmp", 77, 47, 1, 1, true, RGB(255, 0, 255));
	arrowImage = new Animation();
	arrowImage->Init(tmp, 1);
	arrowImage->setAniTask({ {0,10.f} });
	arrowImage->MoveOn({ 30.f,0.f }, 0.7f, Move_SoftEnd | Move_Loop);
	arrowImage->setPos({ 100.f,200.f }, false, true);
	arrowImage->On();



	return S_OK;
}

void GoPopUp::Update()
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	goImage->Update();
	arrowImage->Update();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);

}

void GoPopUp::Render(HDC hdc)
{
	goImage->Render(hdc);
	arrowImage->Render(hdc);
}

void GoPopUp::Release()
{
	if (goImage)
	{
		goImage->Release();
		delete goImage;
		goImage = nullptr;
	}
	if (arrowImage)
	{
		arrowImage->Release();
		delete arrowImage;
		arrowImage = nullptr;
	}
}

void GoPopUp::On(FPOINT& player, FPOINT& dest)
{
}

void GoPopUp::Off()
{
}
