#pragma once
#include "Singleton.h"
#include "config.h"
#include <list>




enum class ECollisionGroup : uint8_t
{
	Player,
	Enemy,
	Bullet,
	Item,
	Projectile,
	End
};

class Collider;
class GameObject;

struct FHitResult
{
	Collider* HitCollision;		// 이름을 HitCollider로 바꿔야하는데 ㅠ
};

class CollisionManager : public Singleton<CollisionManager>
{
public:
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void AddCollider(Collider* InCollider, ECollisionGroup InCollisionGroup) { CollisionList[(int)InCollisionGroup].push_back(InCollider); }

	bool CollisionAABB(Collider* InSour, FHitResult& OutHitResult ,ECollisionGroup InCollisionGroup);
	bool CollisionSphere(Collider* InSour, FHitResult& OutHitResult, ECollisionGroup InCollisionGroup);

private:
	list<Collider*> CollisionList[(int)ECollisionGroup::End];

};

