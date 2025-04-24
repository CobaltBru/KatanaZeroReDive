#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"
//
//enum class EnemyState
//{
//	IDLE,		// 탐지
//	Patrol,		// 단순 이동
//	Attack,		// 추적 성공 시 공격
//	Chasing,	// 탐지 후 추적
//	Dead,		// 피격 시
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

