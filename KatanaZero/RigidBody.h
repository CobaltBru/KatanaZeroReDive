#pragma once
#include "config.h"

class GameObject;
class RigidBody
{
public:
	RigidBody();
	RigidBody(GameObject* InOwner, bool CanSlowSystem = true);
	~RigidBody();

	void Update();
	void GravityUpdate();

	const FPOINT& GetVelocity() const { return Velocity; }
	const float& GetMass() const { return Mass; }
	const bool& IsGround() const { return bGround; }
	const FLineResult& GetResult() const { return Result; }

	void SetMass(float InMass) {Mass = InMass;}
	void SetVelocity(FPOINT InVelocity) { Velocity = InVelocity; }
	void SetMaxVelocity(FPOINT InMaxVelocity) { MaxVelocity = InMaxVelocity;}
	void SetGround(bool InGround) { bGround = InGround; }
	void SetGravityVisible(bool InGravity) { bGravity = InGravity; }
	void SetAccelerationAlpha(FPOINT InAccelerationAlpha) { SaveAccelerationAlpha = InAccelerationAlpha; }
	void SetDown(bool InDown) { bDown = InDown; }
	void SetElasticity(float InElasticity) { Elasticity = min(InElasticity,1.f); }
	void SetFriction(float InFriction) {FrictionCoefficient = InFriction;}

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
	FPOINT SaveAccelerationAlpha;		// ���ӵ� ����
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
	bool CanSlowSystem;
	FLineResult Result;
};

