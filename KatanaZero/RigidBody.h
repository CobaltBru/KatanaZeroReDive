#pragma once
#include "config.h"

class GameObject;
class RigidBody
{
public:
	RigidBody();
	RigidBody(GameObject* InOwner);
	~RigidBody();

	void Update();
	void GravityUpdate();

	FPOINT GetVelocity() const { return Velocity; }
	float GetMass() const { return Mass; }
	bool GetGround() const { return bGround; }

	void SetMass(float InMass) {Mass = InMass;}
	void SetVelocity(FPOINT InVelocity) { Velocity = InVelocity; }
	void SetMaxVelocity(FPOINT InMaxVelocity) { MaxVelocity = InMaxVelocity;}
	void SetGround(bool InGround) { bGround = InGround; }
	void SetGravityVisible(bool InGravity) { bGravity = InGravity; }
	void SetAccelerationAlpha(FPOINT InAccelerationAlpha) { AccelerationAlpha = InAccelerationAlpha; }
	void SetDown(bool InDown) { bDown = InDown; }
	void SetElasticity(float InElasticity) { Elasticity = InElasticity; }

	void AddForce(FPOINT InForce) {	Force += InForce;}
	void AddVelocity(FPOINT InVelocity) { Velocity += InVelocity; }

private:
	void Move();

	void CollisionLine();
private:
	GameObject* Owner;

	
	FPOINT Velocity;		// 속도 (속력, 방향)
	FPOINT MaxVelocity;		// 최대 속도
	FPOINT Force;			// 크기 방향정보
	FPOINT Acceleration;		// 가속도
	FPOINT AccelerationAlpha;		// 가속도 알파
	float Mass;				// 질량
	 
	float FrictionCoefficient; // 마찰계수
	float Elasticity; // 탄성

	// F = m * a    즉 퓨마				힘 = 질량 * 가속도
	// V += a * DT  (deltatime)

	float Gravity;
	bool bGravity;
	bool bGround;
	bool bDown;
	bool bDiagonalLine;
};

