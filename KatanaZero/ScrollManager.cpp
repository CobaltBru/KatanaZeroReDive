#include "ScrollManager.h"

ScrollManager::ScrollManager()
	:bFocus(false), Speed(0.f), Intensity(0.f), CurrentTime(0.f), ShakeSeconds(0.f)
{
	ZeroMemory(&Scroll, sizeof(FPOINT));
	ZeroMemory(&ScrollOffset, sizeof(FPOINT));
	ZeroMemory(&ShakeOffset, sizeof(FPOINT));
}

void ScrollManager::Init()
{
	Speed = 300.f;
}

void ScrollManager::Update()
{
	if (ShakeSeconds)
	{
		CurrentTime += TimerManager::GetInstance()->GetDeltaTime();

		if (ShakeSeconds > CurrentTime)
		{
			uniform_int_distribution<int> Range(-Intensity, Intensity);

			ShakeOffset.x = Range(dre);
			ShakeOffset.y = Range(dre);
		}
		else
		{
			ShakeOffset.x = 0.f;
			ShakeOffset.y = 0.f;
			ShakeSeconds = 0.f;
			CurrentTime = 0.f;
		}
	}

	Scroll.x = ScrollOffset.x + ShakeOffset.x;
	Scroll.y = ScrollOffset.y + ShakeOffset.y;

	if (bFocus)
		return;

	if (KeyManager::GetInstance()->IsStayKeyDown('W'))
		ScrollOffset.y += Speed * TimerManager::GetInstance()->GetDeltaTime();
	else if (KeyManager::GetInstance()->IsStayKeyDown('S'))
		ScrollOffset.y -= Speed * TimerManager::GetInstance()->GetDeltaTime();
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
		ScrollOffset.x += Speed * TimerManager::GetInstance()->GetDeltaTime();
	else if (KeyManager::GetInstance()->IsStayKeyDown('D'))
		ScrollOffset.x -= Speed * TimerManager::GetInstance()->GetDeltaTime();
}

void ScrollManager::Release()
{
	ReleaseInstance();
}
