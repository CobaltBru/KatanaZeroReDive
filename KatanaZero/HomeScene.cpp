#include "HomeScene.h"
#include "CommonFunction.h"

#include "Background.h"
#include "Animation.h"
#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "GPImage.h"
#include "Image.h"

HomeScene::HomeScene():
	ObjectManager(nullptr), RenderManager(nullptr)
{
}

HRESULT HomeScene::Init()
{
	SetClientRect(g_hWndParent, WINSIZE_X, WINSIZE_Y);

	ObjectManager = ObjectManager::GetInstance();
	ObjectManager->Init();
	RenderManager = RenderManager::GetInstance();
	RenderManager->Init();

	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene InitObject Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

void HomeScene::Release()
{
	if (ObjectManager != nullptr)
		ObjectManager->Release();
	if (RenderManager != nullptr)
		RenderManager->Release();

	ObjectManager = nullptr;
	RenderManager = nullptr;

}

void HomeScene::Update()
{
	ObjectManager->Update();

}

void HomeScene::Render(HDC hdc)
{
	RenderManager->Render(hdc);
}

HRESULT HomeScene::InitImage()
{

	ImageManager::GetInstance()->AddImage("BlackBackground",L"Image/Background/black.bmp");
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
	background->setPos({ 0.f,-600.f }, false, false);
	background->setAniTask({ {0,10.f} });
	background->MoveOn({ 0.f,-300.f }, 2.5f, Move_SoftEnd | Move_Stop);
	background->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, background);

	tmp = ImageManager::GetInstance()->AddImage("title_fence", L"Image/UI/Home/spr_title_fence.bmp", 1600, 1800, 1, 1, true, RGB(255, 0, 255));
	fence = new Animation();
	fence->Init(tmp, 1);
	fence->setPos({ 0.f,-600.f }, false, false);
	fence->setAniTask({ {0,10.f} });
	fence->MoveOn({ 0.f,-300.f }, 2.7f, Move_SoftEnd | Move_Stop);
	fence->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, fence);

	tmp = ImageManager::GetInstance()->AddImage("title_grass1", L"Image/UI/Home/spr_grass1.bmp", 1600, 300, 1, 1, true, RGB(255, 0, 255));
	grass1 = new Animation();
	grass1->Init(tmp, 1);
	grass1->setPos({ 0.f,WINSIZE_Y}, false, false);
	grass1->setAniTask({ {0,10.f} });
	grass1->MoveOn({ 0.f,-300.f }, 2.0f, Move_SoftEnd | Move_Stop);
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
	grass2->MoveOn({ 0.f,-637.f }, 1.8f, Move_SoftEnd | Move_Stop);
	grass2->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, grass2);

	/*GPImage* gpi = new GPImage();
	gpi->AddImage(L"Image/UI/Home/spr_zer.png", 2, 1);
	zero_zer = new Animation();
	zero_zer->Init(gpi, 2);
	zero_zer->setPos({ WINSIZE_X/2 - 100.f,WINSIZE_Y }, false, true);
	zero_zer->setAniTask({
		{0,0.5f},{1,0.5f}});
	zero_zer->MoveOn({ 0.f,-WINSIZE_Y/2 }, 2.5f, Move_SoftEnd | Move_Stop);
	zero_zer->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, zero_zer);*/

	return S_OK;
}
