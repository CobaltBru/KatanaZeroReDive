#include "Stage1Scene.h"
#include "CommonFunction.h"

#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"

#include "TaeKyungObject.h"
#include "Background.h"

HRESULT Stage1Scene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

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
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, background);

		TaeKyungObject* taekyung = new TaeKyungObject();
		taekyung->Init();
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, taekyung);
	}

	return S_OK;
}

void Stage1Scene::Update()
{
	ObjectManager::GetInstance()->Update();
}

void Stage1Scene::Render(HDC hdc)
{
	RenderManager::GetInstance()->Render(hdc);
}

void Stage1Scene::Release()
{
	ObjectManager::GetInstance()->Release();
	RenderManager::GetInstance()->Release();
}
