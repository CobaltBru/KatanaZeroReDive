#include "BossScene.h"
#include "CommonFunction.h"

#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "ScrollManager.h"

#include "TaeKyungObject.h"
#include "Background.h"
#include "TestObject.h"
#include "SoundManager.h"

#include "ChatManager.h"
#include "UIGame.h"
#include "GoPopUp.h"
#include "SnapShotManager.h"
#include "ScreenEffectManager.h"
#include "HeadHunter.h"

#include "LineManager.h"

#include "Effect.h"
#include "EffectManager.h"

#include "GPImageManager.h"

#include "Player.h"
#include "DefaultObject.h"
#include "Factory.h"
#include "Tile.h"
#include "ArrowUI.h"

BossScene::BossScene()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr), snapShotManager(nullptr), ScrollManager(nullptr), gpImageManager(nullptr), LineManager(nullptr), screenEffectManager(nullptr), fxManager(nullptr), elapsedTime(0.0f)
{
}

HRESULT BossScene::Init()
{
	SetClientRect(g_hWndParent, WINSIZE_X, WINSIZE_Y);

	ObjectManager = ObjectManager::GetInstance();
	ObjectManager->Init();
	RenderManager = RenderManager::GetInstance();
	RenderManager->Init();
	CollisionManager = CollisionManager::GetInstance();
	CollisionManager->Init();

	snapShotManager = SnapShotManager::GetInstance();
	snapShotManager->Init();

	screenEffectManager = ScreenEffectManager::GetInstance();
	screenEffectManager->Init();

	ScrollManager = ScrollManager::GetInstance();
	ScrollManager->Init();
	ScrollManager->ZeroScroll();

	LineManager = LineManager::GetInstance();
	LineManager->Init();

	fxManager = EffectManager::GetInstance();
	fxManager->Init();
	gpImageManager = GPImageManager::GetInstance();
	gpImageManager->Init();
	Player* player = new Player();
	player->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, player);


	if (FAILED(LineManager->LoadFile(L"Data/Stage1/headhunter_test.dat")))
	{
		MessageBox(g_hWnd, TEXT("BossScene LineManager LoadFile Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("BossScene InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("BossScene InitObject Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}
	if (FAILED(InitEffects()))
	{
		MessageBox(g_hWnd, TEXT("BossScene InitEffect Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	SoundManager::GetInstance()->PlayBGM("Katana ZeroTest");

	return S_OK;
}

HRESULT BossScene::InitImage()
{
	// 해당 씬에 필요한 모든 이미지 추가
	ImageManager::GetInstance()->AddImage("black", L"Image/Background/blackBg.bmp", 1920, 1080, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("TestPlayer", L"Image/headhunter_jump.bmp", 27, 44, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("HeadHunter", L"Image/HeadHunter/headhunter_idle_init.bmp", 25, 50, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BossRoom", L"Image/Background/spr_outside_vault_bg_0.bmp", 640, 360, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("Player", L"Image/TestPlayer.bmp", 25, 35, 1, 1, true, RGB(255, 0, 255));

	InitBackgroundImage();

	return S_OK;
}

HRESULT BossScene::InitObject()
{
	Background* background = new Background();
	background->Init("black", 0.f);
	ObjectManager->AddGameObject(EObjectType::GameObject, background);

	LoadBackground();
	LoadObject();
	LoadFloor();

	return S_OK;
}

HRESULT BossScene::InitEffects()
{
	return S_OK;
}

void BossScene::TestCode()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F1))
		SceneManager::GetInstance()->ChangeScene("Test", "로딩_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F2))
		SceneManager::GetInstance()->ChangeScene("MapTool", "로딩_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_ESCAPE))
		SceneManager::GetInstance()->ChangeScene("Home", "로딩_1");
}

void BossScene::InitBackgroundImage()
{
	vector<string> backgrounds = GetFileNames("Image/Background/*.bmp");

	if (backgrounds.empty())
		return;

	for (int i = 0; i < backgrounds.size(); ++i)
	{
		int dotPos = backgrounds[i].find_last_of('.');
		string nameOnly = dotPos != string::npos ? backgrounds[i].substr(0, dotPos) : backgrounds[i];

		wstring wsPath = L"Image/Background/";
		wsPath += wstring(backgrounds[i].begin(), backgrounds[i].end());

		ImageManager::GetInstance()->AddImage(nameOnly, wsPath.c_str(), true, RGB(255, 0, 255));
	}
}

void BossScene::LoadBackground()
{
	HANDLE hFile = CreateFile(
		L"Data/Stage1/headhunter_bg.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"LoadBackGround Failed.", TEXT("경고"), MB_OK);
		return;
	}

	DWORD dwByte = 0;

	while (true)
	{
		int Size;
		float ScrollPer;
		FPOINT Pos;
		ReadFile(hFile, &ScrollPer, sizeof(float), &dwByte, NULL);
		ReadFile(hFile, &Size, sizeof(int), &dwByte, NULL);

		char* buffer = new char[Size + 1];
		ReadFile(hFile, buffer, Size, &dwByte, NULL);
		buffer[Size] = '\0';
		ReadFile(hFile, &Pos, sizeof(FPOINT), &dwByte, NULL);

		string BackgroundName = buffer;

		delete[] buffer;

		if (dwByte == 0)
			break;

		Background* BackgroundObj = new Background();
		BackgroundObj->Init(BackgroundName, ScrollPer, ScrollManager::GetInstance()->GetScale());
		BackgroundObj->SetPos(Pos);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, BackgroundObj);
	}

	CloseHandle(hFile);
}

void BossScene::LoadObject()
{
	UIGame* ui = new UIGame();
	ui->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, ui);

	ArrowUI* ArrowUIObj = new ArrowUI();
	ArrowUIObj->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, ArrowUIObj);


	HANDLE hFile = CreateFile(
		L"Data/Stage1/headhunter_object.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"LoadObject Failed.", TEXT("경고"), MB_OK);
		return;
	}

	DWORD dwByte = 0;

	while (true)
	{
		FObjectData ObjData;
		ZeroMemory(&ObjData, sizeof(FObjectData));

		ReadFile(hFile, &ObjData.ClsasNameSize, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &ObjData.ImageNameSize, sizeof(int), &dwByte, NULL);
		ObjData.ClassName = new char[ObjData.ClsasNameSize + 1];
		ObjData.ImageName = new char[ObjData.ImageNameSize + 1];

		ReadFile(hFile, ObjData.ClassName, ObjData.ClsasNameSize, &dwByte, NULL);
		ReadFile(hFile, ObjData.ImageName, ObjData.ImageNameSize, &dwByte, NULL);
		ReadFile(hFile, &ObjData.Pos, sizeof(FPOINT), &dwByte, NULL);
		ReadFile(hFile, &ObjData.Offset, sizeof(FPOINT), &dwByte, NULL);
		ReadFile(hFile, &ObjData.Size, sizeof(FPOINT), &dwByte, NULL);
		ReadFile(hFile, &ObjData.bLeft, sizeof(bool), &dwByte, NULL);

		ObjData.ClassName[ObjData.ClsasNameSize] = '\0';
		ObjData.ImageName[ObjData.ImageNameSize] = '\0';

		string ClassName = ObjData.ClassName;
		string ImageName = ObjData.ImageName;

		delete[] ObjData.ClassName;
		delete[] ObjData.ImageName;

		if (dwByte == 0)
			break;

		GameObject* Obj = CreateObject(ClassName);
		Obj->Init(ImageName, ObjData.Pos, ObjData.Offset, ObjData.Size, ObjData.bLeft, ERenderGroup::NonAlphaBlend);
		ObjectManager->AddGameObject(EObjectType::GameObject, Obj);

		if (ClassName == "StartPoint")
		{
			static_cast<SimpleObject*>(Obj)->SetUI(ui);
			static_cast<SimpleObject*>(Obj)->SetArrowUI(ArrowUIObj);
		}
	}
	
	CloseHandle(hFile);
}

void BossScene::LoadFloor()
{
	HANDLE hFile = CreateFile(
		L"Data/Stage1/Stage1Floor.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"LoadObject Failed.", TEXT("경고"), MB_OK);
		return;
	}

	DWORD dwByte = 0;

	vector<FloorZone> FloorZones;
	while (true)
	{
		FloorZone fz;
		ZeroMemory(&fz, sizeof(FloorZone));

		ReadFile(hFile, &fz, sizeof(FloorZone), &dwByte, NULL);

		if (dwByte == 0)
			break;

		FloorZones.push_back(fz);
	}

	CloseHandle(hFile);
}

void BossScene::Update()
{
	ObjectManager->Update();
	CollisionManager->Update();
	fxManager->Update();
	snapShotManager->Update(snapShotManager->IsReplaying());

	ScrollManager->Update();

	TestCode();
}

void BossScene::Render(HDC hdc)
{
	RenderManager->Render(hdc);
	CollisionManager->Render(hdc);
	fxManager->Render(hdc);
	screenEffectManager->RenderDistortion(hdc);
	LineManager->Render(hdc);
}

void BossScene::Release()
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
	if (screenEffectManager != nullptr)
		screenEffectManager->Release();
	if (snapShotManager != nullptr)
		snapShotManager->Release();
	if (fxManager != nullptr)
		fxManager->Release();
	ObjectManager = nullptr;
	CollisionManager = nullptr;
	RenderManager = nullptr;
	ScrollManager = nullptr;
	LineManager = nullptr;
	screenEffectManager = nullptr;
	snapShotManager = nullptr;
	fxManager = nullptr;
}
