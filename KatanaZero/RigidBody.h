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

	void SetMass(float InMass) {
		Mass = InMass;
	}

	void AddForce(FPOINT InForce) {
		Force += InForce;
	}

private:
	void Move();

private:
	GameObject* Owner;

	FPOINT Velocity;		// 속도 (속력, 방향)
	FPOINT Force;			// 크기 방향정보
	FPOINT Acceleration;		// 가속도
	float Mass;				// 질량
	 
	// F = m * a    즉 퓨마				힘 = 질량 * 가속도
	// V += a * DT  (deltatime)
};

