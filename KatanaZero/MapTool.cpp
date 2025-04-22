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

#include "Factory.h"
#include "ImGuiManager.h"

MapTool::MapTool()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr), ScrollManager(nullptr), LineManager(nullptr)
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

	ImGuiManager::GetInstance()->Init(LineManager, ObjectManager, ScrollManager);

	//if (FAILED(LineManager->LoadFile(L"Data/Stage1/Stage1Line.dat")))
	//{
	//	MessageBox(g_hWnd, TEXT("MapTool LineManager LoadFile Failed."), TEXT("실패"), MB_OK);
	//	return E_FAIL;
	//}

	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("MapTool InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("MapTool InitObject Failed."), TEXT("실패"), MB_OK);
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
		SceneManager::GetInstance()->ChangeScene("Test", "로딩_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F3))
		SceneManager::GetInstance()->ChangeScene("Stage1", "로딩_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_ESCAPE))
		SceneManager::GetInstance()->ChangeScene("Home", "로딩_1");
}

void MapTool::Render(HDC hdc)
{
	RenderManager->Render(hdc);
	CollisionManager->Render(hdc);

	LineManager->Render(hdc);

	ImGuiManager::GetInstance()->APIRender(hdc);

	ImGuiManager::GetInstance()->Render();
}

HRESULT MapTool::InitImage()
{
	ImageManager::GetInstance()->AddImage("black", L"Image/Background/blackBg.bmp", 1920, 1080, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("rocket", L"Image/rocket.bmp", 52, 64, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("TestPlayer", L"Image/headhunter_jump.bmp", 27, 44, 1, 1, true, RGB(255, 0, 255));

	return S_OK;
}

HRESULT MapTool::InitObject()
{
	Background* background = new Background();
	background->Init("black",0.f);
	ObjectManager->AddGameObject(EObjectType::GameObject, background);

	return S_OK;
}


void MapTool::Release()
{
	ImGuiManager::GetInstance()->Reset();

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

	ObjectManager = nullptr;
	CollisionManager = nullptr;
	RenderManager = nullptr;
	ScrollManager = nullptr;
	LineManager = nullptr;
}