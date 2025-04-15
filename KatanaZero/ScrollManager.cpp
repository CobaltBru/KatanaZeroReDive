#include "ScrollManager.h"

void ScrollManager::Init()
{
	ZeroMemory(&Scroll, sizeof(FPOINT));
	bFocus = false;
}

void ScrollManager::Release()
{
	ReleaseInstance();
}
 