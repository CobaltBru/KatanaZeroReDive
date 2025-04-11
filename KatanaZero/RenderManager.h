#pragma once
#include "Singleton.h"
#include "config.h"

class RenderManager : public Singleton<RenderManager>
{
public:
	void Init();
	void Render(HDC hdc);
	void Release();
};

