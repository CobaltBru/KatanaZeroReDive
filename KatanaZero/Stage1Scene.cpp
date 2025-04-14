#include "Stage1Scene.h"
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
#include "SnapShotManager.h"



Stage1Scene::Stage1Scene()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr), snapShotManager(nullptr), elapsedTime(0.0f)
{
}

HRESULT Stage1Scene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	ObjectManager = ObjectManager::GetInstance();
	ObjectManager->Init();
	RenderManager = RenderManager::GetInstance();
	RenderManager->Init();
	CollisionManager = CollisionManager::GetInstance();
	CollisionManager->Init();

	snapShotManager = SnapShotManager::GetInstance();
	snapShotManager->Init();
	

	ScrollManager::GetInstance()->ZeroScroll();

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
		
	SoundManager::GetInstance()->PlayBGM("Katana ZeroTest");

 	return S_OK;
}

HRESULT Stage1Scene::InitImage()
{
	// 해당 씬에 필요한 모든 이미지 추가
	ImageManager::GetInstance()->AddImage("TestBg", L"Image/TestBg.bmp", 1639, 824, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("rocket", L"Image/rocket.bmp", 52, 64,1,1, true, RGB(255, 0, 255));

	return S_OK;
}

HRESULT Stage1Scene::InitObject()
{
	// 씬 초기에 필요한 오브젝트 생성

	// 테스트 코드 태경
	{
		Background* background = new Background();
		background->Init("TestBg");
		ObjectManager->AddGameObject(EObjectType::GameObject, background);

		TaeKyungObject* taekyung = new TaeKyungObject();
		taekyung->Init();
		ObjectManager->AddGameObject(EObjectType::GameObject, taekyung);

		TestObject* testObject = new TestObject();
		testObject->Init("rocket", { 1000.f,300.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, testObject);

		//해영 테스트
		{
			snapShotManager->AddGameObject(EObjectClassType::Player, taekyung);
			snapShotManager->AddGameObject(EObjectClassType::Enemy, testObject);
		}
	}
	// 테스트 코드 지운
	{
		/*Token* token1 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", { 0.f,0.f }, Token::APPEAR::NORMAL, Token::OPTION::STOP, Token::COLORS::GREEN);
		token1->setGlobalPos({ 200.f, 200.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token1);
		Token* token2 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", { 50.f,0.f }, Token::APPEAR::NORMAL, Token::OPTION::WAVE, Token::COLORS::SKY);
		token2->setGlobalPos({ 200.f,220.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token2);
		Token* token3 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", { 100.f,0.f }, Token::APPEAR::DOOM, Token::OPTION::WAVE, Token::COLORS::ORANGE);
		token3->setGlobalPos({ 200.f,240.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token3);
		Token* token4 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", { 150.f,0.f }, Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::PURPLE);
		token4->setGlobalPos({ 200.f,260.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token4);
		Token* token5 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", { 200.f,0.f }, Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::YELLOW);
		token5->setGlobalPos({ 200.f,280.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token5);*/
		vector <pair<float, Token >> tokens;
		tokens.push_back(make_pair(0.f, Token(L"오늘은", { 0.f,0.f },
			Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::RED)));
		tokens.push_back(make_pair(0.7f, Token(L"점심으로", { 55,0.f },
			Token::APPEAR::DOOM, Token::OPTION::WAVE, Token::COLORS::RED)));
		tokens.push_back(make_pair(0.7f, Token(L"뭐먹어요", { 122.f,0.f },
			Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::RED)));

		Chat* chat1 = new Chat();
		chat1->Init("test", tokens, 400.f, 50.f);
		chat1->setPos({ 700, 100 });
		ObjectManager->AddGameObject(EObjectType::GameObject, chat1);

		vector <pair<string, Token >> selects;
		selects.push_back(make_pair("test1", Token(L"* 구내식당* ", {0.f,0.f},
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::RED)));
		selects.push_back(make_pair("test2", Token(L"육회바른연어", { 0,0.f },
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::WHITE)));
		selects.push_back(make_pair("test3", Token(L"김밥천국", { 0,0.f },
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::WHITE)));


		OptionChat* oc = new OptionChat();
		oc->Init(3.f, 10.f, selects);
		ObjectManager->AddGameObject(EObjectType::GameObject, oc);
	}
	return S_OK;
}

void Stage1Scene::Update()
{
	ObjectManager->Update();
	CollisionManager->Update();
	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	if (elapsedTime >= 5.0f)
	{
		snapShotManager->Update(true);
	}
	else
	{
		snapShotManager->Update(false);
		
	}
}

void Stage1Scene::Render(HDC hdc)
{
	RenderManager->Render(hdc);
	CollisionManager->Render(hdc);
	
}

void Stage1Scene::Release()
{
	ObjectManager->Release();
	CollisionManager->Release();
	RenderManager->Release();	
}
