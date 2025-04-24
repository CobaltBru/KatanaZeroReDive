#pragma once
#include "GameObject.h"

class Image;
class PickUpUI : public GameObject
{
public:
	PickUpUI();
	~PickUpUI() = default;
	HRESULT Init(string InImageKey, FPOINT* InPos,FPOINT InOffset, bool InFlip);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void SetImage(string InImageKey);
	void Start();
private:
	Image* Image;
	FPOINT* TargetPos;
	FPOINT Offset;
	float CurrentTime;
	float FlashTime;
	bool IsShow;
};

