#pragma once
#include "config.h"
#include "Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* timer;
	wchar_t szText[128];
	float SlowPercent;
	float CurrentSlowPercent;
	float Interp;
public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	// ���� Ÿ�ӵ�Ÿ ���������� false ��������~
	float GetDeltaTime(bool CanSlowSystem = true);

	void SetSlow(float InSlowPercent, float InInterp = 0.5f) { 
		SlowPercent = InSlowPercent;
		Interp = InInterp;
	}
};

