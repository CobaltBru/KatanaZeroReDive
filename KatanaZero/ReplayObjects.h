#pragma once
#include "SnapShot.h"
#include "config.h"
#include "GPImageManager.h"
#include "Image.h"
#include "ImageManager.h"
#include "ScrollManager.h"
#include "GameObject.h"

struct ReplayBase : public GameObject
{
	virtual ~ReplayBase() {}
	virtual void Release() override { delete this; }
	virtual bool IsDead() const override { return false; }
	virtual void ApplySnapShot(const PlayerSnapShot& pSnap) {}
	virtual void ApplySnapShot(const EnemySnapShot& eSnap) {}
	virtual void ApplySnapShot(const EffectSnapShot& fxSnap) {}
	virtual void Render(HDC hdc) override {}
};

class ReplayPlayer : public ReplayBase
{
public:
	PlayerSnapShot snap;
	void ApplySnapShot(const PlayerSnapShot& pSnap) override
	{
		snap = pSnap;
	}
	void Render(HDC hdc) override
	{
		GPImage* image = GPImageManager::GetInstance()->FindImage(snap.animKey);
		FPOINT scroll = ScrollManager::GetInstance()->GetScroll();
		if (image)
		{
			Gdiplus::Graphics graphics(hdc);
			image->Middle_RenderFrameScale(&graphics, snap.pos + scroll, snap.animFrame, snap.bFlip, 1.f, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
		}
		
	}
};

class ReplayEnemy : public ReplayBase
{
public:
	EnemySnapShot snap;
	void ApplySnapShot(const EnemySnapShot& eSnap) override
	{
		snap = eSnap;
	}
	void Render(HDC hdc) override
	{
		GPImage* image = GPImageManager::GetInstance()->FindImage(snap.animKey);
		FPOINT scroll = ScrollManager::GetInstance()->GetScroll();
		if (image)
		{
			Gdiplus::Graphics graphics(hdc);
			image->Middle_RenderFrameScale(&graphics, snap.pos + scroll, snap.animFrame, snap.bFlip, 1.f, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
		}
		
	}
};

class ReplayEffect : public ReplayBase
{
public:
	int ID;
	EffectSnapShot snap;
	void ApplySnapShot(const EffectSnapShot& fxSnap) override
	{
		snap = fxSnap;
	}
	void Render(HDC hdc) override
	{
		GPImage* image = GPImageManager::GetInstance()->FindImage(snap.effectKey);
		FPOINT scroll = ScrollManager::GetInstance()->GetScroll();
		if (image)
		{
			Gdiplus::Graphics graphics(hdc);
			image->Middle_RenderAll(&graphics, snap.pos + scroll, snap.animFrame, snap.angle, snap.bFlip, 1.f, 1.f, 1.f, 1.f, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
			//image->Middle_RenderFrameScale(&graphics, snap.pos, snap.animFrame, snap.bFlip, 1.f, ScrollManager::GetInstance()->GetScale(), ScrollManager::GetInstance()->GetScale());
		}
	}
	void SetID(int id) { ID = id; }
	int GetID() { return ID; }
};
