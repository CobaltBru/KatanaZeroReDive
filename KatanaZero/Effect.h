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
	FPOINT start;
	FPOINT end;
	FPOINT pos;
	int currFrameX;
	int currFrameY;
	int maxFrameX;
	int maxFrameY;
	float angle;
	float frameTimer;
	float speed;
	bool bFlip;
	bool bActive;
	bool bMove;
	float alpha;

	//sour 테스트용
	int offset = 0;
	float offsetTimer = 0.0f;

	void UpdateFrame();
	void Move();
public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(const wchar_t* AType, int maxframeX, int maxframeY, bool bMove = false);
	virtual HRESULT Init(const wchar_t* AType, int maxframeX, int maxframeY, FPOINT start, FPOINT end, float speed, bool bMove = false);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void MakeSnapShot(void* out) override;
	void ApplySnapShot(const EffectSnapShot& fxSnapShot);
	void Activefx(FPOINT pos, float angle ,bool bFlip = false);
	void Activefx(FPOINT pos, FPOINT dest, float speed, bool bFlip = false);
	inline bool IsActive() { return bActive; }

	Effect(const Effect& other);
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



