#include "UIGame.h"
#include "Image.h"
#include "RenderManager.h"
void UIGame::init()
{
	batteryPos = { 2,5 };
	TimerPos = { WINSIZE_X / 2.f,0 };

	for (int i = 0; i < BCELLCNT; i++)
	{

	}

	Image* image = ImageManager::GetInstance()->AddImage("UIbattery_bar", L"Image/UI/spr_hud_battery.bmp", 154 * (UISCALE), 19 * (UISCALE), 2, 1, true, RGB(255, 0, 255));
	battery.Init(image, 2);
	battery.setPos(batteryPos, false);
	battery.setTask({ {ONE,3.f},{TWO,0.05f},{ONE,0.1f},{TWO,0.05f},{ONE,2.f},{TWO,0.1f} });
	battery.On();
	battery.Start();

	image = ImageManager::GetInstance()->AddImage("UItimer", L"Image/UI/spr_hud_timer.bmp", 224 * (UISCALE), 19 * (UISCALE), 2, 1, true, RGB(255, 0, 255));
	Timer.Init(image, 2);
	Timer.setPos(TimerPos, false);
	Timer.setTask({ {ONE,3.f},{TWO,0.05f},{ONE,0.1f},{TWO,0.05f},{ONE,2.f},{TWO,0.1f} });
	Timer.On();
	Timer.Start();


	hud = ImageManager::GetInstance()->AddImage("UIhud", L"Image/UI/spr_hud.bmp", 640 * (UISCALE), 23 * (UISCALE), true, RGB(255, 0, 255));
}

void UIGame::Update()
{
	battery.Update();
	Timer.Update();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
}

void UIGame::Render(HDC hdc)
{
	
	hud->Render(hdc, 0, 0);
	battery.Render(hdc);
	Timer.Render(hdc);
}

void UIGame::Release()
{
}
