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

	for (auto iter = LineList.begin(); iter != LineList.end();)
	{
		(*iter)->Update();
		if (!(*iter)->bDebugDraw)
		{
			delete (*iter);
			(*iter) = nullptr;
			iter = LineList.erase(iter);
		}
		else
			++iter;
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

	for (auto& iter : LineList)
	{
		if (iter->bDebugDraw)
		{
			HPEN hPen = CreatePen(PS_SOLID, 4, iter->DebugColor);
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // 현재 DC에 펜을 설정

			iter->LineRender(hdc);

			// 사용한 펜을 원래대로 복원
			SelectObject(hdc, hOldPen);

			// 펜 메모리 해제
			DeleteObject(hPen);
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

	for (auto& iter : LineList)
		delete iter;
	LineList.clear();

	ReleaseInstance();
}

bool CollisionManager::CollisionAABB(Collider* InSour, FHitResult& OutHitResult, ECollisionGroup InCollisionGroup)
{
	if (!InSour->CanHit())
		return false;

	for (auto& DestCollider : CollisionList[(int)InCollisionGroup])
	{
		if (!DestCollider->CanHit())
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
	if (!InSour->CanHit())
		return false;

	for (auto& DestCollider : CollisionList[(int)InCollisionGroup])
	{
		if (!DestCollider->CanHit())
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

bool CollisionManager::LineTraceByObject(FHitResult& hitResult, ECollisionGroup CollisionGroup, FPOINT start, FPOINT end, bool bDebugDraw, float DebugDuration, COLORREF DebugColor)
{
	if (bDebugDraw)
	{
		Line* line = new Line(start, end);
		line->bDebugDraw = bDebugDraw;
		line->DebugDuration = DebugDuration;
		line->DebugColor = DebugColor;
		LineList.push_back(line);
	}
	// 네 변을 다 검사해야 한다
	// ccw 알고리즘 사용
	for (auto& iter : CollisionList[(int)CollisionGroup])
	{
		if (!iter->CanHit())
			continue;

		const FPOINT collPos = iter->GetPos();
		const FPOINT collSize = iter->GetSize();
		const FPOINT collHalfSize = { collSize.x * 0.5f, collSize.y * 0.5f };

		// 0 1 Rect 기준
		// 3 2
		const FPOINT CollRect[4] = {
			{collPos.x - collHalfSize.x,collPos.y - collHalfSize.y}, {collPos.x + collHalfSize.x,collPos.y - collHalfSize.y},
			{collPos.x + collHalfSize.x, collPos.y + collHalfSize.y}, {collPos.x - collHalfSize.x,collPos.y + collHalfSize.y}
		};

		bool bCollision = false;
		for (int i = 0; i < 4; ++i)
		{
			const FPOINT collEnd = i == 3 ? CollRect[0] : CollRect[i + 1];
			if (CheckLineTrace(CollRect[i], collEnd, start, end))
			{
				// 충돌
				bCollision = true;
				break;
			}
		}

		if (bCollision)
		{
			hitResult.HitCollision = iter;
			return true;
		}
	}

	return false;
}

bool CollisionManager::CheckLineTrace(FPOINT p1, FPOINT p2, FPOINT p3, FPOINT p4)
{
	int ccw1 = ccw(p1, p2, p3) * ccw(p1, p2, p4);
	int ccw2 = ccw(p3, p4, p1) * ccw(p3, p4, p2);

	// 두 선이 일단 평행함 두 선의 min,max 비교 
	if (ccw1 == 0 && ccw2 == 0)
	{
		// p1 , p2    // p3, p4는 각 선분이다.
		// ---------------              ------------- 
		FPOINT minAB = { min(p1.x,p2.x),min(p1.y,p2.y) };
		FPOINT maxAB = { max(p1.x,p2.x),max(p1.y,p2.y) };
		FPOINT minCD = { min(p3.x,p4.x),min(p3.y,p4.y) };
		FPOINT maxCD = { max(p3.x,p4.x),max(p3.y,p4.y) };

		return (minAB.x <= maxCD.x && minAB.y <= maxCD.y && minCD.x <= maxAB.x && minCD.y <= maxAB.y);
	}

	return (ccw1 <= 0 && ccw2 <= 0);
}

int CollisionManager::ccw(FPOINT p1, FPOINT p2, FPOINT p3)
{
	const float result = (p1.x * p2.y + p2.x * p3.y + p3.x * p1.y) - (p1.y * p2.x + p2.y * p3.x + p3.y * p1.x);

	if (result > 0.f) return 1;
	if (result < 0.f) return -1;
	return 0;
}
