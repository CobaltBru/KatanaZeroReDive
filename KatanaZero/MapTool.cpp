#include "MapTool.h"
#include "CommonFunction.h"

#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "ScrollManager.h"

#include "Background.h"
#include "SoundManager.h"
#include "LineManager.h"
#include "SnapShotManager.h"

#include "SimpleObject.h"
#include "SimpleTestObject.h"
#include "EffectManager.h"
#include "SnapShotManager.h"

// 해영 Enemy 테스트
#include "Enemy.h"
#include "Enemies.h"

#include "ImGuiManager.h"

MapTool::MapTool()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr), ScrollManager(nullptr), LineManager(nullptr), fxManager(nullptr), snapShotManager(nullptr)
{
}

HRESULT MapTool::Init()
{
	SoundManager::GetInstance()->StopAll();

	SetClientRect(g_hWndParent, WINSIZE_X + TILEMAPTOOL_X, WINSIZE_Y);

	ObjectManager = ObjectManager::GetInstance();
	ObjectManager->Init();
	RenderManager = RenderManager::GetInstance();
	RenderManager->Init();
	CollisionManager = CollisionManager::GetInstance();
	CollisionManager->Init();

	ScrollManager = ScrollManager::GetInstance();
	ScrollManager->Init();
	ScrollManager->ZeroScroll();

	LineManager = LineManager::GetInstance();
	LineManager->Init();

	fxManager = EffectManager::GetInstance();

	snapShotManager = SnapShotManager::GetInstance();
	snapShotManager->Init();

	if (FAILED(LineManager->LoadFile(L"Data/test2.dat")))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene LineManager LoadFile Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

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



void MapTool::Update()
{
	ObjectManager->Update();
	CollisionManager->Update();
	ScrollManager->Update();
	LineManager->Update();

	ImGuiManager::GetInstance()->Update();
	

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F1))
		SceneManager::GetInstance()->ChangeScene("Stage1", "로딩_1");
}

void MapTool::Render(HDC hdc)
{
	RenderManager->Render(hdc);
	CollisionManager->Render(hdc);

	LineManager->Render(hdc);

	ImGuiManager::GetInstance()->Render();
}

HRESULT MapTool::InitImage()
{
	ImageManager::GetInstance()->AddImage("TestBg", L"Image/TestBg.bmp", 1639, 824, 1, 1, true, RGB(255, 0, 255));

	return S_OK;
}

HRESULT MapTool::InitObject()
{
	Background* background = new Background();
	background->Init("TestBg");
	ObjectManager->AddGameObject(EObjectType::GameObject, background);

	SimpleObject* taekyung = new SimpleObject();
	taekyung->Init({ 500.f,100.f });
	ObjectManager->AddGameObject(EObjectType::GameObject, taekyung);

	SimpleTestObject* enemy = new SimpleTestObject();
	enemy->Init({ 500.f, 0.f });
	ObjectManager->AddGameObject(EObjectType::GameObject, enemy);
	

	// 해영 테스트
	{
		Grunt* grunt = new Grunt();
		grunt->Init({ 800.f, 0.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, grunt);

		snapShotManager->AddGameObject(EObjectClassType::Enemy, grunt);
		snapShotManager->AddGameObject(EObjectClassType::Player, taekyung);
	}
	

	return S_OK;
}


void MapTool::Release()
{
	if (ObjectManager != nullptr)
		ObjectManager->Release();
	if (CollisionManager != nullptr)
		CollisionManager->Release();
	if (RenderManager != nullptr)
		RenderManager->Release();
	if (ScrollManager != nullptr)
		ScrollManager->Release();
	if (LineManager != nullptr)
		LineManager->Release();
	if (fxManager != nullptr)
		fxManager->Release();	
	if (snapShotManager != nullptr)
		snapShotManager->Release();

	ObjectManager = nullptr;
	CollisionManager = nullptr;
	RenderManager = nullptr;
	ScrollManager = nullptr;
	LineManager = nullptr;
	fxManager = nullptr;
	snapShotManager = nullptr;
}