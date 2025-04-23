#include "Tile.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"

HRESULT Tile::Init(LPCWSTR InLoadPath)
{
	HANDLE hFile = CreateFile(
		InLoadPath, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"Tile Failed.", TEXT("경고"), MB_OK);
		return E_FAIL;
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

		CurrentTiles[KeyName].image = image;
		CurrentTiles[KeyName].tileX = td.tileX;
		CurrentTiles[KeyName].tileY = td.tileY;
		CurrentTiles[KeyName].frameX = td.frameX;
		CurrentTiles[KeyName].frameY = td.frameY;
		CurrentTiles[KeyName].ImageName = ImageName;
	}

	CloseHandle(hFile);
	
	return S_OK;
}

void Tile::Update()
{
	RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::BackGround, this);
}

void Tile::Render(HDC hdc)
{
	for (auto& iter : CurrentTiles)
	{
		//컬링을 해보자.
		const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
		const float screenLeft = (iter.second.tileX - (16 * ScrollManager::GetInstance()->GetScale())) + Scroll.x;
		const float screenRight = (iter.second.tileX + (16 * ScrollManager::GetInstance()->GetScale())) + Scroll.x;
		const float screenTop = (iter.second.tileY - (16 * ScrollManager::GetInstance()->GetScale())) + Scroll.y;
		const float screenBottom = (iter.second.tileY + (16 * ScrollManager::GetInstance()->GetScale())) + Scroll.y;
		if (screenRight >= 0.f && screenLeft <= WINSIZE_X && screenBottom >= 0.f && screenTop <= WINSIZE_Y)
			iter.second.image->FrameRender(hdc, iter.second.tileX + Scroll.x, iter.second.tileY + Scroll.y, iter.second.frameX, iter.second.frameY, false, true, ScrollManager::GetInstance()->GetScale());		
	}
}

void Tile::Release()
{
}
