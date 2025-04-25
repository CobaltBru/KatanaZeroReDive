#include "UIGame.h"
#include "Image.h"
#include "RenderManager.h"
#include "ParticleEffect.h"
#include "SoundManager.h"

HRESULT UIGame::Init()
{
	
	Image* image = nullptr;
	timer = 0.f;
	//battery
	{
		batteryPos = { 2.f,5.f };
		batteryGage = 1.0f;
		vector<pair<int, float>> batteryTask = { { {ONE,3.f},{TWO,0.05f},{ONE,0.1f},{TWO,0.05f},{ONE,2.f},{TWO,0.1f} } };
		image = ImageManager::GetInstance()->AddImage("UIbattery_bar", L"Image/UI/spr_hud_battery.bmp", 154.f * (UISCALE), 19.f * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		battery.Init(image, 2);
		battery.setPos(batteryPos, false, false);
		battery.setAniTask(batteryTask);
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
			batteryCellBlue[i].setAniTask(batteryTask);
			batteryCellBlue[i].On();
			batteryCellBlue[i].Start();
		}

		image = ImageManager::GetInstance()->AddImage("UIBatteryCellRed", L"Image/UI/spr_battery_cell_red.bmp", 8.f * (UISCALE), 10.f * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		for (int i = 0; i < BCELLCNT; i++)
		{

			batteryCellRed[i].Init(image, 2);
			batteryCellRed[i].setPos(batteryCellPos[i], false, false);
			batteryCellRed[i].setAniTask(batteryTask);
			batteryCellRed[i].On();
			batteryCellRed[i].Start();
		}

		image = ImageManager::GetInstance()->AddImage("UIShift", L"Image/UI/spr_keyboard_shift.bmp", 60.f * (UISCALE), 14.f * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		shiftButton.Init(image, 2);
		shiftButton.setPos({ batteryPos.x + 190.f, batteryPos.y + 5.f }, false, false);
		shiftButton.setAniTask({ {ONE, 0.f},{TWO,0.f} });
		shiftButton.On();
	
		part1 = new ParticleEffect();
		part1->Init();
		part1->SetPos({ batteryPos.x + 8.f,batteryPos.y + 20.f });
		part2 = new ParticleEffect();
		part2->Init();
		part2->SetPos({ batteryPos.x + 178.f,batteryPos.y + 20.f });
	}

	//timer
	{
		timerPos = { WINSIZE_X / 2.f,0 };
		timeGage = 1.0f;
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
		timerUI.setAniTask(timerTask);
		timerUI.On();
		timerUI.Start();

		image = ImageManager::GetInstance()->AddImage("UITimerBar", L"Image/UI/spr_timer_bar.bmp", 188 * (UISCALE), 11 * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		timerBarPos = { timerPos.x + 42.f, timerPos.y + 5.f };
		timerBarUI.Init(image, 2);
		timerBarUI.setPos(timerBarPos, false,false);
		timerBarUI.setAniTask(timerTask);
		timerBarUI.On();
		timerBarUI.Start();
		//timerBarUI.MoveOn({ 0,30.f }, 1.0f, (Move_SoftStart | Move_Loop));

	}
	//itemslot
	{
		slotPos = { WINSIZE_X , 0 };
		image = ImageManager::GetInstance()->AddImage("UISlot", L"Image/UI/spr_hud_subweapon.bmp", 112 * (UISCALE), 24 * (UISCALE), 2, 1, true, RGB(255, 0, 255));
		slotPos.x -= (image->GetFrameWidth() + 20.f);
		itemSlot.Init(image, 2);
		itemSlot.setPos(slotPos, false);
		itemSlot.setAniTask({ {ONE,3.f},{TWO,0.05f},{ONE,0.1f},{TWO,0.05f},{ONE,1.f},{TWO,0.1f} });
		itemSlot.On();
		itemSlot.Start();

		mouseLeft = ImageManager::GetInstance()->AddImage("UImouseLeft", L"Image/UI/spr_left_click.bmp", 13 * (UISCALE), 17.f * (UISCALE), true, RGB(255, 0, 255));
		mouseRight = ImageManager::GetInstance()->AddImage("UImouseRight", L"Image/UI/spr_right_click.bmp", 13 * (UISCALE), 17.f * (UISCALE), true, RGB(255, 0, 255));

		item1Pos = { slotPos.x + 5.f, slotPos.y + 4.f };
		item2Pos = { slotPos.x + 82.5f, slotPos.y + 4.f };
		item1 = ImageManager::GetInstance()->AddImage("UIKatana", L"Image/UI/spr_katanaicons.bmp", 20 * (UISCALE), 20 * (UISCALE), true, RGB(255, 0, 255));
		leftItem = "UIKatana";
		item2 = ImageManager::GetInstance()->AddImage("UIHand", L"Image/UI/spr_itemicons.bmp", 20 * (UISCALE), 20 * (UISCALE),1,1, true, RGB(255, 0, 255));
		rightItem = "UIHand";
	}
	hud = ImageManager::GetInstance()->AddImage("UIhud", L"Image/UI/spr_hud.bmp", 640 * (UISCALE), 23 * (UISCALE), true, RGB(255, 0, 255));
	
	Item2Scale = 1.f;
	Item2FrameX = 0;

	isSlow = false;


	return S_OK;
}

void UIGame::Update()
{
	timer += TimerManager::GetInstance()->GetDeltaTime(false);
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_SHIFT))
	{
		
		shiftButton.setFrame(TWO);
		if (timer >= 0.1f)
		{
			timer = 0.f;
			batteryGage -= 0.02f;
			timeGage -= 0.02f;
			batteryGage = max(batteryGage, 0.0f);
			timeGage = max(timeGage, 0.0f);
		}
		isSlow = true;
	}
	else
	{
		
		shiftButton.setFrame(ONE);
		if (timer >= 0.1f)
		{
			timer = 0.f;
			batteryGage += 0.01f;
			timeGage += 0.01f;
			batteryGage = min(batteryGage, 1.0f);
			timeGage = min(timeGage, 1.0f);
		}
		isSlow = false;
	}
	
	battery.Update();
	if (isSlow)
	{
		part1->Update();
		part2->Update();
	}
	
	for (int i = 0; i < BCELLCNT; i++)
	{
		batteryCellBlue[i].Update();
		batteryCellRed[i].Update();
	}
	timerBarUI.setSour(0.0f, timeGage);
	timerUI.Update();
	
	timerBarUI.Update();
	
	itemSlot.Update();
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
}

void UIGame::Render(HDC hdc)
{
	
	hud->Render(hdc, 0, 0);
	battery.Render(hdc);
	if (isSlow)
	{
		part1->Render(hdc);
		part2->Render(hdc);
	}
	
	for (int i = 0; i < (BCELLCNT * batteryGage); i++)
	{
		batteryCellBlue[i].Render(hdc);
	}
	for (int i = (BCELLCNT * batteryGage); i < BCELLCNT; i++)
	{
		batteryCellRed[i].Render(hdc);
	}
	shiftButton.Render(hdc);

	timerUI.Render(hdc);
	timerBarUI.Render(hdc);
	itemSlot.Render(hdc);

	item1->Render(hdc, item1Pos.x, item1Pos.y);
	item2->FrameRender(hdc, item2Pos.x, item2Pos.y, Item2FrameX, 0, false, false, Item2Scale);

	mouseLeft->Render(hdc, slotPos.x + 40.f, slotPos.y + 40.f);
	mouseRight->Render(hdc, slotPos.x + 120.f, slotPos.y + 40.f);
}

void UIGame::Release()
{
	if (part1)
	{
		part1->Release();
		delete part1;
		part1 = nullptr;
	}
	if (part2)
	{
		part2->Release();
		delete part2;
		part2 = nullptr;
	}
	
}

void UIGame::EventPlayerState(const ObsPlayerState& ps)
{
	batteryGage = ps.battery;
	if (leftItem != ps.leftItem && ps.leftItem != "")
	{
		leftItem = ps.leftItem;
		item1 = ImageManager::GetInstance()->FindImage(leftItem);
	}
	if (rightItem != ps.rightItem && ps.rightItem != "")
	{
		rightItem = ps.rightItem;
		item2 = ImageManager::GetInstance()->FindImage(rightItem);
	}

	isSlow = ps.isSlow;
	batteryGage = ps.battery;
}

void UIGame::TimerUIEvent(const float t)
{
	timeGage = t;
}

void UIGame::SetRightItem(string InImageKey, FPOINT InOffset, float InFrameX, float InScale)
{
	if (InImageKey == "")
	{
		item2 = ImageManager::GetInstance()->FindImage("UIHand");
		Item2FrameX = 0;
		Item2Scale = 1;
		item2Pos = { slotPos.x + 82.5f, slotPos.y + 4.f };
		return;
	}

	item2 = ImageManager::GetInstance()->FindImage(InImageKey);
	Item2FrameX = InFrameX;
	Item2Scale = InScale;
	item2Pos = { slotPos.x + 82.5f + InOffset.x, slotPos.y + 4.f + InOffset.y};
}
