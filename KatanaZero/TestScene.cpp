#include "TestScene.h"
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
#include "SimpleTestObject.h"
#include "SimpleObject.h"
#include "Bullet.h"
#include "Collider.h"

TestScene::TestScene()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr), snapShotManager(nullptr), ScrollManager(nullptr), LineManager(nullptr), screenEffectManager(nullptr), fxManager(nullptr), elapsedTime(0.0f)
{
	
}

HRESULT TestScene::Init()
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

	if (FAILED(LineManager->LoadFile(L"Data/Stage1/playerLine.dat")))
	{
		MessageBox(g_hWnd, TEXT("TestScene LineManager LoadFile Failed."), TEXT("����"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("TestScene InitImage Failed."), TEXT("����"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("TestScene InitObject Failed."), TEXT("����"), MB_OK);
		return E_FAIL;
	}
	if (FAILED(InitEffects()))
	{
		MessageBox(g_hWnd, TEXT("TestScene InitEffect Failed."), TEXT("����"), MB_OK);
		return E_FAIL;
	}
	
	SoundManager::GetInstance()->PlayBGM("Katana ZeroTest");

	return S_OK;
}

HRESULT TestScene::InitImage()
{
	// �ش� ���� �ʿ��� ��� �̹��� �߰�
	ImageManager::GetInstance()->AddImage("black", L"Image/Background/blackBg.bmp", 1920, 1080, 1, 1, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("rocket", L"Image/rocket.bmp", 52, 64, 1, 1, true, RGB(255, 0, 255));

	return S_OK;
}

HRESULT TestScene::InitObject()
{
	// �� �ʱ⿡ �ʿ��� ������Ʈ ����
	// ����
	Player* player = new Player();
	player->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, player);

	BulletTest* newBullet = new BulletTest();
	newBullet->Init({300.f, 700.f}, 0.f);
	ObjectManager->AddGameObject(EObjectType::GameObject, newBullet);

	//SimpleObject* simpleObject = new SimpleObject();
	//simpleObject->Init("rocket", { 500.f, 300.f }, { 0.f, 0.f }, { 30.f, 30.f }, false);
	//ObjectManager->AddGameObject(EObjectType::GameObject, simpleObject);

	SimpleTestObject* simpleTestObject = new SimpleTestObject();
	simpleTestObject->Init("rocket", { 500.f, 300.f }, { 0.f, 0.f }, { 30.f, 30.f }, false);
	ObjectManager->AddGameObject(EObjectType::GameObject, simpleTestObject);

	/**/
	// �׽�Ʈ �ڵ� �°�
	{
		Background* background = new Background();
		background->Init("black",0.f);
		ObjectManager->AddGameObject(EObjectType::GameObject, background);
		
		/*TaeKyungObject* taekyung = new TaeKyungObject();
		taekyung->Init({ 500.f,550.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, taekyung);*/

		TestObject* testObject = new TestObject();
		testObject->Init("rocket", { 1000.f,300.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, testObject);

		


		//�ؿ� �׽�Ʈ
		{
			snapShotManager->AddGameObject(EObjectClassType::Player, player);
			snapShotManager->AddGameObject(EObjectClassType::Enemy, testObject);
		}

		{
			HeadHunter* headhunter = new HeadHunter();
			headhunter->Init({ 300,360 });
			ObjectManager->AddGameObject(EObjectType::GameObject, headhunter);
		}

	}
	// �׽�Ʈ �ڵ� ����
	{
		/*chatManager = new ChatManager();
		chatManager->pushPos({ 600,100 });
		chatManager->pushPos({ 400,100 });
		chatManager->LoadChat("ChatDatas/test2.json");

		chatManager->startChat("Q3YMPZfZRO");
		ObjectManager->AddGameObject(EObjectType::GameObject, chatManager);
		*/
		UIGame* ui = new UIGame();
		ui->Init();
		ObjectManager->AddGameObject(EObjectType::GameObject, ui);

		GoPopUp* goPopUp = new GoPopUp();
		
		goPopUp->Init();
		goPopUp->On(player->GetPPos(), &testDestPos);
		ObjectManager->AddGameObject(EObjectType::GameObject, goPopUp);
	}
	return S_OK;
}

HRESULT TestScene::InitEffects()
{
	fxManager->Addfx("normalslash", L"Image/fx/NormalSlash.png", 5, 1);
	fxManager->Addfx("rainbowslash", L"Image/fx/RainbowSlash.png", 7, 1);
	fxManager->Addfx("bulletreflect", L"Image/fx/BulletReflect.png", 5, 1);
	fxManager->Addfx("hitslash", L"Image/fx/HitSlash.png", 4, 1);
	fxManager->Addfx("enemyslash", L"Image/fx/EnemySlash.png", 4, 1);
	fxManager->Addfx("jumpcloud", L"Image/fx/JumpCloud.png", 4, 1);
	fxManager->RegisterEffect();
	return S_OK;
}

void TestScene::TestCode()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F2))
		SceneManager::GetInstance()->ChangeScene("MapTool", "�ε�_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F3))
		SceneManager::GetInstance()->ChangeScene("Stage1", "�ε�_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_ESCAPE))
		SceneManager::GetInstance()->ChangeScene("Home", "�ε�_1");

	if (KeyManager::GetInstance()->IsOnceKeyDown('C'))
	{
		// ���� : ����ũ ����, ���ӽð�
		ScrollManager->CameraShake(5.f, 1.f);
	}

	//���ο�
	if (KeyManager::GetInstance()->IsStayKeyDown('I'))
	{
		// GetDeltaTime ���ڿ� false ������ �������� DeltaTime�������� true������ ���ο� ��� ���� DeltaTime�����ɴϴ�  ����Ʈ true��
		// TimerManager::GetInstance()->GetDeltaTime();
		 
		
		//���ο� �ֱ�                  //���ο��� 0 ~ 1 / �ش� ������� ���µ� ���ʵ��� �����Ұų�
		TimerManager::GetInstance()->SetSlow(0.1f,0.2f);
	}
	else  // ���ο� Ǯ��
		TimerManager::GetInstance()->SetSlow(1.f,0.2f);


	// ���� Ʈ���̽�
	FHitResult HitResult;
	if (CollisionManager->LineTraceByObject(HitResult, ECollisionGroup::Player, { 0.f,0.f }, { (float)g_ptMouse.x,(float)g_ptMouse.y },true, 0.f))
	{
		// ���� Ʈ���̽� ���� ����� �ݶ��̴�
		HitResult.HitCollision->SetHit(true);
	}
}

void TestScene::Update()
{
	ObjectManager->Update();
	CollisionManager->Update();
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
	testDestPos = { Scroll.x + 1600,Scroll.y + 500.f };
	//elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	//testDestPos.y +=1.f;
	fxManager->Update();
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
	{
		fxManager->Activefx("normalslash", { 100.0f, 200.0f }, { 250.0f, 350.0f }, 300.0f, false);
		fxManager->Activefx("normalslash", { 200.0f, 200.0f }, { 350.0f, 350.0f }, 300.0f, false);
		fxManager->Activefx("normalslash", { 300.0f, 200.0f }, { 450.0f, 350.0f }, 300.0f, false);
		fxManager->Activefx("normalslash", { 500.0f, 200.0f }, { 650.0f, 350.0f }, 300.0f, false);
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(82))
	{
		snapShotManager->StartReplay(); // ���÷���
	}
	snapShotManager->Update(snapShotManager->IsReplaying());


	ScrollManager->Update();

	TestCode();
}

void TestScene::Render(HDC hdc)
{
	RenderManager->Render(hdc);
	CollisionManager->Render(hdc);
	fxManager->Render(hdc);
	screenEffectManager->RenderDistortion(hdc);
	LineManager->Render(hdc);
}

void TestScene::Release()
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
