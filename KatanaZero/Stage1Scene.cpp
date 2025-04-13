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

Stage1Scene::Stage1Scene()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr)
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

	}
	// 테스트 코드 지운
	{
		Token* token1 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", Token::APPEAR::NORMAL, Token::OPTION::STOP, Token::COLORS::GREEN);
		token1->setPos({ 200.f,200.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token1);
		Token* token2 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", Token::APPEAR::NORMAL, Token::OPTION::WAVE, Token::COLORS::SKY);
		token2->setPos({ 200.f,220.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token2);
		Token* token3 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", Token::APPEAR::DOOM, Token::OPTION::WAVE, Token::COLORS::ORANGE);
		token3->setPos({ 200.f,240.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token3);
		Token* token4 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::PURPLE);
		token4->setPos({ 200.f,260.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token4);
		Token* token5 = new Token(L"HelloWorld!HelloWorld!HelloWorld!", Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::YELLOW);
		token5->setPos({ 200.f,280.f });
		ObjectManager->AddGameObject(EObjectType::GameObject, token5);
	}

	return S_OK;
}

void Stage1Scene::Update()
{
	ObjectManager->Update();
	CollisionManager->Update();
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
