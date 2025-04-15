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
#include "LineManager.h"

Stage1Scene::Stage1Scene()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr),ScrollManager(nullptr)
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

	ScrollManager = ScrollManager::GetInstance();
	ScrollManager->Init();
	ScrollManager->ZeroScroll();

	LineManager::GetInstance()->Init();

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
	}

	return S_OK;
}

void Stage1Scene::TestCode()
{
	// �׽�Ʈ �ڵ�
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON))
	{
		const FPOINT Scroll = ScrollManager->GetScroll();
		LineManager::GetInstance()->AddLine(g_ptMouse.x - Scroll.x, g_ptMouse.y - Scroll.y);
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD0))
	{
		LineManager::GetInstance()->ResetLinePoint();
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD1))
	{
		LineManager::GetInstance()->SetLineType(ELineType::Normal);
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD2))
	{
		LineManager::GetInstance()->SetLineType(ELineType::Wall);

	}
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD3))
	{
		LineManager::GetInstance()->SetLineType(ELineType::DownLine);
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_NUMPAD4))
	{
		LineManager::GetInstance()->SetLineType(ELineType::Ceiling);
	}
	//����
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F1))
	{
		LineManager::GetInstance()->SaveFile();
	}


	// �÷��̾� ��Ŀ�� toggle
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_CONTROL))
		ScrollManager::GetInstance()->SetFocus(!ScrollManager::GetInstance()->IsFocus());
}

void Stage1Scene::Update()
{
	ObjectManager->Update();
	CollisionManager->Update();

	TestCode();
}

void Stage1Scene::Render(HDC hdc)
{
	RenderManager->Render(hdc);
	CollisionManager->Render(hdc);

	LineManager::GetInstance()->Render(hdc);
}

void Stage1Scene::Release()
{
	ObjectManager->Release();
	CollisionManager->Release();
	RenderManager->Release();	
	ScrollManager->Release();
	LineManager::GetInstance()->Release();
}
