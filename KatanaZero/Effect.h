#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"

class GPImage;
class Effect : public GameObject
{
private:
	GPImage* fxImage;
	int currFrameX;
	int currFrameY;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void MakeSnapShot(void* out) override;
};

