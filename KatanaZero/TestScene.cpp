#include "TestScene.h"
#include "CommonFunction.h"

#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "ScrollManager.h"
#include "GPImageManager.h"

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

#include "Enemy.h"
#include "Enemies.h"
#include "ParticleEffect.h"

TestScene::TestScene()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr), snapShotManager(nullptr), ScrollManager(nullptr), LineManager(nullptr), screenEffectManager(nullptr), fxManager(nullptr), gpImageManager(nullptr), elapsedTime(0.0f)
{
	
}

HRESULT TestScene::Init()
{
	SetClientRect(g_hWndParent, WINSIZE_X, WINSIZE_Y);
	
	srand(time(NULL));
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

	gpImageManager = GPImageManager::GetInstance();
	gpImageManager->Init();

	fxManager = EffectManager::GetInstance();
	fxManager->Init();

	slowStart = false;
	if (FAILED(LineManager->LoadFile(L"Data/Stage1/playerLine.dat")))
	{
		MessageBox(g_hWnd, TEXT("TestScene LineManager LoadFile Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("TestScene InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("TestScene InitObject Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}
	if (FAILED(InitEffects()))
	{
		MessageBox(g_hWnd, TEXT("TestScene InitEffect Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}
	
	SoundManager::GetInstance()->PlayBGM("Katana ZeroTest");

	return S_OK;
}

HRESULT TestScene::InitImage()
{
	// 해당 씬에 필요한 모든 이미지 추가
	ImageManager::GetInstance()->AddImage("black", L"Image/Background/blackBg.bmp", 1920, 1080, 1, 1, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("rocket", L"Image/rocket.bmp", 52, 64, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BGBlood_right1", L"Image/fx/BGBlood_right1.bmp", 96, 27, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BGBlood_right2", L"Image/fx/BGBlood_right2.bmp", 145, 29, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BGBlood_right3", L"Image/fx/BGBlood_right3.bmp", 294, 32, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BGBlood_righttop1", L"Image/fx/BGBlood_righttop1.bmp", 54, 70, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BGBlood_righttop2", L"Image/fx/BGBlood_righttop2.bmp", 92, 121, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BGBlood_righttop3", L"Image/fx/BGBlood_righttop3.bmp", 125, 142, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BGBlood_rightbottom1", L"Image/fx/BGBlood_rightbottom1.bmp", 54, 70, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BGBlood_rightbottom2", L"Image/fx/BGBlood_rightbottom2.bmp", 92, 121, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("BGBlood_rightbottom3", L"Image/fx/BGBlood_rightbottom3.bmp", 125, 142, 1, 1, true, RGB(255, 0, 255));

	return S_OK;
}

HRESULT TestScene::InitObject()
{
	// 씬 초기에 필요한 오브젝트 생성
	// 자은
	Player* player = new Player();
	player->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, player);

	/*BulletTest* newBullet = new BulletTest();
	newBullet->Init({300.f, 700.f}, 0.f);
	ObjectManager->AddGameObject(EObjectType::GameObject, newBullet);*/

	//SimpleObject* simpleObject = new SimpleObject();
	//simpleObject->Init("rocket", { 500.f, 300.f }, { 0.f, 0.f }, { 30.f, 30.f }, false);
	//ObjectManager->AddGameObject(EObjectType::GameObject, simpleObject);

	SimpleTestObject* simpleTestObject = new SimpleTestObject();
	simpleTestObject->Init("rocket", { 500.f, 300.f }, { 0.f, 0.f }, { 30.f, 30.f }, false);
	ObjectManager->AddGameObject(EObjectType::GameObject, simpleTestObject);

	/**/
	// 테스트 코드 태경
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



		//해영 테스트
		{
			Grunt* grunt = new Grunt();
			grunt->Init({ 800.f, 300.f });
			Pomp* pomp = new Pomp();
			pomp->Init({ 500.f, 300.f });
			Gangster* gangster = new Gangster();
			gangster->Init({ 100.f, 300.f });

			ObjectManager->AddGameObject(EObjectType::GameObject, grunt);
			ObjectManager->AddGameObject(EObjectType::GameObject, pomp);
			ObjectManager->AddGameObject(EObjectType::GameObject, gangster);

			
		}

		// 지수 테스트
		/*{
			HeadHunter* headhunter = new HeadHunter();
			headhunter->Init({ 300,360 });
			ObjectManager->AddGameObject(EObjectType::GameObject, headhunter);
		}*/

	}
	// 테스트 코드 지운 사람
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
	return S_OK;
}

void TestScene::TestCode()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F2))
		SceneManager::GetInstance()->ChangeScene("MapTool", "로딩_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F3))
		SceneManager::GetInstance()->ChangeScene("Stage1", "로딩_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_ESCAPE))
		SceneManager::GetInstance()->ChangeScene("Home", "로딩_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F4))
		SceneManager::GetInstance()->ChangeScene("HY", "로딩_1");
	

	if (KeyManager::GetInstance()->IsOnceKeyDown('C'))
	{
		// 인자 : 쉐이크 강도, 지속시간
		ScrollManager->CameraShake(5.f, 1.f);
	}

	//슬로우
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_SHIFT))
	{
		// GetDeltaTime 인자에 false 넣으면 오리지날 DeltaTime가져오고 true넣으면 슬로우 계수 붙은 DeltaTime가져옵니다  디폴트 true임
		// TimerManager::GetInstance()->GetDeltaTime();
		if (slowStart == false)
		{
			SoundManager::GetInstance()->PlaySounds("slowon", EChannelType::Effect);
			slowStart = true;
		}
		SoundManager::GetInstance()->PitchDown(EChannelType::BGM);
		//슬로우 주기                  //슬로우계수 0 ~ 1 / 해당 계수까지 가는데 몇초동안 보간할거냐
		TimerManager::GetInstance()->SetSlow(0.1f,0.2f);
	}
	else  // 슬로우 풀기
	{
		if (slowStart == true)
		{
			SoundManager::GetInstance()->PlaySounds("slowoff", EChannelType::Effect);
			slowStart = false;
		}
		SoundManager::GetInstance()->PitchOrigin(EChannelType::BGM);
		TimerManager::GetInstance()->SetSlow(1.f, 0.2f);
	}
		
	//// 라인 트레이스
	//FHitResult HitResult;
	//if (CollisionManager->LineTraceByObject(HitResult, ECollisionGroup::Player, { 0.f,0.f }, { (float)g_ptMouse.x,(float)g_ptMouse.y }, true, 0.f))
	//{
	//	// 라인 트레이스 맞은 대상의 콜라이더
	//	HitResult.HitCollision->SetHit(true);
	//}

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

	if (KeyManager::GetInstance()->IsOnceKeyDown(82))
	{
		snapShotManager->StartReplay(); // 리플레이
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
