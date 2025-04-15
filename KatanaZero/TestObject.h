#pragma once
#include "GameObject.h"
#include "SnapShotManager.h"

enum class EnemyState
{
	IDLE,
	Patrol,
	Attack,
	Chasing,
	Dead,
	End
};

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

private:
	Image* Image;
	Collider* ObjectCollider;
	EnemyState eState{ EnemyState::IDLE };

};

