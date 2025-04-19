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

	
	FPOINT Velocity;		// �ӵ� (�ӷ�, ����)
	FPOINT MaxVelocity;		// �ִ� �ӵ�
	FPOINT Force;			// ũ�� ��������
	FPOINT Acceleration;		// ���ӵ�
	FPOINT AccelerationAlpha;		// ���ӵ� ����
	float Mass;				// ����
	 
	float FrictionCoefficient; // �������
	float Elasticity; // ź��

	// F = m * a    �� ǻ��				�� = ���� * ���ӵ�
	// V += a * DT  (deltatime)

	float Gravity;
	bool bGravity;
	bool bGround;
	bool bDown;
	bool bDiagonalLine;
};

