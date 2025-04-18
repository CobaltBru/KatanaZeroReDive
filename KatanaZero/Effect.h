#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"
#include "Image.h"
#include "ImageManager.h"

class GPImage;
class Effect : public GameObject
{
private:
	GPImage* fxImage;
	FPOINT pos;
	int currFrameX;
	int currFrameY;
	int maxFrameX;
	int maxFrameY;
	float angle;
	float frameTimer;
	bool bFlip;
	bool bActive;
	float alpha;

	//sour 테스트용
	int offset = 0;
	float offsetTimer = 0.0f;

	void UpdateFrame();
public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(const wchar_t* AType, int maxframeX, int maxframeY);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void MakeSnapShot(void* out) override;
	void ApplySnapShot(const EffectSnapShot& fxSnapShot);
	void Activefx(FPOINT pos, float angle ,bool bFlip);
	Effect() {};
	virtual ~Effect() {};
};

struct RemainEffect
{
	GPImage* image;
	FPOINT pos;
	float alpha;
	float lifetime;
	bool bFlip;
	int frame;
};



