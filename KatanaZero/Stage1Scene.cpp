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
		MessageBox(g_hWnd, TEXT("Stage1Scene InitImage Failed."), TEXT("����"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene InitObject Failed."), TEXT("����"), MB_OK);
		return E_FAIL;
	}
		
	SoundManager::GetInstance()->PlayBGM("Katana ZeroTest");

 	return S_OK;
}

HRESULT Stage1Scene::InitImage()
{
	// �ش� ���� �ʿ��� ��� �̹��� �߰�
	ImageManager::GetInstance()->AddImage("TestBg", L"Image/TestBg.bmp", 1639, 824, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("rocket", L"Image/rocket.bmp", 52, 64,1,1, true, RGB(255, 0, 255));

	return S_OK;
}

HRESULT Stage1Scene::InitObject()
{
	// �� �ʱ⿡ �ʿ��� ������Ʈ ����

	// �׽�Ʈ �ڵ� �°�
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

		//�ؿ� �׽�Ʈ
		{
			snapShotManager->AddGameObject(EObjectClassType::Player, taekyung);
			snapShotManager->AddGameObject(EObjectClassType::Enemy, testObject);
		}
	}
	// �׽�Ʈ �ڵ� ����
	{
		ChatManager* chatManager = new ChatManager();
		
		vector <pair<float, Token >> tokens;
		vector <pair<string, Token >> redSelects;
		vector <pair<string, Token >> selects;
		tokens.push_back(make_pair(0.f, Token(L"������", { 0.f,0.f },
			Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::RED)));
		tokens.push_back(make_pair(0.7f, Token(L"��������", { 55,0.f },
			Token::APPEAR::DOOM, Token::OPTION::WAVE, Token::COLORS::RED)));
		tokens.push_back(make_pair(0.7f, Token(L"���Ծ��", { 122.f,0.f },
			Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::RED)));
		

		/*Chat* chat1 = new Chat();
		chat1->Init("test", tokens, 400.f, 50.f);
		chat1->setPos({ 700, 100 });
		ObjectManager->AddGameObject(EObjectType::GameObject, chat1);*/
		
		redSelects.push_back(make_pair("red1", Token(L"* �����Ĵ� * ", {0.f,0.f},
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::RED)));
		selects.push_back(make_pair("normal1", Token(L"��ȸ�ٸ�����", { 0,0.f },
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::WHITE)));
		selects.push_back(make_pair("normal2", Token(L"���õ��", { 0,0.f },
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::WHITE)));
		selects.push_back(make_pair("normal3", Token(L"�������", { 0,0.f },
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::WHITE)));

		OptionChat* oc = new OptionChat();
		oc->Init(tokens, 400.f, 50.f, 3.f, 10.f, redSelects, selects);

		chatManager->Push("Launch", "SELECT", { 600,100 }, oc);

		tokens.clear();
		redSelects.clear();
		selects.clear();

		tokens.push_back(make_pair(0.f, Token(L"�� ����", { 0.f,0.f },
			Token::APPEAR::DOOM, Token::OPTION::WAVE, Token::COLORS::RED)));
		Chat* ch = new Chat();
		ch->Init(tokens, 100, 25);
		chatManager->Push("red1", "END", { 600,100 }, ch);
		tokens.clear();

		tokens.push_back(make_pair(0.f, Token(L"����Ư���� ", { 0.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::WAVE, Token::COLORS::GREEN)));
		tokens.push_back(make_pair(0.f, Token(L"������ ���ƿ� ", { 80.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::STOP, Token::COLORS::WHITE)));
		Chat* ch1 = new Chat();
		ch1->Init(tokens, 200, 25);
		chatManager->Push("normal1", "END", { 600,100 }, ch1);
		tokens.clear();

		tokens.push_back(make_pair(0.f, Token(L"����ŷ��", { 0.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::WAVE, Token::COLORS::YELLOW)));
		tokens.push_back(make_pair(0.f, Token(L"��Ű���", { 70.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::STOP, Token::COLORS::WHITE)));
		Chat* ch2 = new Chat();
		ch2->Init(tokens, 160, 25);
		chatManager->Push("normal2", "END", { 600,100 }, ch2);
		tokens.clear();

		tokens.push_back(make_pair(0.f, Token(L"��..", { 0.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::SHAKE, Token::COLORS::RED)));
		Chat* ch3 = new Chat();
		ch3->Init(tokens, 100, 25);
		chatManager->Push("normal3", "END", { 600,100 }, ch3);
		tokens.clear();
		ObjectManager->AddGameObject(EObjectType::GameObject, chatManager);
		chatManager->startChat("Launch");
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
