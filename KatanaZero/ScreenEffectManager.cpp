#include "ScreenEffectManager.h"
#include "TimerManager.h"
#include <math.h>
#include "SnapShotManager.h"
#include "Image.h"
#include "GPImage.h"
#include "ImageManager.h"


void ScreenEffectManager::Init()
{
	noise->AddImage(L"Image/noise1.bmp");
}

void ScreenEffectManager::Release()
{
	ReleaseInstance();
}

void ScreenEffectManager::StartDistortion()
{
	if (isDistortion) return;
	isDistortion = true;
	elapsedTime = 0.0f;
}

void ScreenEffectManager::RenderDistortion(HDC hdc)
{
	if (!isDistortion) return;
	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	for (int x = 0; x < WINSIZE_X; x += 2)
	{
		int offset = (int)(sinf(x * 0.05f + elapsedTime * distortionSpeed) * distortionForce);

		BitBlt(hdc,
			x, offset, 2, WINSIZE_Y,
			hdc,
			x, 0,
			SRCCOPY);
	}
	for (int y = 0; y < WINSIZE_Y; y += 2)
	{
		int offset = (int)(sinf(y * 0.05f + elapsedTime * distortionSpeed) * distortionForce * 1.5f);

		BitBlt(hdc,
			offset, y, WINSIZE_X, 2,
			hdc,
			0, y,
			SRCCOPY);
	}
	
	//distortionSpeed -= 0.01f;
}

void ScreenEffectManager::RenderNoise(HDC hdc)
{
	Gdiplus::Graphics graphics(hdc);
	noise->Render(&graphics, { 0, 0 }, false, 0.5f);
}

void ScreenEffectManager::StopDistortion()
{
	//if (!isDistortion) return;
	isDistortion = false;
}
