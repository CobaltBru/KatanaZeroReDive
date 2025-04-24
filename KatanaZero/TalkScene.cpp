#include "TalkScene.h"
#include "CommonFunction.h"

#include "Background.h"
#include "Animation.h"
#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "ScrollManager.h"
#include "GPImage.h"
#include "Image.h"
#include "SoundManager.h"
#include "ChatManager.h"

#include "LineManager.h"

#include "ScenePsych.h"
#include "ScenePlayer.h"

void TalkScene::renderEffect(HDC hdc)
{

	int width = WINSIZE_X;
	int height = WINSIZE_Y;
	float offset = -20 * (effectTimer) * (effectTimer - 2.f);
	for (int y = 0; y < height; y++)
	{
		BitBlt(hdc, offset, y, width, 1,
			hdc,0, y, SRCPAINT);
	}
}



TalkScene::TalkScene() :
	ObjectManager(nullptr), RenderManager(nullptr), LineManager(nullptr),CollisionManager(nullptr), chatManager(nullptr)
{
}

HRESULT TalkScene::Init()
{
	SetClientRect(g_hWndParent, WINSIZE_X, WINSIZE_Y);
	ObjectManager = ObjectManager::GetInstance();
	ObjectManager->Init();
	RenderManager = RenderManager::GetInstance();
	RenderManager->Init();
	LineManager = LineManager::GetInstance();
	LineManager->Init();
	CollisionManager = CollisionManager::GetInstance();
	CollisionManager->Init();

	chatManager = new ChatManager();
	chatManager->pushPos({ 865.f,500.f });
	chatManager->LoadChat("ChatDatas/Talkscene.json");
	inChat = false;
	chatManager->startChat("UnBP5gsTpB");
	badChat = false;
	chatDone = false;
	firstdone = false;
	pos = { 0,0 };
	chairPos = { 695.f,640.f };
	DoorPos = { 247.f,600.f };

	drugTimerOn = false;
	drugTimer = 0;
	effectTimer = 0;
	effectOn = false;
	if (FAILED(LineManager->LoadFile(L"Data/Talk/talkLine.dat")))
	{
		MessageBox(g_hWnd, TEXT("TestScene LineManager LoadFile Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}
	
	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("TalkScene InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("TalkScene InitObject Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	SoundManager::GetInstance()->PlayBGM("pyshroom");

	return S_OK;
}

void TalkScene::Release()
{
	if (ObjectManager != nullptr)
		ObjectManager->Release();
	if (RenderManager != nullptr)
		RenderManager->Release();
	if (LineManager != nullptr)
		LineManager->Release();
	if (CollisionManager != nullptr)
		CollisionManager->Release();
	if (chatManager != nullptr)
	{
		chatManager->Release();
		delete chatManager;
		chatManager = nullptr;
	}
		

	ScrollManager::GetInstance()->Release();
	ObjectManager = nullptr;
	RenderManager = nullptr;
	LineManager = nullptr;
	CollisionManager = nullptr;
}

void TalkScene::Update()
{
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	ObjectManager->Update();
	CollisionManager->Update();
	ScrollManager::GetInstance()->Update();
	if (!inChat)
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
		{
			FPOINT playerPos = player->GetPos();
			if (fabs(playerPos.x - chairPos.x) < 15.f)
			{

				player->SetPos(chairPos);
				player->sitUP();
				if (chatDone == false)
				{
					chatManager->startChat("T6qxd3wono");
					inChat = true;
					chatDone = true;
				}
				
			}

			if (fabs(playerPos.x - DoorPos.x) < 20.f)
			{
				if (chatDone)
				{
					SceneManager::GetInstance()->ChangeScene("Test", "로딩_1");
					return;
				}
			}

		}
	}
	
	chatManager->Update();

	if (inChat)
	{
		if (chatManager->getKey() == "X0CXrB2qMG" && chatManager->checkChatComplete("X0CXrB2qMG"))
		{
			psych->Move();
			player->getChronos();
			badChat = true;
			firstdone = true;
		}

		//약맞으러
		if (chatManager->getKey() == "N4bTsm5YwY" && chatManager->checkChatComplete("N4bTsm5YwY"))
		{
			psych->Move();
			player->getChronos();
			firstdone = true;
			
		}
		if ((chatManager->getKey() == "" || chatManager->getKey() == "END") && inChat)
		{
			if (firstdone)
			{
				drugTimerOn = true;
			}
			if (!badChat)
			{
				//약 다맞으면
				if (psych->isMoveEnd())
				{
					chatManager->startChat("jlHjfE32wz");
				}
			}
			else
			{
				//약 다맞으면
				if (psych->isMoveEnd())
				{
					chatManager->startChat("mpYito8hu6");
				}
			}
		}
			

		//끝
		if (chatManager->getKey() == "mpYito8hu6" && chatManager->checkChatComplete("mpYito8hu6"))
		{
			inChat = false;
			timer += TimerManager::GetInstance()->GetDeltaTime();
			if (timer >= 3.f)
			{
				chatManager->startChat("");
			}
			
		}
	}
	if (chatDone == true && inChat == false)
	{
		timer += TimerManager::GetInstance()->GetDeltaTime();
		if (timer >= 3.f)
		{
			chatManager->startChat("");
		}
	}
	if (drugTimerOn)
	{
		drugTimer += dt;
		if (drugTimer >= 10.55f)
		{
			effectTimer += dt;
			if (effectTimer >= 2.0f)
			{
				effectOn = false;
				drugTimerOn = false;
			}
			else
			{
				effectOn = true;
			}
		}
	}
	
}

void TalkScene::Render(HDC hdc)
{
	
	RenderManager->Render(hdc);
	LineManager->Render(hdc);
	CollisionManager->Render(hdc);
	chatManager->Render(hdc);
	RECT rect = { 0,0,WINSIZE_X,WINSIZE_Y };
	//RenderWaveEffect(hdc, hdc, rect, timer);
	if(effectOn) renderEffect(hdc);

	
}


HRESULT TalkScene::InitImage()
{
	ImageManager::GetInstance()->AddImage("BlackBackground", L"Image/Background/black.bmp", 1600, 900, 1, 1);
	return S_OK;
}

HRESULT TalkScene::InitObject()
{
	Background* bg = new Background();
	bg->Init("BlackBackground");
	ObjectManager->AddGameObject(EObjectType::GameObject, bg);
	Image* tmp;
	tmp = ImageManager::GetInstance()->AddImage("talk_bg", L"Image/TalkScene/bg_psychiatrist.bmp", 1597, 900, 1, 1, true, RGB(255, 0, 255));
	background = new Animation();
	background->Init(tmp, 1);
	background->setPos({ 0.f,0.f }, false, false);
	background->setAniTask({ {0,10.f} });
	background->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, background);

	tmp = ImageManager::GetInstance()->AddImage("talk_fireplace", L"Image/TalkScene/spr_psych_fireplace.bmp", 595, 72, 7, 1, true, RGB(255, 0, 255));
	firePlace = new Animation();
	firePlace->Init(tmp, 7);
	firePlace->setPos({ 740,578 }, false, false);
	firePlace->setAniTask({
		{0,0.1f}, { 1,0.1f }, { 2,0.1f },
		{3,0.1f}, { 4,0.1f }, { 5,0.1f } ,
		{6,0.1f} });
	firePlace->On();
	ObjectManager->AddGameObject(EObjectType::GameObject, firePlace);

	

	player = new ScenePlayer();
	player->SetPos(DoorPos);
	player->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, player);

	psych = new ScenePsych();
	psych->SetPos({ 865.f,637.f });
	psych->Init();
	ObjectManager->AddGameObject(EObjectType::GameObject, psych);
	

	return S_OK;
}
