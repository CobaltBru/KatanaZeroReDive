#include "MapTool.h"
#include "CommonFunction.h"

#include "ObjectManager.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "ScrollManager.h"

#include "Background.h"
#include "SoundManager.h"
#include "LineManager.h"
#include "SnapShotManager.h"

#include "SimpleObject.h"
#include "SimpleTestObject.h"
#include "EffectManager.h"
#include "EffectManager.h"

// 해영 Enemy 테스트
#include "Enemy.h"
#include "Enemies.h"

#include "Factory.h"
#include "ImGuiManager.h"

static TCHAR filter[] = L"모든 파일\0*.*\0dat 파일\0*.dat";

MapTool::MapTool()
	:ObjectManager(nullptr), RenderManager(nullptr), CollisionManager(nullptr), ScrollManager(nullptr), LineManager(nullptr)
{
}

HRESULT MapTool::Init()
{
	SoundManager::GetInstance()->StopAll();

	SetClientRect(g_hWndParent, WINSIZE_X + TILEMAPTOOL_X, WINSIZE_Y);

	ObjectManager = ObjectManager::GetInstance();
	ObjectManager->Init();
	RenderManager = RenderManager::GetInstance();
	RenderManager->Init();
	CollisionManager = CollisionManager::GetInstance();
	CollisionManager->Init();

	ScrollManager = ScrollManager::GetInstance();
	ScrollManager->Init();
	ScrollManager->ZeroScroll();

	LineManager = LineManager::GetInstance();
	LineManager->Init();

	ImGuiManager::GetInstance()->Init(LineManager, ObjectManager, ScrollManager, std::bind(&MapTool::SaveTile,this), std::bind(&MapTool::LoadTile, this));

	//if (FAILED(LineManager->LoadFile(L"Data/Stage1/Stage1Line.dat")))
	//{
	//	MessageBox(g_hWnd, TEXT("MapTool LineManager LoadFile Failed."), TEXT("실패"), MB_OK);
	//	return E_FAIL;
	//}

	InitTile();

	if (FAILED(InitImage()))
	{
		MessageBox(g_hWnd, TEXT("MapTool InitImage Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}

	if (FAILED(InitObject()))
	{
		MessageBox(g_hWnd, TEXT("MapTool InitObject Failed."), TEXT("실패"), MB_OK);
		return E_FAIL;
	}



	return S_OK;
}



void MapTool::Update()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F5))
	{
		SceneManager::GetInstance()->ChangeScene("HY", "로딩_1");
		return;
	}

	ObjectManager->Update();
	CollisionManager->Update();
	ScrollManager->Update();
	LineManager->Update();

	string tileName = ImGuiManager::GetInstance()->GetTileName();
	if (tileName != "" && 
		(KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON) ||
			KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE)))
	{
		//기본을 무조건 32x32로 한다는 뜻
		const float TILEX = 32 * (ScrollManager::GetInstance()->GetScale() / 2);
		const float TILEY = 32 * (ScrollManager::GetInstance()->GetScale() / 2);

		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

		int CurrentTileX = (g_ptMouse.x - Scroll.x) / TILEX;
		int CurrentTileY = (g_ptMouse.y - Scroll.y) / TILEY;
		int OffsetX = TILEX / 2;
		int OffsetY = TILEX / 2;

		FPOINT TileIndex = ImGuiManager::GetInstance()->GetselectedTile();
		string key = to_string(CurrentTileX) + to_string(CurrentTileY);
		if (ImGuiManager::GetInstance()->IsTileEraser())
		{
			CurrentTiles.erase(key);
			CurrentTiles2.erase(key);
		}			
		else
		{
			auto iter = TileList.find(tileName);
		
			if (iter != TileList.end())
			{
				if (CurrentTiles.find(key) == CurrentTiles.end())
				{
					CurrentTiles[key].image = iter->second;
					CurrentTiles[key].tileX = (CurrentTileX * TILEX) + OffsetX;
					CurrentTiles[key].tileY = (CurrentTileY * TILEY) + OffsetY;
					CurrentTiles[key].frameX = TileIndex.x;
					CurrentTiles[key].frameY = iter->second->GetMaxFrameY() - TileIndex.y - 1;
					CurrentTiles[key].ImageName = tileName;
				}
				else
				{
					CurrentTiles2[key].image = iter->second;
					CurrentTiles2[key].tileX = (CurrentTileX * TILEX) + OffsetX;
					CurrentTiles2[key].tileY = (CurrentTileY * TILEY) + OffsetY;
					CurrentTiles2[key].frameX = TileIndex.x;
					CurrentTiles2[key].frameY = iter->second->GetMaxFrameY() - TileIndex.y - 1;
					CurrentTiles2[key].ImageName = tileName;					
				}
				
			}
		}
	}

	ImGuiManager::GetInstance()->Update();

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F1))
		SceneManager::GetInstance()->ChangeScene("Test", "로딩_1");
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F3))
		SceneManager::GetInstance()->ChangeScene("Stage1", "로딩_1");
}

void MapTool::Render(HDC hdc)
{
	RenderManager->RenderBackGround(hdc);

	for (auto& iter : CurrentTiles)
	{
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		iter.second.image->FrameRender(hdc, iter.second.tileX + Scroll.x, iter.second.tileY + Scroll.y, iter.second.frameX, iter.second.frameY, false, true, ScrollManager::GetInstance()->GetScale() / 2);
	}
	for (auto& iter : CurrentTiles2)
	{
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		iter.second.image->FrameRender(hdc, iter.second.tileX + Scroll.x, iter.second.tileY + Scroll.y, iter.second.frameX, iter.second.frameY, false, true, ScrollManager::GetInstance()->GetScale() / 2);
	}


	RenderManager->RenderNonAlphaBlend(hdc);
	RenderManager->RenderAlphaBlend(hdc);
	RenderManager->RenderUI(hdc);

	CollisionManager->Render(hdc);

	LineManager->Render(hdc);

	

	ImGuiManager::GetInstance()->APIRender(hdc);

	ImGuiManager::GetInstance()->Render();
}

HRESULT MapTool::InitImage()
{
	ImageManager::GetInstance()->AddImage("black", L"Image/Background/blackBg.bmp", 1920, 1080, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("rocket", L"Image/rocket.bmp", 52, 64, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("headhunter", L"Image/HeadHunter/headhunter_idle_init.bmp", 25, 50, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("TestPlayer", L"Image/TestPlayer.bmp", 25, 35, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("Grunt", L"Image/Enemy/Grunt/Grunt.bmp", 30, 36, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("Pomp", L"Image/Enemy/Pomp/Pomp.bmp", 33, 42, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("Gangster", L"Image/Enemy/Gangster/Gangster.bmp", 49, 50, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("spr_beer_bottle_3_0", L"Image/Bottle/spr_beer_bottle_3_0.bmp", 48, 48, 2, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("spr_beer_bottle_4_0", L"Image/Bottle/spr_beer_bottle_4_0.bmp", 48, 48, 2, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("Player", L"Image/zero_maptool.bmp", 29, 35, 1, 1, true, RGB(255, 0, 255));
	return S_OK;
}

HRESULT MapTool::InitObject()
{
	Background* background = new Background();
	background->Init("black", 0.f);
	ObjectManager->AddGameObject(EObjectType::GameObject, background);

	return S_OK;
}

void MapTool::InitTile()
{
	vector<string> Tiles = GetFileNames("Image/Tile/*.bmp");

	if (Tiles.empty())
		return;

	for (int i = 0; i < Tiles.size(); ++i)
	{
		int dotPos = Tiles[i].find_last_of('.');
		string nameOnly = dotPos != string::npos ? Tiles[i].substr(0, dotPos) : Tiles[i];

		wstring wsPath = L"Image/Tile/";
		wsPath += wstring(Tiles[i].begin(), Tiles[i].end());

		Image* image = ImageManager::GetInstance()->AddImage(nameOnly, wsPath.c_str(),true, RGB(255, 0, 255),32,32);

		TileList.insert({ nameOnly,image });
	}
}

void MapTool::SaveTile()
{
	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetSaveInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetSaveFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);

		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"SaveTile Failed.", TEXT("경고"), MB_OK);
			return;
		}

		DWORD dwByte = 0;

		for (auto& iter : CurrentTiles)
		{
			FTileData td;
			ZeroMemory(&td, sizeof(td));
			td.tileX = iter.second.tileX;
			td.tileY = iter.second.tileY;
			td.frameX = iter.second.frameX;
			td.frameY = iter.second.frameY;
			td.KeySize = iter.first.size();
			td.imageNameSize = iter.second.ImageName.size();
			 
			WriteFile(hFile, &td.KeySize, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, &td.imageNameSize, sizeof(int), &dwByte, NULL);			
			WriteFile(hFile, iter.first.c_str(), td.KeySize, &dwByte, NULL);
			WriteFile(hFile, iter.second.ImageName.c_str(), td.imageNameSize, &dwByte, NULL);
			WriteFile(hFile, &td.tileX, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, &td.tileY, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, &td.frameX, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, &td.frameY, sizeof(int), &dwByte, NULL);
		}
		for (auto& iter : CurrentTiles2)
		{
			FTileData td;
			ZeroMemory(&td, sizeof(td));
			td.tileX = iter.second.tileX;
			td.tileY = iter.second.tileY;
			td.frameX = iter.second.frameX;
			td.frameY = iter.second.frameY;
			td.KeySize = iter.first.size();
			td.imageNameSize = iter.second.ImageName.size();

			WriteFile(hFile, &td.KeySize, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, &td.imageNameSize, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, iter.first.c_str(), td.KeySize, &dwByte, NULL);
			WriteFile(hFile, iter.second.ImageName.c_str(), td.imageNameSize, &dwByte, NULL);
			WriteFile(hFile, &td.tileX, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, &td.tileY, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, &td.frameX, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, &td.frameY, sizeof(int), &dwByte, NULL);
		}



		MessageBox(g_hWnd, L"타일 저장 성공", TEXT("성공"), MB_OK);

		CloseHandle(hFile);
	}
}

void MapTool::LoadTile()
{
	CurrentTiles.clear();
	CurrentTiles2.clear();

	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetLoadInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);

		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"LoadTile Failed.", TEXT("경고"), MB_OK);
			return;
		}

		DWORD dwByte = 0;

		while (true)
		{
			FTileData td;
			ZeroMemory(&td, sizeof(td));

			ReadFile(hFile, &td.KeySize, sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &td.imageNameSize, sizeof(int), &dwByte, NULL);

			td.key = new char[td.KeySize + 1];
			td.imageName = new char[td.imageNameSize + 1];

			ReadFile(hFile, td.key, td.KeySize, &dwByte, NULL);
			ReadFile(hFile, td.imageName, td.imageNameSize, &dwByte, NULL);
			ReadFile(hFile, &td.tileX, sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &td.tileY, sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &td.frameX, sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &td.frameY, sizeof(int), &dwByte, NULL);

			td.key[td.KeySize] = '\0';
			td.imageName[td.imageNameSize] = '\0';

			string KeyName = td.key;
			string ImageName = td.imageName;

			delete[] td.key;
			delete[] td.imageName;

			if (dwByte == 0)
				break;

			Image* image = ImageManager::GetInstance()->FindImage(ImageName);

			if (CurrentTiles.find(KeyName) == CurrentTiles.end())
			{
				CurrentTiles[KeyName].image = image;
				CurrentTiles[KeyName].tileX = td.tileX;
				CurrentTiles[KeyName].tileY = td.tileY;
				CurrentTiles[KeyName].frameX = td.frameX;
				CurrentTiles[KeyName].frameY = td.frameY;
				CurrentTiles[KeyName].ImageName = ImageName;
			}
			else
			{
				CurrentTiles2[KeyName].image = image;
				CurrentTiles2[KeyName].tileX = td.tileX;
				CurrentTiles2[KeyName].tileY = td.tileY;
				CurrentTiles2[KeyName].frameX = td.frameX;
				CurrentTiles2[KeyName].frameY = td.frameY;
				CurrentTiles2[KeyName].ImageName = ImageName;
			}

		}

		MessageBox(g_hWnd, L"타일 불러오기 성공", TEXT("성공"), MB_OK);

		CloseHandle(hFile);
	}
}


void MapTool::Release()
{
	ImGuiManager::GetInstance()->Reset();

	CurrentTiles.clear();
	CurrentTiles2.clear();

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

	ObjectManager = nullptr;
	CollisionManager = nullptr;
	RenderManager = nullptr;
	ScrollManager = nullptr;
	LineManager = nullptr;

	
}