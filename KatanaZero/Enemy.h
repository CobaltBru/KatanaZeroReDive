#pragma once
#include "GameObject.h"
#include "config.h"
#include "EnemyState.h"
#include "INode.h"
#include <memory>
#include "NavigationPath.h"

enum class EType
{
	Grunt,
	Pomp,
	Gangster,
	None
};

class GPImage;
class PathFinder;
class Enemy : public GameObject
{
protected:
	EnemyState* eState;				// �������Ͽ� Ŭ����, delete �߰�
	GPImage* image;					// GDI+ �̹���, delete �߰�
	vector<GPImage*> images;		// �� ĳ���Ϳ� �ʿ��� ��� �̹���, ����س��� �ҷ����� ��, delete �߰�
	string currAnimKey;
	float Speed;					// �̵��ӵ�
	int currFrame;					// �ִϸ��̼�
	float frameTimer;				// �ִϸ��̼� ������ƮŸ��
	bool bFlip;						// ������
	int dir;
	float detectRange;
	float attackRange;
	float meleeAttackRange;

	EType eType;
	int targetFloor;
	bool bReachedTargetFloor;
	float HitAngle;
	bool canHit;

	// BT�� ��Ʈ������
	Selector* root;
	float idleTimer{ 0.f };
	float idleDuration{ 1.5f };
	bool canPatrol{ false };
	float patrolTimer{ 0.f };
	float patrolDuration{ 3.f };
	float attackDuration{ 0.8f };
	float attackTimer{ 0.f };
	bool bChasing{ false };
	bool bAttacking{ false };

	// ��� Ž�� �� pathFinder ��ü
	PathFinder* pathFinder;
	NavigationPath navPath;
	float recalcDuration{ 0.3f };
	float recalcTimer{ 0.f };

	// ���� �ݶ��̴� �Ǻ�
	float baseAttackPivotX;
	float baseAttackPivotY;

	// ���� ���� ���� �Ǵ�
	bool bHitted{ false };

	// Jump
	float Gravity;
	float dY;
	bool bJump;
	bool bFalling;
	bool bDown;
	bool bDead{ false };
	bool bCanAttack;
	
	Collider* AttackCollider;
public:
	Enemy();
	virtual ~Enemy();
	virtual HRESULT Init(FPOINT InPos);
	virtual HRESULT Init(string InImageKey, FPOINT InPos, FPOINT InColliderOffset, FPOINT InColliderSize, bool InFlip, ERenderGroup InRenderGroup);
	virtual void InitImages();
	virtual void InitRigidBodySetting();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void MakeSnapShot(void* out);
	EnemyState* GetState() const { return eState; }
	int GetMaxAttackFrame() const;
	int GetCurrFrame() const { return currFrame; }
	EType GetEnemyType() const { return eType; }
	int GetDir() const { return dir; }
	void SetDir(int InDir) { dir = InDir; }
	float GetSpeed() const { return Speed; }
	GPImage* GetImage() const { return image; }
	void SetTargetFloor(int floor) { targetFloor = floor; }
	int GetTargetFloor() const { return targetFloor; }
	void SetReachedTargetFloor(bool value) { bReachedTargetFloor = value; }
	bool HasReachedTargetFloor() const { return bReachedTargetFloor; }
	void SetHitAngle(float hitangle) { HitAngle = hitangle; }
	float GetHitAngle() const { return HitAngle; }

	void UpdateAnimation();
	void ChangeState(EnemyState* newState);
	void ChangeAnimation(EImageType newImage);
	virtual void SetAnimKey(EImageType newImage) {};
	virtual bool Detecting();
	virtual bool IsInAttackRange();
	virtual bool IsInMeleeAttackRange();
	virtual bool IsInSameFloor();
	virtual bool IsOnDownLine();
	void AddATimer(float dt) { attackTimer += dt; }
	float GetATimer() { return attackTimer; }
	void SetATimer(float t) { attackTimer = t; }
	float GetADuration() { return attackDuration; }
	void SetCanAttack(bool bAttack) { bCanAttack = bAttack; }
	bool CanAttack() { return bCanAttack; }
	bool IsHitted();
	void Collision();

	virtual void DebugDrawText(HDC hdc, int x, int y, const char* text)
	{
		TextOutA(hdc, x, y, text, (int)strlen(text));
	}

	// �ൿƮ�� ���� �޼���
	virtual NodeStatus IDLEAction();
	virtual NodeStatus PatrolAction();
	virtual NodeStatus DeadAction();
	virtual NodeStatus MeleeAttackAction();
	virtual NodeStatus ChaseAction();
	virtual NodeStatus CalcPathAction();
	virtual NodeStatus FindPathAction();
	virtual NodeStatus WatingAction();
	virtual NodeStatus AttackIDLEAction();
};
