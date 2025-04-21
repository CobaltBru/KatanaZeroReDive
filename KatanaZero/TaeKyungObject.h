#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"

class GPImage;
class Image;
class Collider;
class RigidBody;
class TaeKyungObject : public GameObject
{
public:
	TaeKyungObject();
	virtual ~TaeKyungObject() = default;

	virtual HRESULT Init(FPOINT InPos) override;
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out) override;
	void ApplySnapShot(const PlayerSnapShot& snapShot);
	inline bool IsDead() { return bDead; }

	Collider* GetCollider() const {return ObjectCollider;}
	FPOINT GetLastPos()  const { return LastPos; }
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

	//잔상효과 테스트
	GPImage* gpImage;
	float timer;
	FPOINT scroll;

	//Jump
	float Gravity;
	float dY;
	bool bJump;
	bool bFalling;
	bool bDown;
	bool bDead{ false };


	// 물리 필요한 데이터
	FPOINT LastPos;
};

