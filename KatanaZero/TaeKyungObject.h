#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"

class Image;
class Collider;
class TaeKyungObject : public GameObject
{
public:
	TaeKyungObject();
	virtual ~TaeKyungObject() = default;

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out) override;
	void ApplySnapShot(const PlayerSnapShot& snapShot);
	inline bool IsDead() { return bDead; }

private:
	void Move();
	void Collision();

	void InitOffset();
	void Offset();
private:
	Image* Image;
	Collider* ObjectCollider;
	float Speed;
	bool bDead{ false };
};

