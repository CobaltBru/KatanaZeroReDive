#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"

class Image;
class Collider;
class RigidBody;
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

	Collider* GetCollider() const {return ObjectCollider;}

private:
	void Move();
	void Jump();
	void Collision();

	void InitOffset();
	void Offset();

	void RigidBodyTest();
	
private:
	Image* Image;
	Collider* ObjectCollider;
	RigidBody* ObjectRigidBody;
	float Speed;

	//Jump
	float Gravity;
	float dY;
	bool bJump;
	bool bFalling;
	bool bDown;
	bool bDead{ false };
};

