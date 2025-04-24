#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"
//
//enum class EnemyState
//{
//	IDLE,		// Ž��
//	Patrol,		// �ܼ� �̵�
//	Attack,		// ���� ���� �� ����
//	Chasing,	// Ž�� �� ����
//	Dead,		// �ǰ� ��
//};

class Collider;
class TestObject : public GameObject
{
public:
	TestObject();
	virtual ~TestObject() = default;

	void Init(string InImageKey, FPOINT InPos);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out) override;
	void ApplySnapShot(const EnemySnapShot& eSnapShot);
	void Patrol();

private:
	Image* Image;
	Collider* ObjectCollider;
	//EnemyState eState{ EnemyState::IDLE };
	float detectionRange{ 600.0f };
	float verticalRange{ 20.0f };
	float elapsedTime{ 0.0f };
	float patrolDuration{ 3.0f };
	float idleDuration{ 0.6f };
	float speed{ 100.0f };
	float chasingSpeed{ 200.0f };
	int dir{ 1 };

};

