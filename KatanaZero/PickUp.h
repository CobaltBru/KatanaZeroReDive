#pragma once
#include "GameObject.h"

class GPImage;
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

	void SetVisible(bool InVisible) { IsVisible = InVisible; }
	void Shoot(float InAngle, float InSpeed);

private:
	void Collision();
	void CreateBrokeParticle();
private:
	GPImage* gpImage;
	Image* Image;
	int FrameX;
	bool IsVisible;
	float Angle;
	float SpinAlngle;
	float Speed;
};

