#include "HomeScene.h"
#include "CommonFunction.h"

#include "Animation.h"
#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "GPImage.h"

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

	return S_OK;
}

HRESULT HomeScene::InitObject()
{
	GPImage* tmp = new GPImage();
	tmp->AddImage(L"Image/UI/Home/spr_title_background.png");
	background = new Animation();
	background->Init(tmp, 1);
	background->setPos({ 0.f,-600.f }, false, false);
	background->setAniTask({ {0,10.f} });
	background->MoveOn({ 0.f,-300.f }, 1.0f, Move_SoftEnd | Move_Stop);
	background->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, background);
	return S_OK;
}
