#pragma once
#include "GameObject.h"

class Image;
class Collider;
class RigidBody;
class SimpleTestObject : public GameObject
{
public:
	SimpleTestObject();
	virtual ~SimpleTestObject() = default;

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

	void RigidBodyTest();

private:
	Image* Image;
	//Collider* ObjectCollider;
	float Speed;

	FPOINT scroll;

	//Jump
	float Gravity;
	float dY;
	bool bJump;
	bool bFalling;
	bool bDown;
	bool bDead{ false };


	// ���� �ʿ��� ������

};

