#pragma once
#include "Singleton.h"
#include "config.h"
#include <unordered_map>

class GameObject;
class ImGuiManager : public Singleton<ImGuiManager>
{
public:
	ImGuiManager();
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

	void Reset();

private:
	vector<string> GetFileNames(const string& InFolderPath);

	void HelpMarker(const char* desc);

	void InitBackground();
	void CreateBackground(int Index);
	void DestroyBackGround();

	void ObjectTap();
	void ObjectUpdate();
	void WorldObjectUpdate();
	void DestroySelectObject();
	void DestroyAllObject();


	OPENFILENAME GetSaveInfo(TCHAR* lpstrFile);
	OPENFILENAME GetLoadInfo(TCHAR* lpstrFile);

	void SaveLine();
	void SaveBackGround();
	void SaveObject();

	void LoadFont();
	void LoadLine();
	void LoadBackGround();
	void LoadObject();

private:
	unordered_map<const char*, string> BackGroundMap;
	const char** BackgroundList;

	GameObject* PlayerStartPoint;
	GameObject* SelectObject;

	vector<const char*> WorldObjectName;
	vector<const char*> BackGroundName;

};

