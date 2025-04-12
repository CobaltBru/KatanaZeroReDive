#pragma once
#include "GameObject.h"
#include "GPImage.h"

class Timer;
class Image;
class EnemyManager;
class TilemapTool;
class MainGame : public GameObject
{
private:
	HDC hdc;
	PAINTSTRUCT ps;
	wchar_t szText[128];

	Image* backBuffer;

	TilemapTool* tilemapTool;

	Timer* timer;

	GPImage testDraw;
	int tmpTimer;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();
};

