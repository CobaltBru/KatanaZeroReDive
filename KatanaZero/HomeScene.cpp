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

	/*if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene InitObject Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}*/
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
	background->MoveOn({ 0.f,-300.f }, 3.0f, Move_SoftEnd | Move_Stop);
	background->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, background);

	tmp = ImageManager::GetInstance()->AddImage("title_grass1", L"Image/UI/Home/spr_grass1.bmp", 1600, 300, 1, 1, true, RGB(255, 0, 255));
	grass1 = new Animation();
	grass1->Init(tmp, 1);
	grass1->setPos({ 0.f,WINSIZE_Y}, false, false);
	grass1->setAniTask({ {0,10.f} });
	grass1->MoveOn({ 0.f,-300.f }, 3.0f, Move_SoftEnd | Move_Stop);
	grass1->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, grass1);

	/*Image* tmp1 = ImageManager::GetInstance()->AddImage("title_grass1", L"Image/UI/Home/spr_grass1.bmp", 1600, 300, 1, 1, true, RGB(255, 0, 255));
	fence = new Animation();
	fence->Init(tmp1, 1);
	fence->setPos({ 0.f,WINSIZE_Y }, false, false);
	fence->setAniTask({ {0,10.f} });
	fence->MoveOn({ 0.f,-300.f }, 3.0f, Move_SoftEnd | Move_Stop);
	fence->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, fence);*/
	return S_OK;
}
