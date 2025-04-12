#include "Collider.h"
#include "GameObject.h"
#include "ScrollManager.h"

Collider::Collider(GameObject* owner, EColliderType colliderType, FPOINT pivot, FPOINT size, bool debugDraw, float hitDelayTime)
	:Owner(owner), ColliderType(colliderType), PivotPos(pivot), Size(size), bDebugDraw(debugDraw), HitDelayTime(hitDelayTime), Pos(), bHit(false), bCanHit(true), bDead(false), CurrentDelayTime(0.f)
{
}

Collider::Collider(GameObject* owner, EColliderType colliderType, FPOINT pivot, float size, bool debugDraw, float hitDelayTime)
	: Owner(owner), ColliderType(colliderType), PivotPos(pivot), Size({ size,size }), bDebugDraw(debugDraw), HitDelayTime(hitDelayTime), Pos(), bHit(false), bCanHit(true), bDead(false), CurrentDelayTime(0.f)
{
}

void Collider::Init()
{
}

void Collider::Update()
{
	if (Owner)
	{
		Pos.x = Owner->GetPos().x + PivotPos.x;
		Pos.y = Owner->GetPos().y + PivotPos.y;

		if (!bCanHit)
		{
			CurrentDelayTime += TimerManager::GetInstance()->GetDeltaTime();

			if (CurrentDelayTime >= HitDelayTime)
			{
				CurrentDelayTime = 0.f;
				bCanHit = true;
				bHit = false;
			}
		}
	}
}

void Collider::Render(HDC hdc)
{
#ifdef _DEBUG

	FPOINT HalfSize = { Size.x / 2.f, Size.y / 2.f };

	HPEN hPen;

	if (!bHit)
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // 초록
	else
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // 빨강

	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // 현재 DC에 펜을 설정

	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

	switch (ColliderType)
	{
	case EColliderType::Sphere:
		Ellipse(hdc, (int)(Pos.x - HalfSize.x) + Scroll.x, (int)(Pos.y - HalfSize.y) + Scroll.y, (int)(Pos.x + HalfSize.x) + Scroll.x, (int)(Pos.y + HalfSize.y) + Scroll.y);
		break;
	case EColliderType::Rect:
		DrawRectLine(hdc, HalfSize);
		break;
	}
	// 사용한 펜을 원래대로 복원
	SelectObject(hdc, hOldPen);

	// 펜 메모리 해제
	DeleteObject(hPen);

#endif // _DEBUG
}

void Collider::Release()
{
}

void Collider::DrawRectLine(HDC hdc, FPOINT HalfSize)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

	MoveToEx(hdc, int(Pos.x - HalfSize.x) + Scroll.x, int(Pos.y - HalfSize.y) + Scroll.y, NULL);
	LineTo(hdc, int(Pos.x + HalfSize.x) + Scroll.x, int(Pos.y - HalfSize.y) + Scroll.y);
	LineTo(hdc, int(Pos.x + HalfSize.x) + Scroll.x, int(Pos.y + HalfSize.y) + Scroll.y);
	LineTo(hdc, int(Pos.x - HalfSize.x) + Scroll.x, int(Pos.y + HalfSize.y) + Scroll.y);
	LineTo(hdc, int(Pos.x - HalfSize.x) + Scroll.x, int(Pos.y - HalfSize.y) + Scroll.y);
}
