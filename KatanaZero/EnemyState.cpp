#include "EnemyState.h"
#include "Enemy.h"
#include "RigidBody.h"
#include "SnapShotManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "GPImage.h"
#include "LineManager.h"
#include "CommonFunction.h"

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

	int dir = enemy.GetDir();
	const float speed = enemy.GetSpeed();

	enemy.GetRigidBody()->AddVelocity({ dir * speed, 0.f });

	enemy.GetRigidBody()->Update();
}

void EWalk::Exit(Enemy& enemy)
{
	enemy.GetRigidBody()->SetVelocity({ 0.f, 0.f });
	enemy.SetDir(enemy.GetDir() * -1);
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
	enemy.GetRigidBody()->SetDown(false);
}

void ERun::Update(Enemy& enemy)
{
	if (SnapShotManager::GetInstance()->GetPlayer().empty()) return;

	FPOINT playerPos = SnapShotManager::GetInstance()->GetPlayer().front()->GetPos();
	FPOINT pos = enemy.GetPos();

	float dx = playerPos.x - pos.x;
	int dir = (dx > 0) ? 1 : -1;
	enemy.SetDir(dir);

	const float chaseSpeed = enemy.GetSpeed() * 2.f;
	enemy.GetRigidBody()->AddVelocity({ dir * chaseSpeed, 0.f });

	enemy.GetRigidBody()->Update();
}

void ERun::Exit(Enemy& enemy)
{
}

EnemyState* ERun::CheckTransition(Enemy* enemy)
{
	// 적 층 이동 체크
	if (!enemy->IsInSameFloor())
	{
		return new EFindSlope();
	}

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
	if (enemy.GetCurrFrame() >= enemy.GetImage()->getMaxFrame())
	{
		enemy.SetDead(true);
	}
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

void EFindSlope::Enter(Enemy& enemy)
{
	state = "FindSlope";
	auto player = SnapShotManager::GetInstance()->GetPlayer().front();
	int targetFloor = player->GetFloorIndex();
	FPOINT playerPos = player->GetPos();
	pair<FPOINT, FPOINT> slope = LineManager::GetInstance()->FindNearestSlope(playerPos, enemy.GetFloorIndex(), targetFloor);
	slopeEntry = slope.first;
	slopeExit = slope.second;
	int a = 1;
}

void EFindSlope::Update(Enemy& enemy)
{
	const float speed = enemy.GetSpeed();
	const FPOINT pos = enemy.GetPos();

	float dx = slopeEntry.x - pos.x;
	int dir = (dx > 0) ? 1 : -1;

	const float chaseSpeed = speed * 2.f;
	enemy.SetDir(dir);
	enemy.GetRigidBody()->AddVelocity({ dir * chaseSpeed, 0.f });
	enemy.GetRigidBody()->Update();
}

void EFindSlope::Exit(Enemy& enemy)
{
	int playerFloor = SnapShotManager::GetInstance()->GetPlayer().front()->GetFloorIndex();
	enemy.SetTargetFloor(playerFloor);
	enemy.SetReachedTargetFloor(false);
}

EnemyState* EFindSlope::CheckTransition(Enemy* enemy)
{
	const FPOINT pos = enemy->GetPos();
	const float dist = fabs(slopeEntry.x - pos.x);

	if (dist < 3.f)
	{
		return new ERunOnSlope(slopeEntry, slopeExit);
	}

	return nullptr;
}

ERunOnSlope::ERunOnSlope(const FPOINT& entry, const FPOINT& exit)
	: slopeEntry(entry), slopeExit(exit)
{
}

void ERunOnSlope::Enter(Enemy& enemy)
{
	state = "OnSlope";
	enemy.GetRigidBody()->SetDown(true);
	enemy.GetRigidBody()->SetGravityVisible(false);
	enemy.GetRigidBody()->SetElasticity(0.f);
}

void ERunOnSlope::Update(Enemy& enemy)
{
	FPOINT dir;
	dir.x = slopeExit.x - slopeEntry.x;
	dir.y = slopeExit.y - slopeEntry.y;
	if (dir.x > 0)
		enemy.SetDir(1);
	else if (dir.x < 0)
		enemy.SetDir(-1);
	Normalize(dir);
	float chaseSpeed = enemy.GetSpeed() * 2.f;
	enemy.GetRigidBody()->SetVelocity({ dir.x * chaseSpeed, dir.y * chaseSpeed });

	enemy.GetRigidBody()->Update();
}

void ERunOnSlope::Exit(Enemy& enemy)
{
	enemy.GetRigidBody()->SetDown(false);
	enemy.GetRigidBody()->SetGravityVisible(true);
	enemy.GetRigidBody()->SetElasticity(0.3f);
}

EnemyState* ERunOnSlope::CheckTransition(Enemy* enemy)
{
	/*FLineResult result = enemy->GetRigidBody()->GetResult();
	if (result.LineType == ELineType::Normal)
	{
		return new ERun();
	}*/
	/*auto player = SnapShotManager::GetInstance()->GetPlayer().front();
	if (player == nullptr) return nullptr;
	if (enemy->GetFloorIndex() == player->GetFloorIndex())
	{
		return new ERun();
	}*/
	float dist = fabs(slopeExit.x - enemy->GetPos().x);
	if (dist < 3.f)
	{
		return new ERun();
	}
	return nullptr;
}
