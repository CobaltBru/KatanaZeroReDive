#include "UIGame.h"
#include "Image.h"
#include "RenderManager.h"
void UIGame::init()
{
	
	

	
	Image* image = nullptr;

	//battery
	{
		batteryPos = { 2.f,5.f };

		vector<pair<int, float>> batteryTask = { { {ONE,3.f},{TWO,0.05f},{ONE,0.1f},{TWO,0.05f},{ONE,2.f},{TWO,0.1f} } };
		image = ImageManager::GetInstance()->AddImage("UIbattery_bar", L"Image/UI/spr_hud_battery.bmp", 154.f * (UISCALE), 19.f * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		battery.Init(image, 2);
		battery.setPos(batteryPos, false, false);
		battery.setTask(batteryTask);
		battery.On();
		battery.Start();

		image = ImageManager::GetInstance()->AddImage("UIBatteryCellBlue", L"Image/UI/spr_battery_cell_blue.bmp", 8.f * (UISCALE), 10.f * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		for (int i = 0; i < BCELLCNT; i++)
		{
			batteryCellPos[i] = { batteryPos.x + 25.f + i * (image->GetFrameWidth() + 3.f),batteryPos.y + 10.f };
		}

		for (int i = 0; i < BCELLCNT; i++)
		{

			batteryCellBlue[i].Init(image, 2);
			batteryCellBlue[i].setPos(batteryCellPos[i], false, false);
			batteryCellBlue[i].setTask(batteryTask);
			batteryCellBlue[i].On();
			batteryCellBlue[i].Start();
		}

		image = ImageManager::GetInstance()->AddImage("UIBatteryCellRed", L"Image/UI/spr_battery_cell_red.bmp", 8.f * (UISCALE), 10.f * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		for (int i = 0; i < BCELLCNT; i++)
		{

			batteryCellRed[i].Init(image, 2);
			batteryCellRed[i].setPos(batteryCellPos[i], false, false);
			batteryCellRed[i].setTask(batteryTask);
			batteryCellRed[i].On();
			batteryCellRed[i].Start();
		}

		image = ImageManager::GetInstance()->AddImage("UIShift", L"Image/UI/spr_keyboard_shift.bmp", 60.f * (UISCALE), 14.f * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		shiftButton.Init(image, 2);
		shiftButton.setPos({ batteryPos.x + 190.f, batteryPos.y + 5.f }, false, false);
		shiftButton.setTask({ {ONE, 0.f},{TWO,0.f} });
		shiftButton.On();
	
	
	}

	//timer
	{
		timerPos = { WINSIZE_X / 2.f,0 };
		vector<pair<int, float>> timerTask = { 
			{ONE,3.f},{TWO,0.05f},
			{ONE,0.1f},{TWO,0.05f},
			{ONE,2.f},{TWO,0.1f},
			{ONE,0.05f},{TWO,0.05f},
			{ONE,0.05f},{TWO,0.1f} };
		image = ImageManager::GetInstance()->AddImage("UITimer", L"Image/UI/spr_hud_timer.bmp", 224 * (UISCALE), 19 * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		timerPos.x -= (image->GetFrameWidth() / 2.f + 12.f);
		timerUI.Init(image, 2);
		timerUI.setPos(timerPos, false, false);
		timerUI.setTask(timerTask);
		timerUI.On();
		timerUI.Start();

		image = ImageManager::GetInstance()->AddImage("UITimerBar", L"Image/UI/spr_timer_bar.bmp", 188 * (UISCALE), 11 * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		timerBarPos = { timerPos.x + 42.f, timerPos.y + 5.f };
		timerBarUI.Init(image, 2);
		timerBarUI.setPos(timerBarPos, false,false);
		timerBarUI.setTask(timerTask);
		timerBarUI.On();
		timerBarUI.Start();

	}
	//itemslot
	{
		slotPos = { WINSIZE_X , 0 };
		image = ImageManager::GetInstance()->AddImage("UISlot", L"Image/UI/spr_hud_subweapon.bmp", 112 * (UISCALE), 24 * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		slotPos.x -= (image->GetFrameWidth() + 20.f);
		itemSlot.Init(image, 2);
		itemSlot.setPos(slotPos, false);
		itemSlot.setTask({ {ONE,3.f},{TWO,0.05f},{ONE,0.1f},{TWO,0.05f},{ONE,1.f},{TWO,0.1f} });
		itemSlot.On();
		itemSlot.Start();

		mouseLeft = ImageManager::GetInstance()->AddImage("UImouseLeft", L"Image/UI/spr_left_click.bmp", 13 * (UISCALE), 17.f * (UISCALE), true, RGB(255, 0, 255));
		mouseRight = ImageManager::GetInstance()->AddImage("UImouseRight", L"Image/UI/spr_right_click.bmp", 13 * (UISCALE), 17.f * (UISCALE), true, RGB(255, 0, 255));

		item1Pos = { slotPos.x + 5.f, slotPos.y + 4.f };
		item2Pos = { slotPos.x + 82.5f, slotPos.y + 4.f };
		item1 = ImageManager::GetInstance()->AddImage("UIKatana", L"Image/UI/spr_katanaicons.bmp", 20 * (UISCALE), 20 * (UISCALE), true, RGB(255, 0, 255));
		itemKey.push_back("UIKatana");
		item2 = ImageManager::GetInstance()->AddImage("UIHand", L"Image/UI/spr_itemicons.bmp", 20 * (UISCALE), 20 * (UISCALE), true, RGB(255, 0, 255));
		itemKey.push_back("UIHand");
	}
	hud = ImageManager::GetInstance()->AddImage("UIhud", L"Image/UI/spr_hud.bmp", 640 * (UISCALE), 23 * (UISCALE), true, RGB(255, 0, 255));
}

void UIGame::Update()
{
	battery.Update();
	
	for (int i = 0; i < BCELLCNT; i++)
	{
		batteryCellBlue[i].Update();
		batteryCellRed[i].Update();
	}
	timerBarUI.setSour(0.0f, 0.3f);
	timerUI.Update();
	
	timerBarUI.Update();
	
	itemSlot.Update();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
}

void UIGame::Render(HDC hdc)
{
	
	hud->Render(hdc, 0, 0);
	battery.Render(hdc);
	for (int i = 0; i < BCELLCNT; i++)
	{
		//batteryCellBlue[i].Render(hdc);
		batteryCellRed[i].Render(hdc);
	}
	shiftButton.Render(hdc);

	timerUI.Render(hdc);
	timerBarUI.Render(hdc);
	itemSlot.Render(hdc);

	item1->Render(hdc, item1Pos.x, item1Pos.y);
	item2->Render(hdc, item2Pos.x, item2Pos.y);

	mouseLeft->Render(hdc, slotPos.x + 40.f, slotPos.y + 40.f);
	mouseRight->Render(hdc, slotPos.x + 120.f, slotPos.y + 40.f);
}

void UIGame::Release()
{
}
