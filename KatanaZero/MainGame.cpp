#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "TilemapTool.h"
#include "Stage1Scene.h"

#include "LoadingScene.h"
#include "SoundManager.h"

//static void RenderWaveEffect(HDC hdcDest, HDC hdcSource, const RECT& rect, float time)
//{
//	int width = rect.right - rect.left;
//	int height = rect.bottom - rect.top;
//
//	int amplitude = 40;
//	float cycle = 10.0f;
//	float speed = 10.0f;
//
//	for (int y = 0; y < height; y++)
//	{
//		int offset = (int)(amplitude * sin((y / cycle) + time * speed));
//		BitBlt(hdcDest, rect.left + offset, rect.top + y, width, 1,
//			hdcSource, rect.left, rect.top + y, SRCCOPY);
//	}
//}
HRESULT MainGame::Init()
{
	ImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
	SoundManager::GetInstance()->Init();

	if (FAILED(InitSound()))
	{
		MessageBox(g_hWnd, L"InitSound Failed.", TEXT("경고"), MB_OK);
		return E_FAIL;
	}
	

	//SceneManager::GetInstance()->AddScene("타일맵툴", new TilemapTool());
	SceneManager::GetInstance()->AddScene("Stage1", new Stage1Scene());
	SceneManager::GetInstance()->AddLoadingScene("로딩_1", new LoadingScene());
	SceneManager::GetInstance()->ChangeScene("Stage1","로딩_1");

	hdc = GetDC(g_hWnd);
	backBuffer = new Image();
	testDraw.AddImage(L"Image/dragon_idle.png", 12, 1);
	tmpTimer = 0;
	if (FAILED(backBuffer->Init(TILEMAPTOOL_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd, 
			TEXT("백버퍼 생성 실패"), TEXT("경고"), MB_OK);
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
	testDraw.Release();
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
	tmpTimer += TimerManager::GetInstance()->GetDeltaTime();
	if (tmpTimer > 0.1f)
	{
		frameIdx++;
		tmpTimer -= 0.1f;
		if (frameIdx >= 12) frameIdx = 0;
	}
}

void MainGame::Render()
{
	// 백버퍼에 먼저 복사
	HDC hBackBufferDC = backBuffer->GetMemDC();
	Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hBackBufferDC);

	SceneManager::GetInstance()->Render(hBackBufferDC);

	TimerManager::GetInstance()->Render(hBackBufferDC);
	wsprintf(szText, TEXT("Mouse X : %d, Y : %d"), g_ptMouse.x, g_ptMouse.y);
	TextOut(hBackBufferDC, 20, 60, szText, wcslen(szText));
	testDraw.RenderRect(pGraphics, { 200,100 }, 100, 100, GPImage::Pcolor::GREEN);
	/*testDraw.Render(pGraphics, {140,100}, 0.5f);
	testDraw.Render(pGraphics, { 160,100 }, 0.7f);
	testDraw.Render(pGraphics, { 180,100 }, 0.9f);
	testDraw.Render(pGraphics, { 200,100 }, 1.0f);*/
	// 백버퍼에 있는 내용을 메인 hdc에 복사
	testDraw.Middle_RenderFrameAngle(pGraphics, { 300,100 }, frameIdx, frameIdx * 30,false,0.5f);
	testDraw.RenderFrameAngle(pGraphics, { 200,100 }, frameIdx, frameIdx * 30);
	//화면 왜곡
	/*RECT effectRect = { WINSIZE_X / 4.f, WINSIZE_Y / 4.f, WINSIZE_X * (3.f / 4.f), WINSIZE_Y * (3.f / 4.f) };
	RenderWaveEffect(hBackBufferDC, hBackBufferDC, effectRect, tmpTimer);*/
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
	// 모든 음악은 여기에 등록
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
