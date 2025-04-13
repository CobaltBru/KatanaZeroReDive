#include "Stage1Scene.h"
#include "CommonFunction.h"

#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "CollisionManager.h"

#include "TaeKyungObject.h"
#include "Background.h"
#include "Boss.h"

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

	if (FAILED(ImageInit()))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene ImageInit Failed."), TEXT("����"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(ObjectInit()))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene ObjectInit Failed."), TEXT("����"), MB_OK);
		return E_FAIL;
	}
		
 	return S_OK;
}

HRESULT Stage1Scene::ImageInit()
{
	// �ش� ���� �ʿ��� ��� �̹��� �߰�
	ImageManager::GetInstance()->AddImage("BackGround", L"Image/BackGround.bmp", 1080, 500, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("rocket", L"Image/rocket.bmp", 52, 64,1,1, true, RGB(255, 0, 255));

	return S_OK;
}

HRESULT Stage1Scene::ObjectInit()
{
	// �� �ʱ⿡ �ʿ��� ������Ʈ ����

	// �׽�Ʈ �ڵ� �°�
	{
		Background* background = new Background();
		background->Init("BackGround");
		ObjectManager->AddGameObject(EObjectType::GameObject, background);

		TaeKyungObject* taekyung = new TaeKyungObject();
		taekyung->Init();
		ObjectManager->AddGameObject(EObjectType::GameObject, taekyung);

		Boss* boss = new Boss();
		boss->Init();
		ObjectManager->AddGameObject(EObjectType::GameObject, boss);
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
