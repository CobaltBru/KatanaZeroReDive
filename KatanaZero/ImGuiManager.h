#pragma once
#include "Singleton.h"
#include "config.h"
#include <unordered_map>
#include <functional>

class GameObject;
class LineManager;
class ObjectManager;
class ScrollManager;
class ImGuiManager : public Singleton<ImGuiManager>
{
public:
	ImGuiManager();
	void Init();
	void Init(LineManager* InLineManager, ObjectManager* InObjectManager, ScrollManager* InScrollManager, function<void()> InSaveTileCallBack, function<void()> InLoadTileCallBack);
	void Update();
	void APIRender(HDC hdc);
	void Render();
	void Release();
	
	void ShowGui();
	void Editor();
	void MapTool();
	void Line();
	void Tile();
	void Object();

	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();

	void Reset();

	string GetTileName() { 		
		if (Tile_current == -1 || TileList.empty())
			return "";
		
		return TileList[Tile_current];
	}

	FPOINT GetselectedTile() const { return selectedTile; }
	bool IsTileEraser() const { return TileEraser; }
private:
	//vector<string> GetFileNames(const string& InFolderPath);

	void CreatePlayerObject();
	void HelpMarker(const char* desc);

	void InitBackground();
	void CreateBackground(int Index);
	void DestroyBackGround();
	void DestroyAllBackGround();

	void InitTile();

	void ObjectTap();
	void ObjectUpdate();
	void WorldObjectUpdate();
	void DestroySelectObject();
	void DestroyAllObject();

	void Floor();
	void DestroyAllFloor();
	void DrawFloor(HDC hdc);

	void DrawTile();

	//OPENFILENAME GetSaveInfo(TCHAR* lpstrFile);
	//OPENFILENAME GetLoadInfo(TCHAR* lpstrFile);

	void SaveLine();
	void SaveBackGround();
	void SaveObject();
	void SaveFloor();
	void SaveTile();

	void LoadFont();
	void LoadLine();
	void LoadBackGround();
	void LoadObject();
	void LoadFloor();
	void LoadTile();


	bool LoadTextureFromMemory(const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	bool LoadTextureFromFile(const char* file_name, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	
private:
	unordered_map<const char*, string> BackGroundMap;
	const char** BackgroundList;

	GameObject* PlayerStartPoint;
	GameObject* selectObject;

	vector<const char*> WorldObjectName;
	vector<const char*> BackGroundName;
	vector<const char*> FloorName;

	GameObject* PlayerObject;

	LineManager* lineManager;
	ObjectManager* objectManager;
	ScrollManager* scrollManager;

	vector<const char*> TileList;
	unordered_map<const char*, pair<ID3D11ShaderResourceView*, pair<int, int>>> TileTextures;
	
	int Tile_current = -1;
	FPOINT selectedTile;
	bool TileEraser = false;
	function<void()> SaveTileCallBack;
	function<void()> LoadTileCallBack;
};





//// Get the ImGui cursor in screen space
//ImVec2 display_p0 = ImGui::GetCursorScreenPos();
//// Get size of the tileset display
//ImVec2 display_sz = ImGui::GetContentRegionAvail();
//display_sz.x = (display_sz.x < 50.0f ? 50.0f : display_sz.x);
//display_sz.y = (display_sz.y < 50.0f ? 50.0f : (display_sz.y > 320.0f ? 320.0f : display_sz.y));
//ImVec2 display_p1 = ImVec2(display_p0.x + display_sz.x, display_p0.y + display_sz.y);
//
//// Draw border and background color
//ImGuiIO& io = ImGui::GetIO();
//ImDrawList* draw_list = ImGui::GetWindowDrawList();
//draw_list->AddRectFilled(display_p0, display_p1, IM_COL32(50, 50, 50, 255));
//draw_list->AddRect(display_p0, display_p1, IM_COL32(255, 255, 255, 255));