#pragma once
#include "Singleton.h"
#include "config.h"
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
	void SaveLine();
	
	void LoadFont();
	void LoadLine();	
};

