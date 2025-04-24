#pragma once
#include "GameObject.h"
class Image;
class WorldBackground : public GameObject
{
public:
	virtual	HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend);
	virtual void Update();
	virtual void Render(HDC hdc);

protected:
	Image* Image;
};

