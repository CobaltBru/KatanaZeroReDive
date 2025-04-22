#include "TalkScene.h"
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

TalkScene::TalkScene() :
	ObjectManager(nullptr), RenderManager(nullptr)
{
}

HRESULT TalkScene::Init()
{
	SetClientRect(g_hWndParent, WINSIZE_X, WINSIZE_Y);
	ObjectManager = ObjectManager::GetInstance();
	ObjectManager->Init();
	RenderManager = RenderManager::GetInstance();
	RenderManager->Init();

	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("TalkScene InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("TalkScene InitObject Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

void TalkScene::Release()
{
	if (ObjectManager != nullptr)
		ObjectManager->Release();
	if (RenderManager != nullptr)
		RenderManager->Release();

	ScrollManager::GetInstance()->Release();
	ObjectManager = nullptr;
	RenderManager = nullptr;
}

void TalkScene::Update()
{
	ObjectManager->Update();
}

void TalkScene::Render(HDC hdc)
{
	RenderManager->Render(hdc);
}

HRESULT TalkScene::InitImage()
{
	ImageManager::GetInstance()->AddImage("BlackBackground", L"Image/Background/black.bmp", 1600, 900, 1, 1);
	return S_OK;
}

HRESULT TalkScene::InitObject()
{
	Background* bg = new Background();
	bg->Init("BlackBackground");
	ObjectManager->AddGameObject(EObjectType::GameObject, bg);

	Image* tmp = ImageManager::GetInstance()->AddImage("title_bg", L"Image/UI/Home/spr_title_background.bmp", 1600, 1800, 1, 1, true, RGB(255, 0, 255));
	background = new Animation();
	background->Init(tmp, 1);
	background->setPos({ 0.f,-400.f }, false, false);
	background->setAniTask({ {0,10.f} });
	background->MoveOn({ 0.f,-500.f }, 1.0f, Move_SoftEnd | Move_Stop);
	background->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, background);





	return S_OK;
}
