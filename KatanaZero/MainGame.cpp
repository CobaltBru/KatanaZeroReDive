#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "Stage1Scene.h"
#include "TestScene.h"
#include "MapTool.h"

#include "TalkScene.h"
#include "HomeScene.h"
#include "LoadingScene.h"
#include "SoundManager.h"

#include "ScreenEffectManager.h"
#include "SnapShotManager.h"
#include "ScrollManager.h"
#include "ObjectManager.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "EffectManager.h"



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
	SceneManager::GetInstance()->AddScene("Talk", new TalkScene());
	SceneManager::GetInstance()->AddScene("Test", new TestScene());
	SceneManager::GetInstance()->AddScene("Home", new HomeScene());
	SceneManager::GetInstance()->AddScene("Stage1", new Stage1Scene());
	SceneManager::GetInstance()->AddScene("MapTool", new MapTool());
	SceneManager::GetInstance()->AddLoadingScene("�ε�_1", new LoadingScene());
	SceneManager::GetInstance()->ChangeScene("Stage1","�ε�_1");

	hdc = GetDC(g_hWnd);
	backBuffer = new Image();

	int nFontsAdded = AddFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, 0);
	if (nFontsAdded == 0)
	{
		MessageBox(NULL, L"��Ʈ �ε� ����", L"Error", MB_OK);
	}
	LOGFONT lf = { 0 };
	lf.lfHeight = -MulDiv(18, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	wcscpy_s(lf.lfFaceName, L"DungGeunMo");

	hFont = CreateFontIndirect(&lf);
	if (!hFont)
	{
		MessageBox(NULL, L"��Ʈ ���� ����", L"Error", MB_OK);
	}

	if (FAILED(backBuffer->Init(WINSIZE_X, WINSIZE_Y)))
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

	//ScreenEffectManager::GetInstance()->Release();
	//SnapShotManager::GetInstance()->Release();
	//ScrollManager::GetInstance()->Release();
	//ObjectManager::GetInstance()->Release();
	//RenderManager::GetInstance()->Release();
	//CollisionManager::GetInstance()->Release();
	//EffectManager::GetInstance()->Release();


	SceneManager::GetInstance()->Release();
	KeyManager::GetInstance()->Release();
	ImageManager::GetInstance()->Release();
	SoundManager::GetInstance()->Release();
	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);
	RemoveFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, 0);
}

void MainGame::Update()
{
	SceneManager::GetInstance()->Update();
	SoundManager::GetInstance()->Update();

	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	// ����ۿ� ���� ����
	HDC hBackBufferDC = backBuffer->GetMemDC();
	hOldFont = (HFONT)SelectObject(hBackBufferDC, hFont);

	Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hBackBufferDC);

	SceneManager::GetInstance()->Render(hBackBufferDC);

	TimerManager::GetInstance()->Render(hBackBufferDC);
	wsprintf(szText, TEXT("Mouse X : %d, Y : %d"), g_ptMouse.x, g_ptMouse.y);
	TextOut(hBackBufferDC, 20, 60, szText, wcslen(szText));

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
	if (FAILED(SoundManager::GetInstance()->AddSound("HomeOST", "Sound/HomeOST.mp3")))
		return E_FAIL;

	return S_OK;
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

//testDraw.AddImage(L"Image/dragon_idle.png",12,1);
//testDraw.AddImage(L"Image/UI/Home/spr_titlegraphic_0.png",1,1);
// testDraw.Release();
// tmpTimer = 0;
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

/*tmpTimer += TimerManager::GetInstance()->GetDeltaTime();
	if (tmpTimer > 0.1f)
	{
		frameIdx++;
		tmpTimer -= 0.1f;
		if (frameIdx >= 12) frameIdx = 0;
	}*/

	//testDraw.RenderRect(pGraphics, { 200,100 }, 100, 100, GPImage::Pcolor::GREEN);
		/*testDraw.Render(pGraphics, {140,100}, 0.5f);
		testDraw.Render(pGraphics, { 160,100 }, 0.7f);
		testDraw.Render(pGraphics, { 180,100 }, 0.9f);
		testDraw.Render(pGraphics, { 200,100 }, 1.0f);*/
		// ����ۿ� �ִ� ������ ���� hdc�� ����
		/*testDraw.RenderAll(pGraphics, { 0,0 }, 0, 0, false,
			1.0f, 1.0f, 1.0f, 1.0f, 1.f, 1.f);*/
			/*testDraw.RenderAll(pGraphics, { 200,100 }, frameIdx, frameIdx * 30, true,
				1.0f, 1.0f, 0.5f, 0.5f, 1.0f, 1.0f);*/
				/*testDraw.RenderAll(pGraphics, { 100,100 });
				testDraw.RenderAll(pGraphics, { 130,100 });
				testDraw.RenderAll(pGraphics, { 160,100 });
				testDraw.RenderAll(pGraphics, { 190,100 });
				testDraw.RenderAll(pGraphics, { 220,100 });
				testDraw.RenderAll(pGraphics, { 250,100 });
				testDraw.RenderAll(pGraphics, { 280,100 });
				testDraw.RenderAll(pGraphics, { 310,100 });
				testDraw.RenderAll(pGraphics, { 340,100 });
				testDraw.RenderAll(pGraphics, { 370,100 });*/

				//ȭ�� �ְ�
				/*RECT effectRect = { WINSIZE_X / 4.f, WINSIZE_Y / 4.f, WINSIZE_X * (3.f / 4.f), WINSIZE_Y * (3.f / 4.f) };
				RenderWaveEffect(hBackBufferDC, hBackBufferDC, effectRect, tmpTimer);*/