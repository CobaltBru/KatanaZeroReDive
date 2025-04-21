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

	virtual HRESULT Init(FPOINT InPos);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	//Collider* GetCollider() const { return ObjectCollider; }
	//RigidBody* GetRigidBody() const { return ObjectRigidBody; }
	//FPOINT GetLastPos()  const { return LastPos; }
	
private:
	void Collision();

	void InitOffset();
	void Offset();

	void RigidBodyTest();
	void InitRegidBodySetting();

	void PhysicsMove();
	void NoPhysicsMove();

private:
	Image* Image;
	/*Collider* ObjectCollider;
	RigidBody* ObjectRigidBody;*/

	float ScrollSpeed;
	FPOINT scroll;

	bool bWall;
	bool bIsLeft;
};

