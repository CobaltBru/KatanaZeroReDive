#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"

class GPImage;
class Image;
class Collider;
class TaeKyungObject : public GameObject
{
public:
	TaeKyungObject();
	virtual ~TaeKyungObject() = default;

	HRESULT Init(FPOINT InPos);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out) override;
	void ApplySnapShot(const PlayerSnapShot& snapShot);
	inline bool IsDead() { return bDead; }

private:
	void Move();
	void Jump();
	void Collision();

	void InitOffset();
	void Offset();
private:
	Image* Image;
	Collider* ObjectCollider;
	float Speed;

	//잔상효과 테스트
	GPImage* gpImage;
	float timer;

	//Jump
	float Gravity;
	float dY;
	bool bJump;
	bool bFalling;
	bool bDown;
	bool bDead{ false };
};

