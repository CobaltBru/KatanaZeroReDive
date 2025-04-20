#pragma once
#include "Singleton.h"
#include "config.h"
#include <unordered_map>

class GameObject;
class ImGuiManager : public Singleton<ImGuiManager>
{
public:
	void Init();
	void Update();
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

private:
	vector<string> GetFileNames(const string& InFolderPath);

	void InitBackground();
	void HelpMarker(const char* desc);
	void CreateBackground(int Index);
	bool CheckBakcground(int Index);

	void SaveLine();
	void SaveBackGround();

	void LoadFont();
	void LoadLine();
	void LoadBackGround();



private:
	unordered_map<const char*, string> BackGroundMap;
	const char** BackgroundList;
	GameObject* BackgroundObj;
	int CurrentBackgroundIndex;
};

