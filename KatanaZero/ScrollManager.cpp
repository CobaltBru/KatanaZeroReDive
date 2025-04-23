#include "ScrollManager.h"

ScrollManager::ScrollManager()
	:bFocus(false), Speed(0.f), Intensity(0.f), CurrentTime(0.f), ShakeSeconds(0.f), Scale(1.f)
{
	ZeroMemory(&Scroll, sizeof(FPOINT));
	ZeroMemory(&ScrollOffset, sizeof(FPOINT));
	ZeroMemory(&ShakeOffset, sizeof(FPOINT));
}

void ScrollManager::Init()
{
	Speed = 300.f;
	ScaleUpdate();
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

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_CONTROL))
		bFocus = !bFocus;

	if (bFocus)
		return;

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
		ScrollOffset.y += Speed * TimerManager::GetInstance()->GetDeltaTime();
	else if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
		ScrollOffset.y -= Speed * TimerManager::GetInstance()->GetDeltaTime();
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
		ScrollOffset.x += Speed * TimerManager::GetInstance()->GetDeltaTime();
	else if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
		ScrollOffset.x -= Speed * TimerManager::GetInstance()->GetDeltaTime();
}

void ScrollManager::Release()
{
	ReleaseInstance();
}

void ScrollManager::ScaleUpdate()
{
	const int BASE_WIDTH = 640;
	const int BASE_HEIGHT = 360;

	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);

	float ScaleX = (float)clientRect.right / BASE_WIDTH;
	float ScaleY = (float)clientRect.bottom / BASE_HEIGHT;
	Scale = min(ScaleX, ScaleY);
	//Scale = 1.f;
}
