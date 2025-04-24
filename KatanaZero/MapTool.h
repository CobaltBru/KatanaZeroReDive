#pragma once
#include "Scene.h"
#include <unordered_map>



class ObjectManager;
class RenderManager;
class CollisionManager;
class ScrollManager;
class LineManager;
class Image;

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

class MapTool : public Scene
{
public:
	MapTool();
	virtual ~MapTool() = default;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;


protected:
	virtual HRESULT InitImage();
	virtual HRESULT InitObject();

private:
	void InitTile();
	void SaveTile();
	void LoadTile();
private:
	ObjectManager* ObjectManager;
	RenderManager* RenderManager;
	CollisionManager* CollisionManager;
	ScrollManager* ScrollManager;
	LineManager* LineManager;

	unordered_map<string, Image*> TileList;
	unordered_map<string,FTileInfo> CurrentTiles;
	unordered_map<string, FTileInfo> CurrentTiles2;
};

