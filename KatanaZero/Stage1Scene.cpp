#include "Stage1Scene.h"
#include "CommonFunction.h"

#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"

#include "TaeKyungObject.h"

HRESULT Stage1Scene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	if (FAILED(ImageInit()))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene ImageInit Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(ObjectInit()))
	{
		MessageBox(g_hWnd, TEXT("Stage1Scene ObjectInit Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}
		
 	return S_OK;
}

HRESULT Stage1Scene::ImageInit()
{
	// 해당 씬에 필요한 모든 이미지 추가
	return S_OK;
}

HRESULT Stage1Scene::ObjectInit()
{
	// 씬 초기에 필요한 오브젝트 생성

	// 테스트 코드 태경
	{
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
