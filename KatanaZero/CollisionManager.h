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
	Collider* HitCollision;		// �̸��� HitCollider�� �ٲ���ϴµ� ��
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
	/// ����Ʈ���̽� �浹 �Լ�
	/// <para> hitResult : ���� ���� ���� ���� ��ȯ�� /
	/// objType : ���̸� �´� ����� ������Ʈ Ÿ�� /
	/// start : ���� ������ /
	/// end : ���� ���� /
	/// owner : ���̸� ��� ��� /
	/// bIgnoreSelf : �ڱ� �ڽ��� ���̸� �´� ��� ���� /
	/// bDebugDraw : Ȯ�ο� Render�� �ʿ� ���� /
	/// DebugDuration : ���� ���ӽð� /
	/// DebugColor : RGB()�� ����Ͽ� �ش� ������ ���� ����
	/// </para>
  /// </summary>
	bool LineTraceByObject(FHitResult& hitResult, ECollisionGroup CollisionGroup, FPOINT start, FPOINT end, bool bDebugDraw = true, float DebugDuration = 1.f, COLORREF DebugColor = RGB(255,0,0));
	bool CheckLineTrace(FPOINT p1, FPOINT p2, FPOINT p3, FPOINT p4);
	int ccw(FPOINT p1, FPOINT p2, FPOINT p3);

	//ġƮŰ
	Collider* GetTargetCollider(ECollisionGroup InCollisionGroup) { return CollisionList[(int)InCollisionGroup].front(); }
private:
	list<Collider*> CollisionList[(int)ECollisionGroup::End];
	list<Line*> LineList;
};

