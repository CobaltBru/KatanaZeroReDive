#include "ScrollManager.h"

void ScrollManager::Init()
{
	ZeroMemory(&Scroll, sizeof(FPOINT));
}

void ScrollManager::Release()
{
	ReleaseInstance();
}
 