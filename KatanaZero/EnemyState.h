#pragma once
#include "config.h"
#include "TimerManager.h"

enum class EImageType
{
	IDLE,
	Walk,
	Run,
	Attack,
	GangsterAttack,
	Dead,
	End
};

class Enemy;
class EnemyState
{
protected:
	string state;
public:
	virtual void Enter(Enemy& enemy) {};
	virtual void Update(Enemy& enemy) = 0;
	virtual void Exit(Enemy& enemy) {};

	virtual EnemyState* CheckTransition(Enemy* enemy) { return nullptr; }

	inline string GetState() { return state; }

	EnemyState() {};
	virtual ~EnemyState() {};
};

class EIDLE : public EnemyState
{
private:
	float idletimer{ 0.f };
	float idleCooldown{ 2.f };
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class EWalk : public EnemyState
{
private:
	float walktimer{ 0.f };
	float walkCooldown{ 2.f };
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class ERun : public EnemyState
{
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class EAttack : public EnemyState
{
protected:
	bool isAttackFinish{ false };
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class EDead : public EnemyState
{
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class GruntAttack : public EAttack
{

public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class PompAttack : public EAttack
{
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class GangsterAttack : public EAttack
{
private:
	bool isFire{ false };
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class GangsterMeleeAttack : public EAttack
{
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class EFindSlope : public EnemyState
{
private:
	FPOINT slopeEntry;
	FPOINT slopeExit;
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class ERunOnSlope : public EnemyState
{
private:
	FPOINT slopeEntry;
	FPOINT slopeExit;
	int targetFloor;
public:
	ERunOnSlope(const FPOINT& entry, const FPOINT& exit, int targetFloor);
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class PompGroggy : public EnemyState
{
private:
	float groggyTimer{ 0.f };
	float groggyDuration{ 1.f };
	bool bCanStand{ false };
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};