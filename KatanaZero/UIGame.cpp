#include "UIGame.h"
#include "Image.h"
#include "RenderManager.h"
void UIGame::init()
{
	Image* image = ImageManager::GetInstance()->AddImage("battery_bar", L"Image/UI/spr_hud_battery.bmp", 154, 19, 2, 1, true, RGB(255, 0, 255));
	ani.Init(image, 2);
	ani.setPos({ WINSIZE_X / 2,50.f }, false);
	ani.setTask({ {0,3.f},{1,0.05f},{0,0.1f},{1,0.05f},{0,3.f},{1,0.1f} });
	ani.On();
	ani.Start();
}

void UIGame::Update()
{
	ani.Update();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
}

void UIGame::Render(HDC hdc)
{
	ani.Render(hdc);
}

void UIGame::Release()
{
}
