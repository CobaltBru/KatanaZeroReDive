#pragma once
#include "GameObject.h"

class Image;
class Collider;
class RigidBody;
class SimpleObject : public GameObject
{
public:
	SimpleObject();
	virtual ~SimpleObject() = default;

	HRESULT Init(FPOINT InPos);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	inline bool IsDead() { return bDead; }

	//Collider* GetCollider() const { return ObjectCollider; }
	//RigidBody* GetRigidBody() const { return ObjectRigidBody; }
	//FPOINT GetLastPos()  const { return LastPos; }
	
private:
	void Collision();

	void InitOffset();
	void Offset();

	void RigidBodyTest();

private:
	Image* Image;
	/*Collider* ObjectCollider;
	RigidBody* ObjectRigidBody;*/
	float Speed;

	FPOINT scroll;

	//Jump
	float Gravity;
	float dY;
	bool bJump;
	bool bFalling;
	bool bDown;
	bool bDead{ false };


	// 물리 필요한 데이터
	//FPOINT LastPos;
};

