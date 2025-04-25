#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "Stage1Scene.h"
#include "HYScene.h"
#include "TestScene.h"
#include "MapTool.h"

#include "TalkScene.h"
#include "HomeScene.h"
#include "LoadingScene.h"
#include "HiddenScene.h"
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
	srand(time(NULL));
	ImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
	SoundManager::GetInstance()->Init();
	
	if (FAILED(InitSound()))
	{
		MessageBox(g_hWnd, L"InitSound Failed.", TEXT("경고"), MB_OK);
		return E_FAIL;
	}
	SceneManager::GetInstance()->AddScene("Talk", new TalkScene());
	SceneManager::GetInstance()->AddScene("Test", new TestScene());
	SceneManager::GetInstance()->AddScene("Home", new HomeScene());
	SceneManager::GetInstance()->AddScene("Stage1", new Stage1Scene());
	SceneManager::GetInstance()->AddScene("HY", new HYScene());
	SceneManager::GetInstance()->AddScene("MapTool", new MapTool());
	SceneManager::GetInstance()->AddScene("Hidden", new HiddenScene());	
	SceneManager::GetInstance()->AddLoadingScene("로딩_1", new LoadingScene());
	SceneManager::GetInstance()->ChangeScene("Hidden","로딩_1");

	hdc = GetDC(g_hWnd);
	backBuffer = new Image();
	
	int nFontsAdded = AddFontResourceEx(L"Font/DungGeunMo.ttf", FR_PRIVATE, 0);
	if (nFontsAdded == 0)
	{
		MessageBox(NULL, L"폰트 로드 실패", L"Error", MB_OK);
	}
	LOGFONT lf = { 0 };
	lf.lfHeight = -MulDiv(18, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	wcscpy_s(lf.lfFaceName, L"DungGeunMo");

	hFont = CreateFontIndirect(&lf);
	if (!hFont)
	{
		MessageBox(NULL, L"폰트 생성 실패", L"Error", MB_OK);
	}

	if (FAILED(backBuffer->Init(WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("백버퍼 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, L"InitImage Failed.", TEXT("경고"), MB_OK);
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
	// 백버퍼에 먼저 복사
	HDC hBackBufferDC = backBuffer->GetMemDC();
	hOldFont = (HFONT)SelectObject(hBackBufferDC, hFont);

	Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hBackBufferDC);

	SceneManager::GetInstance()->Render(hBackBufferDC);

	//TimerManager::GetInstance()->Render(hBackBufferDC);
	/*wsprintf(szText, TEXT("Mouse X : %d, Y : %d"), g_ptMouse.x, g_ptMouse.y);
	TextOut(hBackBufferDC, 20, 60, szText, wcslen(szText))*/;

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
	if (FAILED(SoundManager::GetInstance()->AddSound("HomeOST", "Sound/HomeOST.mp3")))
		return E_FAIL;

	//talk
	{
		if (FAILED(SoundManager::GetInstance()->AddSound("pyshstep01", "Sound/talkScene/sound_npc_therapist_footstep_01.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("pyshstep02", "Sound/talkScene/sound_npc_therapist_footstep_02.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("pyshstep03", "Sound/talkScene/sound_npc_therapist_footstep_03.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("pyshstep04", "Sound/talkScene/sound_npc_therapist_footstep_04.wav")))
			return E_FAIL;

		if (FAILED(SoundManager::GetInstance()->AddSound("pyshflick", "Sound/talkScene/sound_npc_therapistmeds_flick_01.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("pyshinject", "Sound/talkScene/sound_npc_therapistmeds_inject_01.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("pyshdrugffect", "Sound/talkScene/sound_ambience_drug_effect_01.ogg")))
			return E_FAIL;

		if (FAILED(SoundManager::GetInstance()->AddSound("pyshroom", "Sound/talkScene/song_nocturne.ogg")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("pyshmove", "Sound/talkScene/sound_npc_therapist_sleeve_01.wav")))
			return E_FAIL;

		if (FAILED(SoundManager::GetInstance()->AddSound("chatnormal", "Sound/chat/sound_PA_talk.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("chatdoom", "Sound/chat/sound_ui_dialogue_emphasis_01.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("chatexplode", "Sound/chat/sound_ui_dialogue_shatter_01.wav")))
			return E_FAIL;


		if (FAILED(SoundManager::GetInstance()->AddSound("menumove", "Sound/menu_move.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("menuselect", "Sound/menu_select.wav")))
			return E_FAIL;

		if (FAILED(SoundManager::GetInstance()->AddSound("flicker1", "Sound/sound_object_neon_flicker_01.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("flicker2", "Sound/sound_object_neon_flicker_02.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("flicker3", "Sound/sound_object_neon_flicker_03.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("flicker5", "Sound/sound_object_neon_flicker_05.wav")))
			return E_FAIL;
	}
	//home
	{

	}


	//PickUp
	{
		if (FAILED(SoundManager::GetInstance()->AddSound("sound_player_throw", "Sound/PickUp/sound_player_throw.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("sound_player_grabtea", "Sound/PickUp/sound_player_grabtea.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("sound_enemy_death_bottle", "Sound/PickUp/sound_enemy_death_bottle.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("sound_clatter_glass2", "Sound/PickUp/sound_clatter_glass2.wav")))
			return E_FAIL;
		if (FAILED(SoundManager::GetInstance()->AddSound("sound_clatter_glass1", "Sound/PickUp/sound_clatter_glass1.wav")))
			return E_FAIL;

		if (FAILED(SoundManager::GetInstance()->AddSound("BossBossBoss", "Sound/BossBossBoss.mp3")))
			return E_FAIL;

		if (FAILED(SoundManager::GetInstance()->AddSound("sound_cutblack", "Sound/sound_cutblack.wav")))
			return E_FAIL;
		
	}

	return S_OK;
}

HRESULT MainGame::InitImage()
{
	ImageManager::GetInstance()->AddImage("spr_beer_bottle_3_0_broke", L"Image/Bottle/spr_beer_bottle_3_0_broke.bmp", 4, 4, 1, 1, false);
	ImageManager::GetInstance()->AddImage("spr_beer_bottle_4_0_broke", L"Image/Bottle/spr_beer_bottle_4_0_broke.bmp", 4, 4, 1, 1, false);
	ImageManager::GetInstance()->AddImage("spr_pickuparrow_anim", L"Image/UI/spr_pickuparrow_anim.bmp", 144, 21, 8, 1, true,RGB(255,0,255));
	ImageManager::GetInstance()->AddImage("normalslash", L"Image/fx/NormalSlash.bmp", 530, 32, 5, 1, true, RGB(255, 255, 255));

	ImageManager::GetInstance()->AddImage("spr_psychboss_giant", L"Image/Hidden/spr_psychboss_giant.bmp", 11352, 235, 24, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("spr_psychboss_giant_lilguy", L"Image/Hidden/spr_psychboss_giant_lilguy.bmp", 4848, 126, 24, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("spr_psychboss_giant_face_idle", L"Image/Hidden/spr_psychboss_giant_face_idle.bmp", 954, 231, 6, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("spr_psychboss_giant_face_hurt", L"Image/Hidden/spr_psychboss_giant_face_hurt.bmp", 1449, 234, 9, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("spr_psychboss_giant_tentacle_idle", L"Image/Hidden/spr_psychboss_giant_tentacle_idle.bmp", 1008, 163, 12, 1, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("spr_psychboss_giant_tentacle_stab", L"Image/Hidden/spr_psychboss_giant_tentacle_stab.bmp", 400, 216, 5, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("spr_psychboss_giant_tentacle_stab_end", L"Image/Hidden/spr_psychboss_giant_tentacle_stab_end.bmp", 276, 216, 4, 1, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("spr_psychboss_stabber_0", L"Image/Hidden/spr_psychboss_stabber_0.bmp", 36, 450, 1, 1, true, RGB(255, 0, 255));
	
	ImageManager::GetInstance()->AddImage("spr_psychboss_tentacle_hurt_", L"Image/Hidden/spr_psychboss_tentacle_hurt_.bmp", 204, 105, 6, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("spr_psychboss_tentacle_idle_", L"Image/Hidden/spr_psychboss_tentacle_idle_.bmp", 210, 96, 7, 1, true, RGB(255, 0, 255));

	InitBackground();	

	return S_OK;
}

void MainGame::InitBackground()
{
	vector<string> backgrounds = GetFileNames("Image/Background/*.bmp");
	if (!backgrounds.empty())
	{
		for (int i = 0; i < backgrounds.size(); ++i)
		{
			int dotPos = backgrounds[i].find_last_of('.');
			string nameOnly = dotPos != string::npos ? backgrounds[i].substr(0, dotPos) : backgrounds[i];

			wstring wsPath = L"Image/Background/";
			wsPath += wstring(backgrounds[i].begin(), backgrounds[i].end());

			ImageManager::GetInstance()->AddImage(nameOnly, wsPath.c_str(), true, RGB(255, 0, 255));
		}
	}
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
		// 백버퍼에 있는 내용을 메인 hdc에 복사
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

				//화면 왜곡
				/*RECT effectRect = { WINSIZE_X / 4.f, WINSIZE_Y / 4.f, WINSIZE_X * (3.f / 4.f), WINSIZE_Y * (3.f / 4.f) };
				RenderWaveEffect(hBackBufferDC, hBackBufferDC, effectRect, tmpTimer);*/