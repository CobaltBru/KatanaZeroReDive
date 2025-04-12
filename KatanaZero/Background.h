#pragma once
#include "GameObject.h"
class Background : public GameObject
{
public:
	Background();
	virtual ~Background() = default;

	HRESULT Init(string InImageKey);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	Image* Image;
};

