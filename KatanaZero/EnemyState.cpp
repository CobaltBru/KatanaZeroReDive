#include "EnemyState.h"
#include "Enemy.h"
#include "RigidBody.h"

void EIDLE::Enter(Enemy& enemy)
{
	idletimer = 0.f;
	state = "IDLE";
	enemy.ChangeAnimation(EImageType::IDLE);
}

void EIDLE::Update(Enemy& enemy)
{
	idletimer += TimerManager::GetInstance()->GetDeltaTime();
	enemy.GetRigidBody()->Update();
}

void EIDLE::Exit(Enemy& enemy)
{
}

EnemyState* EIDLE::CheckTransition(Enemy* enemy)
{
	if (enemy->Detecting())
	{
		return new ERun();
	}
	if (idletimer > idleCooldown)
	{
		return new EWalk();
	}
	return nullptr;
}

void EWalk::Enter(Enemy& enemy)
{
	walktimer = 0.f;
	state = "Walk";
	enemy.ChangeAnimation(EImageType::Walk);
}

void EWalk::Update(Enemy& enemy)
{
	walktimer += TimerManager::GetInstance()->GetDeltaTime();
	enemy.GetRigidBody()->Update();
}

void EWalk::Exit(Enemy& enemy)
{
}

EnemyState* EWalk::CheckTransition(Enemy* enemy)
{
	if (enemy->Detecting())
	{
		return new ERun();
	}
	if (walktimer > walkCooldown)
	{
		return new EIDLE();
	}
	return nullptr;
}

void ERun::Enter(Enemy& enemy)
{
	state = "Run";
	enemy.ChangeAnimation(EImageType::Run);
}

void ERun::Update(Enemy& enemy)
{
	enemy.GetRigidBody()->Update();
}

void ERun::Exit(Enemy& enemy)
{
}

EnemyState* ERun::CheckTransition(Enemy* enemy)
{
	if (enemy->IsInAttackRange())
	{
		EType type = enemy->GetEnemyType();
		switch (type)
		{
		case EType::Grunt:
			return new GruntAttack();
			break;
		case EType::Pomp:
			return new PompAttack();
			break;
		case EType::Gangster:
			return new GangsterAttack();
			break;
		case EType::SheildCop:
			return new ShieldCopAttack();
			break;
		}
	}
	return nullptr;
}

void EAttack::Enter(Enemy& enemy)
{
	state = "Attack";
	enemy.ChangeAnimation(EImageType::Attack);
	attacktimer = 0.f;
}

void EAttack::Update(Enemy& enemy)
{
	enemy.GetRigidBody()->Update();
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

void GruntAttack::Enter(Enemy& enemy)
{
	EAttack::Enter(enemy);
	attackCooldown = 1.f;
	isAttacking = true;
}

void GruntAttack::Update(Enemy& enemy)
{
	enemy.GetRigidBody()->Update();
	if (enemy.GetCurrFrame() >= enemy.GetMaxAttackFrame())
	{
		isAttacking = false;
	}
}

void GruntAttack::Exit(Enemy& enemy)
{

}

EnemyState* GruntAttack::CheckTransition(Enemy* enemy)
{
	if (!isAttacking)
	{
		if (enemy->IsInAttackRange())
		{
			return new GruntAttack();
		}
		else
		{
			return new ERun();
		}
	}
	return nullptr;
}

void PompAttack::Enter(Enemy& enemy)
{
	EAttack::Enter(enemy);
	attackCooldown = 1.f;
}

void PompAttack::Update(Enemy& enemy)
{
	enemy.GetRigidBody()->Update();
}

void PompAttack::Exit(Enemy& enemy)
{
}

EnemyState* PompAttack::CheckTransition(Enemy* enemy)
{
	return nullptr;
}

void GangsterAttack::Enter(Enemy& enemy)
{
	EAttack::Enter(enemy);
	attackCooldown = 1.f;
}

void GangsterAttack::Update(Enemy& enemy)
{
	enemy.GetRigidBody()->Update();
}

void GangsterAttack::Exit(Enemy& enemy)
{
}

EnemyState* GangsterAttack::CheckTransition(Enemy* enemy)
{
	return nullptr;
}

void ShieldCopAttack::Enter(Enemy& enemy)
{
	EAttack::Enter(enemy);
	attackCooldown = 1.f;
}

void ShieldCopAttack::Update(Enemy& enemy)
{
	enemy.GetRigidBody()->Update();
}

void ShieldCopAttack::Exit(Enemy& enemy)
{
}

EnemyState* ShieldCopAttack::CheckTransition(Enemy* enemy)
{
	return nullptr;
}
