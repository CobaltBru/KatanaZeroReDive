#include "ScreenEffectManager.h"
#include "TimerManager.h"
#include <math.h>
#include "SnapShotManager.h"
#include "Image.h"
#include "GPImage.h"
#include "ImageManager.h"
#include "ScrollManager.h"
#include "SceneManager.h"


void ScreenEffectManager::Init()
{
	noise = new GPImage();
	noise->AddImage(L"Image/noise2.bmp");
}

void ScreenEffectManager::Release()
{
	if (noise)
	{
		delete noise;
		noise = nullptr;
	}
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
	if (isFinishReplay)
	{
		ScrollDistortion(hdc, frequency * 2.0f, distortionSpeed * 10.0f, distortionForce * 0.3f);
	}
	else
	{
		Distortion(hdc, frequency, distortionSpeed * 0.8f, distortionForce);
	}
}

void ScreenEffectManager::StopDistortion()
{
	if (!isDistortion) return;
	isFinishReplay = true;
	ScrollManager::GetInstance()->CameraShake(20.0f, scrollDuration);
}

void ScreenEffectManager::Distortion(HDC hdc, float frequency, float distortionSpeed, float distortionForce)
{
	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	for (int x = 0; x < WINSIZE_X; x += 2)
	{
		int offset = (int)(sinf(x * frequency + elapsedTime * distortionSpeed) * distortionForce);

		BitBlt(hdc,
			x, offset, 2, WINSIZE_Y,
			hdc,
			x, 0,
			SRCCOPY);
	}
	for (int y = 0; y < WINSIZE_Y; y += 2)
	{
		int offset = (int)(sinf(y * frequency + elapsedTime * distortionSpeed) * distortionForce * 1.5f);

		BitBlt(hdc,
			offset, y, WINSIZE_X, 2,
			hdc,
			0, y,
			SRCCOPY);
	}
}

void ScreenEffectManager::ScrollDistortion(HDC hdc, float frequency, float distortionSpeed, float distortionForce)
{
	Distortion(hdc, frequency, distortionSpeed * 10.0f, distortionForce);
	scrollTimer += TimerManager::GetInstance()->GetDeltaTime();
	scrollOffset += scrollSpeed * TimerManager::GetInstance()->GetDeltaTime();
	int offset = (int)scrollOffset % WINSIZE_Y;
	BitBlt(hdc,
		0, 0,
		WINSIZE_X, WINSIZE_Y - offset,
		hdc,
		0, offset,
		SRCCOPY);
	BitBlt(hdc,
		0, WINSIZE_Y - offset,
		WINSIZE_X, offset,
		hdc,
		0, 0,
		SRCCOPY);
	if (scrollTimer >= 0.2f)
	{
		scrollSpeed * scrollacceleration;
	}
	if (scrollTimer >= scrollDuration)
	{
		isDistortion = false;
		isFinishReplay = false;
		scrollTimer = 0.0f;
		scrollOffset = 0.0f;
		SceneManager::GetInstance()->RestartCurrentScene();
	}
}
