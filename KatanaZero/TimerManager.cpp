#include "TimerManager.h"
#include "Timer.h"

void TimerManager::Init()
{
	timer = new Timer();
	timer->Init();
	SlowPercent = 1.f;
	CurrentSlowPercent = 1.f;
	Interp = 0.5f;
}

void TimerManager::Release()
{
	timer->Release();
	delete timer;
	timer = nullptr;

	ReleaseInstance();
}

void TimerManager::Update()
{
	if (timer)
	{
		timer->Tick();

		if (SlowPercent != CurrentSlowPercent)
		{
			float t = timer->GetDeltaTime() / Interp; //몇초 동안 보간할건지 

			if (t > 1.f)
				t = 1.f;

			CurrentSlowPercent += (SlowPercent - CurrentSlowPercent) * t;

			if (abs(SlowPercent - CurrentSlowPercent) <= 0.01f)
				CurrentSlowPercent = SlowPercent;			
		}		
	}		
}

void TimerManager::Render(HDC hdc)
{
	if (timer)
	{
		wsprintf(szText, TEXT("FPS : %d"), timer->GetFPS());
		TextOut(hdc, WINSIZE_X - 130, 20, szText, wcslen(szText));
	}
}

float TimerManager::GetDeltaTime(bool CanSlowSystem)
{
	if (timer)
	{
		if (CanSlowSystem)
		{
			return timer->GetDeltaTime() * CurrentSlowPercent;
		}
		else
			return timer->GetDeltaTime();
	}
		
	return 0.0f;
}
