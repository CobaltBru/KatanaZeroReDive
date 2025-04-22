#include "HomeScene.h"
#include "CommonFunction.h"

#include "Background.h"
#include "Animation.h"
#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "ScrollManager.h"
#include "GPImage.h"
#include "Image.h"
#include "SoundManager.h"
#include "ChatManager.h"

HomeScene::HomeScene():
	ObjectManager(nullptr), RenderManager(nullptr), wall(nullptr)
{
	timerStart = false;
	sceneChangeTimer = 0;
}

HRESULT HomeScene::Init()
{
	SetClientRect(g_hWndParent, WINSIZE_X, WINSIZE_Y);

	ObjectManager = ObjectManager::GetInstance();
	ObjectManager->Init();
	RenderManager = RenderManager::GetInstance();
	RenderManager->Init();

	cursor = 0;
	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("HomeScene InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("HomeScene InitObject Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	SoundManager::GetInstance()->PlayBGM("HomeOST");
	return S_OK;
}

void HomeScene::Release()
{
	if (ObjectManager != nullptr)
		ObjectManager->Release();
	if (RenderManager != nullptr)
		RenderManager->Release();
	
	ScrollManager::GetInstance()->Release();

	ObjectManager = nullptr;
	RenderManager = nullptr;
}

void HomeScene::Update()
{
	ObjectManager->Update();
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		if (cursor == 0)
			nextSceneEffect();
		else if (cursor == 2)
			nextSceneEffect();
	}
	if (timerStart)
	{
		sceneChangeTimer += TimerManager::GetInstance()->GetDeltaTime();
	}
	if (sceneChangeTimer >= 2.1f)
	{
		if(cursor == 0)
			SceneManager::GetInstance()->ChangeScene("Test", "로딩_1");
		else if(cursor == 2)
			PostMessage(g_hWnd, WM_CLOSE, 0, 0);
		timerStart = false;
		sceneChangeTimer = 0;
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		if (cursor - 1 >= 0)
			--cursor;
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('S'))
	{
		if (cursor + 1 < 3)
			++cursor;
	}
}

void HomeScene::Render(HDC hdc)
{
	RenderManager->Render(hdc);
	SelectBox(hdc, { selectBoxPos });
}

HRESULT HomeScene::InitImage()
{
	ImageManager::GetInstance()->AddImage("BlackBackground", L"Image/Background/black.bmp", 1600, 900, 1, 1);
	return S_OK;
}

HRESULT HomeScene::InitObject()
{
	Background* bg = new Background();
	bg->Init("BlackBackground");
	ObjectManager->AddGameObject(EObjectType::GameObject, bg);

	Image* tmp = ImageManager::GetInstance()->AddImage("title_bg", L"Image/UI/Home/spr_title_background.bmp",1600,1800,1,1, true, RGB(255, 0, 255));
	background = new Animation();
	background->Init(tmp, 1);
	background->setPos({ 0.f,-400.f }, false, false);
	background->setAniTask({ {0,10.f} });
	background->MoveOn({ 0.f,-500.f }, 1.0f, Move_SoftEnd | Move_Stop);
	background->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, background);

	tmp = ImageManager::GetInstance()->AddImage("title_fence", L"Image/UI/Home/spr_title_fence.bmp", 1600, 1800, 1, 1, true, RGB(255, 0, 255));
	fence = new Animation();
	fence->Init(tmp, 1);
	fence->setPos({ 0.f,-400.f }, false, false);
	fence->setAniTask({ {0,10.f} });
	fence->MoveOn({ 0.f,-500.f }, 1.05f, Move_SoftEnd | Move_Stop);
	fence->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, fence);

	

	GPImage* gpi = new GPImage();
	gpi->AddImage(L"Image/UI/Home/spr_zer.png", 2, 1);
	zero_zer = new Animation();
	zero_zer->Init(gpi, 2);
	zero_zer->setPos({ WINSIZE_X/2 - 80.f,WINSIZE_Y+100.f }, false, true);
	zero_zer->setAniTask({
		{0,1.0f},{1,0.01f},
		{0,0.05f},{1,0.01f} ,
		{0,0.05f},{1,0.01f} ,
		{0,2.f},{1,0.01f},
		{0,1.5f} });
	zero_zer->MoveOn({ 0.f,-WINSIZE_Y/2 - 165.f }, 1.0f, Move_SoftEnd | Move_Stop);
	zero_zer->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, zero_zer);

	gpi = new GPImage();
	gpi->AddImage(L"Image/UI/Home/spr_o.png", 2, 1);
	zero_o = new Animation();
	zero_o->Init(gpi, 2);
	zero_o->setPos({ WINSIZE_X / 2 + 203.f,WINSIZE_Y-2.f + 100.f}, false, true);
	zero_o->setAniTask({
		{0,2.0f},{1,0.5f},
		{0,0.05f},{1,0.01f} ,
		{0,0.05f},{1,0.01f},
		{0,4.f}});
	zero_o->MoveOn({ 0.f,-WINSIZE_Y / 2 - 165.f }, 1.0f, Move_SoftEnd | Move_Stop);
	zero_o->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, zero_o);

	gpi = new GPImage();
	gpi->AddImage(L"Image/UI/Home/Select_bg.png", 1, 1);
	selectBoxPos = { WINSIZE_X / 2-20.f,WINSIZE_Y + 400.f };
	select_bg = new Animation();
	select_bg->Init(gpi, 2);
	select_bg->setPos(selectBoxPos, false, true);
	select_bg->setAniTask({
		{0,10.f}});
	select_bg->MoveOn({ 0.f,-600.f}, 2.0f, Move_SoftEnd | Move_Stop);
	select_bg->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, select_bg);

	gpi = new GPImage();
	gpi->AddImage(L"Image/UI/Home/newGame.png", 3, 1);
	buttons[0] = new Animation();
	buttons[0]->Init(gpi, 2);
	buttons[0]->setPos({ selectBoxPos.x,selectBoxPos.y-110.f }, false, true);
	buttons[0]->setAniTask({
		{1,0.05f},{2,0.05f},
		{1,0.05f},{2,0.05f}, 
		{1,0.05f},{2,0.05f} });
	buttons[0]->MoveOn({ 0.f,-600.f }, 2.0f, Move_SoftEnd | Move_Stop);
	buttons[0]->On();
	buttons[0]->Stop();
	buttons[0]->setFrame(0);
	ObjectManager->AddGameObject(EObjectType::GameObject, buttons[0]);

	gpi = new GPImage();
	gpi->AddImage(L"Image/UI/Home/continue.png", 3, 1);
	buttons[1] = new Animation();
	buttons[1]->Init(gpi, 2);
	buttons[1]->setPos({ selectBoxPos.x,selectBoxPos.y }, false, true);
	buttons[1]->setAniTask({
		{1,0.05f},{2,0.05f},
		{1,0.05f},{2,0.05f},
		{1,0.05f},{2,0.05f} });
	buttons[1]->MoveOn({ 0.f,-600.f }, 2.0f, Move_SoftEnd | Move_Stop);
	buttons[1]->On();
	buttons[1]->Stop();
	buttons[1]->setFrame(0);
	ObjectManager->AddGameObject(EObjectType::GameObject, buttons[1]);

	gpi = new GPImage();
	gpi->AddImage(L"Image/UI/Home/quit.png", 3, 1);
	buttons[2] = new Animation();
	buttons[2]->Init(gpi, 2);
	buttons[2]->setPos({ selectBoxPos.x,selectBoxPos.y + 110.f }, false, true);
	buttons[2]->setAniTask({
		{1,0.05f},{2,0.05f},
		{1,0.05f},{2,0.05f},
		{1,0.05f},{2,0.05f} });
	buttons[2]->MoveOn({ 0.f,-600.f }, 2.0f, Move_SoftEnd | Move_Stop);
	buttons[2]->On();
	buttons[2]->Stop();
	buttons[2]->setFrame(0);
	ObjectManager->AddGameObject(EObjectType::GameObject, buttons[2]);

	gpi = new GPImage();
	gpi->AddImage(L"Image/UI/Home/spr_title_katana.png", 1, 1);
	katana = new Animation();
	katana->Init(gpi, 2);
	katana->setPos({ WINSIZE_X / 2,WINSIZE_Y - 100.f + 100.f }, false, true);
	katana->setAniTask({
		{0,10.f} });
	katana->MoveOn({ 0.f,-WINSIZE_Y / 2 - 165.f }, 1.0f, Move_SoftEnd | Move_Stop);
	katana->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, katana);


	tmp = ImageManager::GetInstance()->AddImage("title_grass1", L"Image/UI/Home/spr_grass1.bmp", 1600, 300, 1, 1, true, RGB(255, 0, 255));
	grass1 = new Animation();
	grass1->Init(tmp, 1);
	grass1->setPos({ 0.f,WINSIZE_Y }, false, false);
	grass1->setAniTask({ {0,10.f} });
	grass1->MoveOn({ 0.f,-200.f }, 1.3f, Move_SoftEnd | Move_Stop);
	grass1->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, grass1);

	tmp = ImageManager::GetInstance()->AddImage("title_grass2", L"Image/UI/Home/spr_grass2.bmp", 19200, 637, 12, 1, true, RGB(255, 0, 255));
	grass2 = new Animation();
	grass2->Init(tmp, 1);
	grass2->setPos({ 0.f,WINSIZE_Y }, false, false);
	float fs = 0.1f;
	grass2->setAniTask({
		{0,fs},{1,fs},{2,fs},
		{3,fs},{4,fs},{5,fs},
		{6,fs},{7,fs},{8,fs},
		{9,fs},{10,fs},{11,fs}, });
	grass2->MoveOn({ 0.f,-637.f }, 1.3f, Move_SoftEnd | Move_Stop);
	grass2->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, grass2);

	tmp = ImageManager::GetInstance()->FindImage("BlackBackground");
	wall = new Animation();
	wall->Init(tmp, 1);
	wall->setPos({ 0.f,WINSIZE_Y }, false, false);
	wall->setAniTask({ {0,10.f} });
	ObjectManager->AddGameObject(EObjectType::GameObject, wall);

	
	return S_OK;
}

void HomeScene::SelectBox(HDC& hdc, FPOINT bpos)
{
	for (int i = 0; i < 3; i++)
	{
		if (i == cursor)
		{
			buttons[i]->setFrame(1);
		}
		else
		{
			buttons[i]->setFrame(0);
		}
	}
}

void HomeScene::nextSceneEffect()
{
	timerStart = true;
	buttons[cursor]->Start();
	background->MoveOn({ 0.f, -WINSIZE_Y }, 3.0f, Move_SoftStart | Move_Stop | POS_Update);
	fence->MoveOn({ 0.f,-WINSIZE_Y }, 3.0f, Move_SoftStart | Move_Stop | POS_Update);
	zero_zer->MoveOn({ 0.f,-WINSIZE_Y }, 3.0f, Move_SoftStart | Move_Stop | POS_Update);
	zero_o->MoveOn({ 0.f,-WINSIZE_Y }, 3.0f, Move_SoftStart | Move_Stop | POS_Update);
	katana->MoveOn({ 0.f,-WINSIZE_Y }, 3.0f, Move_SoftStart | Move_Stop | POS_Update);

	grass1->MoveOn({ 0.f,-WINSIZE_Y }, 1.7f, Move_SoftStart | Move_Stop | POS_Update);
	grass2->MoveOn({ 0.f,-WINSIZE_Y }, 1.7f, Move_SoftStart | Move_Stop | POS_Update);
	wall->On();
	wall->MoveOn({ 0.f,-WINSIZE_Y }, 1.7f, Move_SoftStart | Move_Stop);
}


