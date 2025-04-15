#include "ScreenEffectManager.h"
#include "TimerManager.h"
#include <math.h>
#include "SnapShotManager.h"


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
	
	//distortionSpeed -= 0.01f;
}

void ScreenEffectManager::RenderGlitch(HDC hdc)
{
	glitchOffset += 20;
	for (int x = 0; x < WINSIZE_X; x += 2)
	{
		BitBlt(hdc,
			x, glitchOffset, 2, WINSIZE_Y,
			hdc,
			x, 0,
			SRCCOPY);
	}
}

void ScreenEffectManager::StopDistortion()
{
	//if (!isDistortion) return;
	isDistortion = false;
}
