#include "CollisionManager.h"
#include "GameObject.h"
#include "Collider.h"
#include "CommonFunction.h"


void CollisionManager::Init()
{
}

void CollisionManager::Update()
{
	for (int collGroup = 0; collGroup < (int)ECollisionGroup::End; ++collGroup)
	{
		for (auto iter = CollisionList[collGroup].begin(); iter != CollisionList[collGroup].end();)
		{
			if ((*iter)->IsDead() /*|| (*iter)->GetOwner() == nullptr || (*iter)->GetOwner()->IsDead()*/)
			{
				(*iter)->Release();
				delete (*iter);
				(*iter) = nullptr;
				iter = CollisionList[collGroup].erase(iter);
			}
			else
			{
				(*iter)->Update();
				++iter;
			}
		}
	}
}

void CollisionManager::Render(HDC hdc)
{
#ifdef _DEBUG
	for (int collGroup = 0; collGroup < (int)ECollisionGroup::End; ++collGroup)
	{
		for (auto& iter : CollisionList[collGroup])
		{
			if (!iter->CanDebugDraw())
				continue;

			iter->Render(hdc);
		}
	}
#endif
}

void CollisionManager::Release()
{
	for (int collGroup = 0; collGroup < (int)ECollisionGroup::End; ++collGroup)
	{
		for (auto& iter : CollisionList[collGroup])
		{
			iter->Release();
			delete iter;
			iter = nullptr;
		}
		CollisionList[collGroup].clear();
	}
}

bool CollisionManager::CollisionAABB(Collider* InSour, FHitResult& OutHitResult, ECollisionGroup InCollisionGroup)
{
	for (auto& DestCollider : CollisionList[(int)InCollisionGroup])
	{
		if (!InSour->CanHit() || !DestCollider->CanHit())
			continue;

		FPOINT collider1Pos = InSour->GetPos();
		FPOINT Collider1HalfSize = { InSour->GetSize().x * 0.5f,InSour->GetSize().y * 0.5f };

		FPOINT collider2Pos = DestCollider->GetPos();
		FPOINT Collider2HalfSize = { DestCollider->GetSize().x * 0.5f,DestCollider->GetSize().y * 0.5f };

		RECT rc1;
		rc1.left = LONG(collider1Pos.x - Collider1HalfSize.x);
		rc1.right = LONG(collider1Pos.x + Collider1HalfSize.x);
		rc1.top = LONG(collider1Pos.y - Collider1HalfSize.y);
		rc1.bottom = LONG(collider1Pos.y + Collider1HalfSize.y);

		RECT rc2;
		rc2.left = LONG(collider2Pos.x - Collider2HalfSize.x);
		rc2.right = LONG(collider2Pos.x + Collider2HalfSize.x);
		rc2.top = LONG(collider2Pos.y - Collider2HalfSize.y);
		rc2.bottom = LONG(collider2Pos.y + Collider2HalfSize.y);

		if (RectInRect(rc1, rc2))
		{
			OutHitResult.HitCollision = DestCollider;
			return true;
		}
	}

	return false;
}

bool CollisionManager::CollisionSphere(Collider* InSour, FHitResult& OutHitResult, ECollisionGroup InCollisionGroup)
{
	for (auto& DestCollider : CollisionList[(int)InCollisionGroup])
	{
		if (!InSour->CanHit() || !DestCollider->CanHit())
			continue;

		float radius = (InSour->GetSize().x + DestCollider->GetSize().x) * 0.5f;
		float distance = GetDistance(InSour->GetPos(), DestCollider->GetPos());

		if (distance <= radius)
		{
			OutHitResult.HitCollision = DestCollider;
			return true;
		}
	}

	return false;
}
