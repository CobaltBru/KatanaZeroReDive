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

#include "ScenePsych.h"
#include "ScenePlayer.h"

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
	pos = { 0,0 };

	
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

	/*if (KeyManager::GetInstance()->IsStayKeyDown('W'))
	{
		pos.y -= 1;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		pos.y += 1;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		pos.x -= 1;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		pos.x += 1;
	}*/
	//firePlace->setPos(pos, false, false);
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
	Image* tmp;
	tmp = ImageManager::GetInstance()->AddImage("talk_bg", L"Image/TalkScene/bg_psychiatrist.bmp", 1597, 900, 1, 1, true, RGB(255, 0, 255));
	background = new Animation();
	background->Init(tmp, 1);
	background->setPos({ 0.f,0.f }, false, false);
	background->setAniTask({ {0,10.f} });
	background->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, background);

	tmp = ImageManager::GetInstance()->AddImage("talk_fireplace", L"Image/TalkScene/spr_psych_fireplace.bmp", 595, 72, 7, 1, true, RGB(255, 0, 255));
	firePlace = new Animation();
	firePlace->Init(tmp, 7);
	firePlace->setPos({ 740,578 }, false, false);
	firePlace->setAniTask({
		{0,0.1f}, { 1,0.1f }, { 2,0.1f },
		{3,0.1f}, { 4,0.1f }, { 5,0.1f } ,
		{6,0.1f} });
	firePlace->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, firePlace);

	psych = new ScenePsych();
	psychPos = { 865.f,637.f };
	psych->SetPos(psychPos);
	psych->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, psych);

	player = new ScenePlayer();
	playerPos = { 695.f,640.f };
	player->SetPos(playerPos);
	player->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, player);
	
	

	return S_OK;
}
