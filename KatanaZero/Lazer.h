#pragma once
#include "GameObject.h"
class Lazer : public GameObject
{
public:
	Lazer();
	virtual ~Lazer() = default;
	HRESULT Init(string InImageKey);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
};

