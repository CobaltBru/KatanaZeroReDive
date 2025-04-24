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

struct Line
{
	Line(FPOINT st, FPOINT ed)
		: start(st), end(ed), bDebugDraw(false), CurrentTime(0.f), DebugDuration(0.f), DebugColor(RGB(0, 255, 0))
	{

	}

	void LineRender(HDC hdc)
	{
		MoveToEx(hdc, (int)start.x, (int)start.y, NULL);
		LineTo(hdc, (int)end.x, (int)end.y);
	}
	void Update()
	{
		CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
		if (CurrentTime >= DebugDuration)
		{
			CurrentTime = 0.f;
			bDebugDraw = false;
		}
	}
	FPOINT start;
	FPOINT end;

	float CurrentTime;
	float DebugDuration;
	bool bDebugDraw;
	COLORREF DebugColor;
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

	/// <summary>
	/// 라인트레이스 충돌 함수
	/// <para> hitResult : 맞은 것의 대한 정보 반환값 /
	/// objType : 레이를 맞는 대상의 오브젝트 타입 /
	/// start : 레이 시작점 /
	/// end : 레이 끝점 /
	/// owner : 레이를 쏘는 대상 /
	/// bIgnoreSelf : 자기 자신은 레이를 맞는 대상 유무 /
	/// bDebugDraw : 확인용 Render가 필요 유무 /
	/// DebugDuration : 렌더 지속시간 /
	/// DebugColor : RGB()를 사용하여 해당 레이의 색깔 결정
	/// </para>
  /// </summary>
	bool LineTraceByObject(FHitResult& hitResult, ECollisionGroup CollisionGroup, FPOINT start, FPOINT end, bool bDebugDraw = true, float DebugDuration = 1.f, COLORREF DebugColor = RGB(255,0,0));
	bool CheckLineTrace(FPOINT p1, FPOINT p2, FPOINT p3, FPOINT p4);
	int ccw(FPOINT p1, FPOINT p2, FPOINT p3);

	//치트키
	Collider* GetTargetCollider(ECollisionGroup InCollisionGroup) { return CollisionList[(int)InCollisionGroup].front(); }
private:
	list<Collider*> CollisionList[(int)ECollisionGroup::End];
	list<Line*> LineList;
};

