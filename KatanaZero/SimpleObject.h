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

	HRESULT Init(FPOINT InPos, string InImageName = "rocket");
	virtual	HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup = ERenderGroup::NonAlphaBlend) override;

	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void MakeSnapShot(void* out) override;

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

