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

	
	FPOINT Velocity;		// �ӵ� (�ӷ�, ����)
	FPOINT Force;			// ũ�� ��������
	FPOINT Acceleration;		// ���ӵ�
	float Mass;				// ����
	 
	float FrictionCoefficient; // �������
	float MaxSpeed;		// �ִ� �ӷ�

	// F = m * a    �� ǻ��				�� = ���� * ���ӵ�
	// V += a * DT  (deltatime)

	float Gravity;
	bool bGravity;
	bool bGround;
};

