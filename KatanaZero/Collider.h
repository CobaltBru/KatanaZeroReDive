#pragma once
#include "config.h"

enum class EColliderType
{
	Sphere,
	Rect,
	End
};

class GameObject;
class Collider
{
public:
	Collider(GameObject* owner, EColliderType colliderType, FPOINT pivot, FPOINT size, bool debugDraw, float hitDelayTime = 1.f);
	Collider(GameObject* owner, EColliderType colliderType, FPOINT pivot, float size, bool debugDraw, float hitDelayTime = 1.f);
	~Collider() = default;

	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	GameObject* GetOwner() const { return Owner; }
	FPOINT GetPos() const { return Pos; }
	FPOINT GetSize() const { return Size; }
	EColliderType GetColliderType() const { return ColliderType; }
	bool IsDead() const { return bDead; }

	bool CanDebugDraw() const { return bDebugDraw; }
	bool CanHit() const { return bCanHit; }

	void SetSize(FPOINT size) { Size = size; }
	void SetHit(bool bhit) {
		this->bHit = bhit;
		bCanHit = false;
	}
	void SetPivot(FPOINT pivot) { PivotPos = pivot; }
	void SetHitDelayTime(float hitDelayTime) { HitDelayTime = hitDelayTime; }
	void SetDead(bool bDead) { this->bDead = bDead; }
	void SetPos(FPOINT InPos) { Pos = InPos; }
private:
	void DrawRectLine(HDC hdc, FPOINT HalfSize);
private:
	GameObject* Owner;
	EColliderType ColliderType;

	FPOINT Pos;
	FPOINT PivotPos;
	FPOINT Size;

	bool bHit;
	bool bCanHit;
	bool bDebugDraw;
	bool bDead;

	float CurrentDelayTime;
	float HitDelayTime;
};

