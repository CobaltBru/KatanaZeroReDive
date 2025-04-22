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
	angleMode = false;
	isActive = false;
	flip = false;
	oldFlip = false;
	popUpPos = { 0,0 };


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

	/*angleArrow = new GPImage();
	angleArrow->AddImage(L"Image/UI/spr_go_arrow_big.png");*/

	return S_OK;
}

void GoPopUp::Update()
{
	if (isActive)
	{
		calcPos();
		ArrowCalc();
		
		goImage->setPos(popUpPos, false, true);
		arrowImage->setPos({ popUpPos.x,popUpPos.y + 50.f }, flip, true);
		goImage->Update();
		arrowImage->Update();
	}
	
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);

}

void GoPopUp::Render(HDC hdc)
{
	if (isActive)
	{
		goImage->Render(hdc);
		arrowImage->Render(hdc);

		//Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hdc);
		////angleArrow->Render(pGraphics, tmpPos);
		//delete pGraphics;
	}
	
	
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
	/*if (angleArrow)
	{
		angleArrow->Release();
		delete angleArrow;
		angleArrow = nullptr;
	}*/
}

void GoPopUp::On(FPOINT* player, FPOINT* dest)
{
	isActive = true;
	playerPos = player;
	destPos = dest;
	calcPos();
	oldFlip = !flip;
}

void GoPopUp::Off()
{
	isActive = false;
}

void GoPopUp::calcPos()
{
	if (playerPos->x > destPos->x) // ¿ÞÂÊ
	{
		flip = true;
		popUpPos.x = 100.f;
	}
	else //¿À¸¥ÂÊ
	{
		flip = false;
		popUpPos.x = WINSIZE_X - 100.f;
	}
	popUpPos.y = destPos->y;
	angleMode = false;
	if (popUpPos.y <= 150.f)
	{
		angleMode = true;
		popUpPos.y = 150.f;
	}
	if (popUpPos.y >= WINSIZE_Y-150.f)
	{
		angleMode = true;
		popUpPos.y = WINSIZE_Y - 150.f;
	}

	if (oldFlip != flip)
	{
		oldFlip = flip;
		if (flip)
		{
			goImage->MoveOn({ -30.f,0.f }, 0.7f, Move_SoftEnd | Move_Loop);
			arrowImage->MoveOn({ -30.f,0.f }, 0.7f, Move_SoftEnd | Move_Loop);
		}
		else
		{
			goImage->MoveOn({ 30.f,0.f }, 0.7f, Move_SoftEnd | Move_Loop);
			arrowImage->MoveOn({ 30.f,0.f }, 0.7f, Move_SoftEnd | Move_Loop);
		}
	}
}

void GoPopUp::ArrowCalc()
{

}
