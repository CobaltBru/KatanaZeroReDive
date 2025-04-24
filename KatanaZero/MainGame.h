#pragma once
#include "GameObject.h"
#include "GPImage.h"


class Timer;
class Image;
class EnemyManager;
class TilemapTool;
class MainGame : public GameObject
{
public:
	MainGame();
	~MainGame();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

private:
	HRESULT InitSound();
	HRESULT InitImage();
	void InitBackground();
private:
	HDC hdc;
	PAINTSTRUCT ps;
	wchar_t szText[128];

	Image* backBuffer;

	TilemapTool* tilemapTool;

	Timer* timer;

	GPImage testDraw;
	float tmpTimer;
	int frameIdx = 0;

	HFONT hFont;
	HFONT hOldFont;
};

