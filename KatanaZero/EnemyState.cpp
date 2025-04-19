#include "EnemyState.h"
#include "Enemy.h"

void EIDLE::Enter(Enemy& enemy)
{
	state = "IDLE";
	enemy.ChangeAnimation(EImageType::IDLE);
}

void EIDLE::Update(Enemy& enemy)
{
}

void EIDLE::Exit(Enemy& enemy)
{
}

EnemyState* EIDLE::CheckTransition(Enemy* enemy)
{
	return nullptr;
}

void EWalk::Enter(Enemy& enemy)
{
	state = "Walk";
	enemy.ChangeAnimation(EImageType::Walk);
}

void EWalk::Update(Enemy& enemy)
{
}

void EWalk::Exit(Enemy& enemy)
{
}

EnemyState* EWalk::CheckTransition(Enemy* enemy)
{
	return nullptr;
}

void ERun::Enter(Enemy& enemy)
{
	state = "Run";
	enemy.ChangeAnimation(EImageType::Run);
}

void ERun::Update(Enemy& enemy)
{
}

void ERun::Exit(Enemy& enemy)
{
}

EnemyState* ERun::CheckTransition(Enemy* enemy)
{
	return nullptr;
}

void EAttack::Enter(Enemy& enemy)
{
	state = "Attack";
	enemy.ChangeAnimation(EImageType::Attack);
}

void EAttack::Update(Enemy& enemy)
{
}

void EAttack::Exit(Enemy& enemy)
{
}

EnemyState* EAttack::CheckTransition(Enemy* enemy)
{
	return nullptr;
}

void EDead::Enter(Enemy& enemy)
{
	state = "Dead";
	enemy.ChangeAnimation(EImageType::Dead);
}

void EDead::Update(Enemy& enemy)
{
}

void EDead::Exit(Enemy& enemy)
{
}

EnemyState* EDead::CheckTransition(Enemy* enemy)
{
	return nullptr;
}
