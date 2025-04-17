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

	float GetMass() const { return Mass; }
	bool GetGround() const { return bGround; }

	void SetMass(float InMass) {Mass = InMass;}
	void SetVelocity(FPOINT InVelocity) { Velocity = InVelocity; }
	void SetMaxSpeed(float InMaxSpeed) { MaxSpeed = InMaxSpeed;}
	void SetGround(bool InGround) { bGround = InGround; }
	void SetGravityVisible(bool InGravity) { bGravity = InGravity; }

	void AddForce(FPOINT InForce) {	Force += InForce;}
	void AddVelocity(FPOINT InVelocity) { Velocity += InVelocity; }

private:
	void Move();

	void CollisionLine();
private:
	GameObject* Owner;

	
	FPOINT Velocity;		// 속도 (속력, 방향)
	FPOINT Force;			// 크기 방향정보
	FPOINT Acceleration;		// 가속도
	float Mass;				// 질량
	 
	float FrictionCoefficient; // 마찰계수
	float MaxSpeed;		// 최대 속력

	// F = m * a    즉 퓨마				힘 = 질량 * 가속도
	// V += a * DT  (deltatime)

	float Gravity;
	bool bGravity;
	bool bGround;
};

