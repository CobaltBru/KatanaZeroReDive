#include "ScrollManager.h"

ScrollManager::ScrollManager()
	:bFocus (false), Speed(0.f)
{
	ZeroMemory(&Scroll, sizeof(FPOINT));	
}

void ScrollManager::Init()
{
	Speed = 300.f;
}

void ScrollManager::Update()
{
	if (bFocus)
		return;

	if (KeyManager::GetInstance()->IsStayKeyDown('W'))
		Scroll.y += Speed * TimerManager::GetInstance()->GetDeltaTime();
	else if (KeyManager::GetInstance()->IsStayKeyDown('S'))
		Scroll.y -= Speed * TimerManager::GetInstance()->GetDeltaTime();
	if (KeyManager::GetInstance()->IsStayKeyDown('A'))
		Scroll.x += Speed * TimerManager::GetInstance()->GetDeltaTime();
	else if (KeyManager::GetInstance()->IsStayKeyDown('D'))
		Scroll.x -= Speed * TimerManager::GetInstance()->GetDeltaTime();
}

void ScrollManager::Release()
{
	ReleaseInstance();
}
 