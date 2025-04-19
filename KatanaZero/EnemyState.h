#pragma once
#include "config.h"

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
public:
	virtual void Enter(Enemy& enemy) override;
	virtual void Update(Enemy& enemy) override;
	virtual void Exit(Enemy& enemy) override;
	virtual EnemyState* CheckTransition(Enemy* enemy) override;
};

class EWalk : public EnemyState
{
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

