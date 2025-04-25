#include "HiddenScene.h"
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

#include "Player.h"
#include "DefaultObject.h"
#include "Factory.h"
#include "ArrowUI.h"
#include "GPImageManager.h"
#include "HiddenBoss.h"

HiddenScene::HiddenScene()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr), snapShotManager(nullptr), ScrollManager(nullptr),
	LineManager(nullptr), screenEffectManager(nullptr), fxManager(nullptr), gpImageManager(nullptr)
{
}

HRESULT HiddenScene::Init()
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

	if (FAILED(LineManager->LoadFile(L"Data/Hidden/HiddenLine.dat")))
	{
		MessageBox(g_hWnd, TEXT("HiddenScene LineManager LoadFile Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("HiddenScene InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("HiddenScene InitObject Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}
	if (FAILED(InitEffects()))
	{
		MessageBox(g_hWnd, TEXT("HiddenScene InitEffect Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	SoundManager::GetInstance()->PlayBGM("BossBossBoss");

	return S_OK;
}

HRESULT HiddenScene::InitImage()
{
	return S_OK;
}

HRESULT HiddenScene::InitObject()
{
	Background* background = new Background();
	background->Init("black", 0.f);
	ObjectManager->AddGameObject(EObjectType::GameObject, background);
	
	LoadBackground();
	HiddenBoss* boss = new HiddenBoss();
	boss->SetPos({ WINSIZE_X * 0.5f, WINSIZE_Y * 0.5f });
	boss->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, boss);


	




	LoadObject();

	
	



	return S_OK;
}

HRESULT HiddenScene::InitEffects()
{
	return S_OK;
}

void HiddenScene::TestCode()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F1))
		SceneManager::GetInstance()->ChangeScene("Test", "로딩_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F2))
		SceneManager::GetInstance()->ChangeScene("MapTool", "로딩_1");
}

void HiddenScene::LoadBackground()
{
	Image* image = ImageManager::GetInstance()->FindImage("spr_psychboss_background_0");

	for (int i = 0; i < 3; ++i)
	{
		Background* BackgroundObj = new Background();
		BackgroundObj->Init("spr_psychboss_background_0", 1, ScrollManager::GetInstance()->GetScale());

		const float CenterY = WINSIZE_Y * 0.5f;		
		BackgroundObj->SetPos({ 0.f,CenterY - (image->GetHeight() * ScrollManager::GetInstance()->GetScale() * i)});
		BackgroundObj->GetImage()->SetTransparent(false);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, BackgroundObj);
		MoveBackGrounds.push_back(BackgroundObj);
	}	

	{
		Background* BackgroundObj = new Background();
		BackgroundObj->Init("spr_psychboss_bg_1_0", 1, ScrollManager::GetInstance()->GetScale(),ERenderGroup::NonAlphaBlend);
		BackgroundObj->SetPos({ 0.f,WINSIZE_Y * 0.25f });
		BackgroundObj->GetImage()->SetTransparent(true);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, BackgroundObj);
	}

	{
		Background* BackgroundObj = new Background();
		BackgroundObj->Init("spr_psychboss_floor_0", 1, ScrollManager::GetInstance()->GetScale() + 0.5f, ERenderGroup::NonAlphaBlend);
		BackgroundObj->SetPos({ 0.f,WINSIZE_Y * 0.8f});
		BackgroundObj->GetImage()->SetTransparent(true);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, BackgroundObj);
	}

	{
		Background* BackgroundObj = new Background();
		BackgroundObj->Init("spr_psychboss_fg_0", 1, ScrollManager::GetInstance()->GetScale(), ERenderGroup::AlphaBlend);
		BackgroundObj->SetPos({ 0.f,WINSIZE_Y * 0.7f });
		BackgroundObj->GetImage()->SetTransparent(true);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, BackgroundObj);
	}
}

void HiddenScene::LoadObject()
{
	UIGame* ui = new UIGame();
	ui->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, ui);

	ArrowUI* ArrowUIObj = new ArrowUI();
	ArrowUIObj->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, ArrowUIObj);


	HANDLE hFile = CreateFile(
		L"Data/Hidden/HiddenObject.dat", GENERIC_READ, 0, NULL,
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
		ReadFile(hFile, &ObjData.Scale, sizeof(float), &dwByte, NULL);

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
		Obj->SetScale(ObjData.Scale);
		ObjectManager->AddGameObject(EObjectType::GameObject, Obj);

		if (ClassName == "StartPoint")
		{
			static_cast<SimpleObject*>(Obj)->SetUI(ui);
			static_cast<SimpleObject*>(Obj)->SetArrowUI(ArrowUIObj);
		}
	}

	CloseHandle(hFile);
}
void HiddenScene::Update()
{
	ObjectManager->Update();
	CollisionManager->Update();
	fxManager->Update();
	snapShotManager->Update(snapShotManager->IsReplaying());

	ScrollManager->Update();

	for (int i = 0; i < MoveBackGrounds.size(); ++i)
	{
		float Y = MoveBackGrounds[i]->GetPos().y;
		Y += 50.f * TimerManager::GetInstance()->GetDeltaTime();
		if (Y >= WINSIZE_Y)
		{
			Image* image = ImageManager::GetInstance()->FindImage("spr_psychboss_background_0");

			if (i == 0)
				MoveBackGrounds[i]->SetPos({ MoveBackGrounds[2]->GetPos().x,MoveBackGrounds[2]->GetPos().y - image->GetHeight() * ScrollManager::GetInstance()->GetScale() });
			else if (i == 1)
				MoveBackGrounds[i]->SetPos({ MoveBackGrounds[0]->GetPos().x,MoveBackGrounds[0]->GetPos().y - image->GetHeight() * ScrollManager::GetInstance()->GetScale() });
			else
				MoveBackGrounds[i]->SetPos({ MoveBackGrounds[1]->GetPos().x,MoveBackGrounds[1]->GetPos().y - image->GetHeight() * ScrollManager::GetInstance()->GetScale() });
		}
		else
			MoveBackGrounds[i]->SetPos({ 0.f,Y });		
	}

	TestCode();
}

void HiddenScene::Render(HDC hdc)
{
	RenderManager->Render(hdc);
	CollisionManager->Render(hdc);
	fxManager->Render(hdc);
	screenEffectManager->RenderDistortion(hdc);
	LineManager->Render(hdc);
}

void HiddenScene::Release()
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
	if (gpImageManager != nullptr)
		gpImageManager->Release();

	ObjectManager = nullptr;
	CollisionManager = nullptr;
	RenderManager = nullptr;
	ScrollManager = nullptr;
	LineManager = nullptr;
	screenEffectManager = nullptr;
	snapShotManager = nullptr;
	fxManager = nullptr;
	gpImageManager = nullptr;
}
