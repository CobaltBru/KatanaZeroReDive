#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"
#include "Image.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "ScrollManager.h"
#include "GPImage.h"

class Effect : public GameObject
{
private:
	int id;
	static int nextID;
	GPImage* fxImage;
	GameObject* owner;
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
	float frameduration;
	string currAnimKey;

	//sour 테스트용
	int offset = 0;
	float offsetTimer = 0.0f;

	void UpdateFrame();
	void Move();
public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(string key, FPOINT start, FPOINT end, float speed, float frameduration = 0.1f, bool bFlip = false, bool bMove = false);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void MakeSnapShot(void* out) override;
	void ApplySnapShot(const EffectSnapShot& fxSnapShot);
	void Activefx(FPOINT pos, float angle ,bool bFlip = false);
	void Activefx(FPOINT pos, float angle, float speed, bool bFlip = false);
	void Activefx(FPOINT pos, float angle, GameObject* owner, bool bFlip = false);
	void Activefx(FPOINT pos, FPOINT dest, float speed, bool bFlip = false);
	void Activefx(FPOINT pos, float angle, float speed, float scale, bool bFlip = false);
	inline bool IsActive() { return bActive; }
	inline int GetID() { return id; }

	Effect(const Effect& other);
	Effect() : id(nextID++) {};
	virtual ~Effect() {};
};

struct RemainEffect
{
	GPImage* image;
	FPOINT pos;
	FPOINT velocity;
	float alpha;
	float totalLife;
	float lifetime;
	bool bFlip;
	int frame;
};

struct BackgroundBloodfx : public GameObject
{
	Image* image;
	bool bFlip;
	void Update() override
	{
		RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::BackGround, this);
	}
	void Render(HDC hdc) override
	{
		if (image)
		{
			image->FrameRender(hdc, Pos.x + ScrollManager::GetInstance()->GetScroll().x, Pos.y + +ScrollManager::GetInstance()->GetScroll().y, 0, 0, bFlip, false, ScrollManager::GetInstance()->GetScale());
		}
	}
};



