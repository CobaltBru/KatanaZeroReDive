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

	FPOINT Velocity;		// �ӵ� (�ӷ�, ����)
	FPOINT Force;			// ũ�� ��������
	FPOINT Acceleration;		// ���ӵ�
	float Mass;				// ����
	 
	// F = m * a    �� ǻ��				�� = ���� * ���ӵ�
	// V += a * DT  (deltatime)
};

