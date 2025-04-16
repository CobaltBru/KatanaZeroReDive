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


#include "LineManager.h"

Stage1Scene::Stage1Scene()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr), snapShotManager(nullptr), ScrollManager(nullptr), LineManager(nullptr), elapsedTime(0.0f)
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


	ScrollManager = ScrollManager::GetInstance();
	ScrollManager->Init();
	ScrollManager->ZeroScroll();

	LineManager = LineManager::GetInstance();
	LineManager->Init();
	if (FAILED(LineManager->LoadFile()))
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

	SoundManager::GetInstance()->PlayBGM("Katana ZeroTest");

	return S_OK;
}

HRESULT Stage1Scene::InitImage()
{
	// 해당 씬에 필요한 모든 이미지 추가
	ImageManager::GetInstance()->AddImage("TestBg", L"Image/TestBg.bmp", 1639, 824, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("rocket", L"Image/rocket.bmp", 52, 64, 1, 1, true, RGB(255, 0, 255));

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
		taekyung->Init({ 500.f,550.f });
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
		chatManager = new ChatManager();
		chatManager->pushPos({ 600,100 });
		vector <pair<float, Token >> tokens;
		vector <pair<string, Token >> redSelects;
		vector <pair<string, Token >> selects;

		tokens.push_back(make_pair(0.f, Token(L"오늘은", { 0.f,0.f },
			Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::RED)));
		tokens.push_back(make_pair(0.7f, Token(L"점심으로", { 55,0.f },
			Token::APPEAR::DOOM, Token::OPTION::WAVE, Token::COLORS::RED)));
		tokens.push_back(make_pair(0.7f, Token(L"뭐먹어요", { 122.f,0.f },
			Token::APPEAR::DOOM, Token::OPTION::SHAKE, Token::COLORS::RED)));
		
		/*Chat* chat1 = new Chat();
		chat1->Init("test", tokens, 400.f, 50.f);
		chat1->setPos({ 700, 100 });
		ObjectManager->AddGameObject(EObjectType::GameObject, chat1);*/
		
		redSelects.push_back(make_pair("red1", Token(L"* 구내식당 * ", {0.f,0.f},
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::RED)));
		selects.push_back(make_pair("normal1", Token(L"육회바른연어", { 0,0.f },
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::WHITE)));
		selects.push_back(make_pair("normal2", Token(L"김밥천국", { 0,0.f },
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::WHITE)));
		selects.push_back(make_pair("normal3", Token(L"서브웨이", { 0,0.f },
			Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::WHITE)));

		OptionChat* oc = new OptionChat();
		oc->Init(tokens, 400.f, 50.f, 3.f, 10.f, redSelects, selects);
		chatManager->Push("Launch", "SELECT",0, oc);
		tokens.clear();
		redSelects.clear();
		selects.clear();

		tokens.push_back(make_pair(0.f, Token(L"얼른 가죠", { 0.f,0.f },
			Token::APPEAR::DOOM, Token::OPTION::WAVE, Token::COLORS::RED)));
		Chat* ch = new Chat();
		ch->Init(tokens, 100, 25);
		chatManager->Push("red1", "END", 0, ch);
		tokens.clear();

		tokens.push_back(make_pair(0.f, Token(L"점심특선이 ", { 0.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::WAVE, Token::COLORS::GREEN)));
		tokens.push_back(make_pair(0.f, Token(L"가성비가 좋아요 ", { 80.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::STOP, Token::COLORS::WHITE)));
		Chat* ch1 = new Chat();
		ch1->Init(tokens, 200, 25);
		chatManager->Push("normal1", "END", 0, ch1);
		tokens.clear();

		tokens.push_back(make_pair(0.f, Token(L"버거킹은", { 0.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::WAVE, Token::COLORS::YELLOW)));
		tokens.push_back(make_pair(0.f, Token(L"어떠신가요", { 70.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::STOP, Token::COLORS::WHITE)));
		Chat* ch2 = new Chat();
		ch2->Init(tokens, 160, 25);
		chatManager->Push("normal2", "END", 0, ch2);
		tokens.clear();

		tokens.push_back(make_pair(0.f, Token(L"흠..", { 0.f,0.f },
			Token::APPEAR::NORMAL, Token::OPTION::SHAKE, Token::COLORS::RED)));
		Chat* ch3 = new Chat();
		ch3->Init(tokens, 100, 25);
		chatManager->Push("normal3", "END", 0, ch3);
		tokens.clear();

		chatManager->startChat("Launch");
		ObjectManager->AddGameObject(EObjectType::GameObject, chatManager);
		
	}
	return S_OK;
}

void Stage1Scene::TestCode()
{
	// 플레이어 포커스 toggle
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_CONTROL))
		ScrollManager->SetFocus(!ScrollManager::GetInstance()->IsFocus());

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F2))
		SceneManager::GetInstance()->ChangeScene("MapTool", "로딩_1");

	if (KeyManager::GetInstance()->IsOnceKeyDown('C'))
	{
		// 인자 : 쉐이크 강도, 지속시간
		ScrollManager->CameraShake(5.f, 1.f);
	}
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
	ScrollManager->Update();

	TestCode();
}

void Stage1Scene::Render(HDC hdc)
{
	RenderManager->Render(hdc);
	CollisionManager->Render(hdc);

	LineManager->Render(hdc);
}

void Stage1Scene::Release()
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
	if (snapShotManager != nullptr)
		snapShotManager->Release();
	ObjectManager = nullptr;
	CollisionManager = nullptr;
	RenderManager = nullptr;
	ScrollManager = nullptr;
	LineManager = nullptr;
	snapShotManager = nullptr;
}
