#pragma once
#include "GameObject.h"
class Background : public GameObject
{
public:
	Background();
	virtual ~Background() = default;

	HRESULT Init(string InImageKey, float InScrollPercent = 1.f, float InScale = 1.f);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	string GetImageName() const { return ImageName; }
	float GetScrollPercent() const { return ScrollPercent; }

	void SetScrollPercent(float InScrollPercent) { ScrollPercent = InScrollPercent; }
private:
	Image* Image;
	float ScrollPercent;
	float Scale;
	string ImageName;
};

