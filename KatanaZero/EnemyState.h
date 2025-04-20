#pragma once
#include "config.h"
#include "TimerManager.h"

enum class EImageType
{
	IDLE,
	Walk,
	Run,
	Attack,
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
	float walkCooldown{ 3.f };
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
	float attackCooldown{ 0.f };
	float attacktimer{ 0.f };
	bool isAttacking{ true };
	void UpdateCooltime()
	{
		attacktimer += TimerManager::GetInstance()->GetDeltaTime();
	}
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
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class ShieldCopAttack : public EAttack
{
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};
