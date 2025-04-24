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

	string GetImageKey() const { return ImageKey; }

	void SetVisible(bool InVisible) { IsVisible = InVisible; }	

	void Shoot(FPOINT InPos,float InAngle, float InSpeed);
	
private:
	void Collision();
	void CreateBrokeParticle();
private:
	GPImage* gpImage;
	Image* Image;
	bool IsVisible;
	float Angle;
	float SpinAlngle;
	float Speed;
	string ImageKey;
};

