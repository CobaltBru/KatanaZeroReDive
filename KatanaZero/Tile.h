#pragma once
#include "GameObject.h"
#include <unordered_map>

struct FTileInfo
{
	Image* image;
	int tileX;
	int tileY;
	int frameX;
	int frameY;
	string ImageName;
};
struct FTileData
{
	int imageNameSize;
	int KeySize;
	char* imageName;
	char* key;
	int tileX;
	int tileY;
	int frameX;
	int frameY;
};

class Tile : public GameObject
{
public:
	Tile() = default;
	~Tile()= default;

	HRESULT Init(LPCWSTR InLoadPath);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	unordered_map<string, FTileInfo> CurrentTiles;
	unordered_map<string, FTileInfo> CurrentTiles2;
};

