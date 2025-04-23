#pragma once
#include "GameObject.h"

class Image;
class PickUp : public GameObject
{
public:
	PickUp();
	~PickUp() = default;
	virtual	HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	Image* Image;
	int FrameX;
};

