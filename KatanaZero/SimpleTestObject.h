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

	virtual HRESULT Init(FPOINT InPos);
	virtual	HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup= ERenderGroup::NonAlphaBlend) override;

	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	//Collider* GetCollider() const { return ObjectCollider; }
	//RigidBody* GetRigidBody() const { return ObjectRigidBody; }
	//FPOINT GetLastPos()  const { return LastPos; }

	void InitRegidBodySetting();

private:
	void Collision();

	void RigidBodyTest();

private:
	Image* Image;
	//Collider* ObjectCollider;
	float ScrollSpeed;
};

