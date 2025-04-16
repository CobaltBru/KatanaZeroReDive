#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "TilemapTool.h"
#include "Stage1Scene.h"
#include "MapTool.h"

#include "LoadingScene.h"
#include "SoundManager.h"

HRESULT MainGame::Init()
{
	ImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
	SoundManager::GetInstance()->Init();

	if (FAILED(InitSound()))
	{
		MessageBox(g_hWnd, L"InitSound Failed.", TEXT("���"), MB_OK);
		return E_FAIL;
	}
	

	//SceneManager::GetInstance()->AddScene("Ÿ�ϸ���", new TilemapTool());
	SceneManager::GetInstance()->AddScene("Stage1", new Stage1Scene());
	SceneManager::GetInstance()->AddScene("MapTool", new MapTool());
	SceneManager::GetInstance()->AddLoadingScene("�ε�_1", new LoadingScene());
	SceneManager::GetInstance()->ChangeScene("Stage1","�ε�_1");

	hdc = GetDC(g_hWnd);

	backBuffer = new Image();
	testDraw.AddImage(L"Image/dragon_idle.png", 12, 1);
	tmpTimer = 0;
	if (FAILED(backBuffer->Init(TILEMAPTOOL_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd, 
			TEXT("����� ���� ����"), TEXT("���"), MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

void MainGame::Release()
{
	if (backBuffer)
	{
		backBuffer->Release();
		delete backBuffer;
		backBuffer = nullptr;
	}

	ReleaseDC(g_hWnd, hdc);

	SceneManager::GetInstance()->Release();
	KeyManager::GetInstance()->Release();
	ImageManager::GetInstance()->Release();
	SoundManager::GetInstance()->Release();
}

void MainGame::Update()
{
	SceneManager::GetInstance()->Update();
	SoundManager::GetInstance()->Update();

	InvalidateRect(g_hWnd, NULL, false); 
	tmpTimer++;
	if (tmpTimer >= 12) tmpTimer = 0;
}

void MainGame::Render()
{
	// ����ۿ� ���� ����
	HDC hBackBufferDC = backBuffer->GetMemDC();
	Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hBackBufferDC);

	SceneManager::GetInstance()->Render(hBackBufferDC);

	TimerManager::GetInstance()->Render(hBackBufferDC);
	wsprintf(szText, TEXT("Mouse X : %d, Y : %d"), g_ptMouse.x, g_ptMouse.y);
	TextOut(hBackBufferDC, 20, 60, szText, wcslen(szText));
	testDraw.RenderRect(pGraphics , { 100.f,100.f } , 200 , 200 , GPImage::Pcolor::RED);
	testDraw.Middle_RenderFrame(pGraphics, { 100.f,100.f }, tmpTimer, false, 0.5f);
	/*testDraw.Render(pGraphics, { 120,100 }, 0.3f);
	testDraw.Render(pGraphics, { 140,100 }, 0.5f);
	testDraw.Render(pGraphics, { 160,100 }, 0.7f);
	testDraw.Render(pGraphics, { 180,100 }, 0.9f);
	testDraw.Render(pGraphics, { 200,100 }, 1.0f);*/
	// ����ۿ� �ִ� ������ ���� hdc�� ����

	delete pGraphics;
	backBuffer->Render(hdc);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

HRESULT MainGame::InitSound()
{
	// ��� ������ ���⿡ ���
	if (FAILED(SoundManager::GetInstance()->AddSound("Katana ZeroTest", "Sound/Katana ZeroTest.wav")))
		return E_FAIL;
	if (FAILED(SoundManager::GetInstance()->AddSound("EffectTest", "Sound/EffectTest.wav")))
		return E_FAIL;

	return S_OK;
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}
