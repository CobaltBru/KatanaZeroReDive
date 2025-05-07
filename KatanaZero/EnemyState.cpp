#include "EnemyState.h"
#include "Enemy.h"
#include "RigidBody.h"
#include "SnapShotManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "GPImage.h"
#include "LineManager.h"
#include "CommonFunction.h"
#include "EffectManager.h"
#include "ScrollManager.h"
#include "Bullet.h"

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
	if (enemy->IsHitted())
	{
		return new EDead();
	}
	return nullptr;
}

void EWalk::Enter(Enemy& enemy)
{
	walktimer = 0.f;
	urd.param(uniform_real_distribution<float>::param_type{ 1.5f, 3.f });
	walkCooldown = (float)urd(dre);
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
	if (enemy->IsHitted())
	{
		return new EDead();
	}
	return nullptr;
}

void ERun::Enter(Enemy& enemy)
{
	state = "Run";
	enemy.ChangeAnimation(EImageType::Run);
	enemy.GetRigidBody()->SetDown(false);
	enemy.GetRigidBody()->SetMaxVelocity({ enemy.GetSpeed() * 2.f, 400.f });
}

void ERun::Update(Enemy& enemy)
{
	if (!SnapShotManager::GetInstance()->GetPlayer()) return;
	if (!enemy.IsInAttackRange())
	{
		FPOINT playerPos = SnapShotManager::GetInstance()->GetPlayer()->GetPos();
		FPOINT pos = enemy.GetPos();

		float dx = playerPos.x - pos.x;
		int dir = (dx > 0) ? 1 : -1;
		enemy.SetDir(dir);

		const float chaseSpeed = enemy.GetSpeed() * 2.f;
		enemy.GetRigidBody()->AddVelocity({ dir * chaseSpeed, 0.f });
	}

	float dt = TimerManager::GetInstance()->GetDeltaTime();
	enemy.AddATimer(dt);
	if (enemy.GetATimer() >= enemy.GetADuration())
	{
		enemy.SetATimer(0.f);
		enemy.SetCanAttack(true);
	}

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

	if (enemy->IsInAttackRange() && enemy->CanAttack())
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
			//if (enemy->IsInMeleeAttackRange())
			//	return new GangsterMeleeAttack();	// melee
			//else
			//	return new GangsterAttack();		// gun
			break;
		}
	}
	if (enemy->IsHitted())
	{
		return new EDead();
	}
	return nullptr;
}

void EAttack::Enter(Enemy& enemy)
{
	state = "Attack";
	enemy.ChangeAnimation(EImageType::Attack);
	isAttackFinish = false;
	enemy.GetRigidBody()->SetVelocity({ 0.f, 0.f });
}

void EAttack::Update(Enemy& enemy)
{
	if (enemy.GetCurrFrame() >= enemy.GetImage()->getMaxFrame() - 1)
	{
		isAttackFinish = true;
	}
	enemy.Collision();
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
	FPOINT colliderSize = enemy.GetCollider()->GetSize();
	float hitAngle = enemy.GetHitAngle();
	EffectManager::GetInstance()->CreateBGBlood({ enemy.GetPos().x, enemy.GetPos().y - 10.f }, hitAngle, enemy.GetCollider()->GetSize());
	EffectManager::GetInstance()->EmitBlood(enemy.GetPos(), 40);
	EffectManager::GetInstance()->Activefx("hitslash", enemy.GetPos(), 0.f, false);
}

void EDead::Update(Enemy& enemy)
{
	if (enemy.GetCurrFrame() >= enemy.GetImage()->getMaxFrame() - 1)
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
}

void GruntAttack::Update(Enemy& enemy)
{	
	if (enemy.GetCurrFrame() >= enemy.GetImage()->getMaxFrame() - 1)
	{
		isAttackFinish = true;
	}
	enemy.Collision();
	enemy.GetRigidBody()->Update();
}

void GruntAttack::Exit(Enemy& enemy)
{
	enemy.SetCanAttack(false);
}

EnemyState* GruntAttack::CheckTransition(Enemy* enemy)
{
	if (isAttackFinish && enemy->GetCurrFrame() == enemy->GetMaxAttackFrame() - 1)
	{
		return new ERun();
	}
	if (enemy->IsHitted())
	{
		return new EDead();
	}
	return nullptr;
}

void PompAttack::Enter(Enemy& enemy)
{
	EAttack::Enter(enemy);
}

void PompAttack::Update(Enemy& enemy)
{
	if (enemy.GetCurrFrame() >= enemy.GetImage()->getMaxFrame() - 1)
	{
		isAttackFinish = true;
	}
	enemy.Collision();
	enemy.GetRigidBody()->Update();
}

void PompAttack::Exit(Enemy& enemy)
{
	enemy.SetCanAttack(false);
}

EnemyState* PompAttack::CheckTransition(Enemy* enemy)
{
	if (enemy->IsHitted())
	{
		if(enemy->GetCurrFrame() >= 3 && enemy->GetCurrFrame() <= 4)
		{
			return new PompGroggy();
		}
		else
		{
			return new EDead();
		}
	}
	if (isAttackFinish && enemy->GetCurrFrame() == enemy->GetMaxAttackFrame() - 1)
	{
		return new ERun();
	}

	return nullptr;
}

void GangsterAttack::Enter(Enemy& enemy)
{
	state = "Attack";
	enemy.ChangeAnimation(EImageType::GangsterAttack);
	isAttackFinish = false;
	isFire = false;
	enemy.GetRigidBody()->SetVelocity({ 0.f, 0.f });
}

void GangsterAttack::Update(Enemy& enemy)
{
	if (enemy.GetCurrFrame() == 1 && !isFire)
	{
		float offset = (enemy.GetDir() == 1) ? 28.f : -28.f;
		FPOINT firePoint = { enemy.GetPos().x + offset * ScrollManager::GetInstance()->GetScale(), enemy.GetPos().y - 1.f * ScrollManager::GetInstance()->GetScale() };
		bool bFlip = (enemy.GetDir() == 1) ? false : true;
		EffectManager::GetInstance()->Activefx("gangstergun", firePoint, 0.f, bFlip);
		Bullet1* bullet = new Bullet1();
		auto player = SnapShotManager::GetInstance()->GetPlayer();

		int dir = (enemy.GetDir() == 1) ? 1 : -1;
		float targetangle = atan2f(player->GetPos().y - enemy.GetPos().y, player->GetPos().x - enemy.GetPos().x);
		bullet->Init(firePoint, -targetangle, 1000.f, dir);
		isFire = true;
	}
	if (enemy.GetCurrFrame() >= enemy.GetImage()->getMaxFrame() - 1)
	{
		isAttackFinish = true;
	}
	enemy.Collision();
	enemy.GetRigidBody()->Update();
}

void GangsterAttack::Exit(Enemy& enemy)
{
	enemy.SetCanAttack(false);
}

EnemyState* GangsterAttack::CheckTransition(Enemy* enemy)
{
	if (isAttackFinish && enemy->GetCurrFrame() == enemy->GetMaxAttackFrame() - 1)
	{
		return new ERun();
	}
	if (enemy->IsHitted())
	{
		return new EDead();
	}
	return nullptr;
}

void EFindSlope::Enter(Enemy& enemy)
{
	state = "FindSlope";
	auto player = SnapShotManager::GetInstance()->GetPlayer();
	int targetFloor = player->GetFloorIndex(g_FloorZones);
	FPOINT playerPos = player->GetPos();
	pair<FPOINT, FPOINT> slope = LineManager::GetInstance()->FindNearestSlope(playerPos, enemy.GetFloorIndex(g_FloorZones), targetFloor);
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
	int playerFloor = SnapShotManager::GetInstance()->GetPlayer()->GetFloorIndex(g_FloorZones);
	enemy.SetTargetFloor(playerFloor);
	enemy.SetReachedTargetFloor(false);
}

EnemyState* EFindSlope::CheckTransition(Enemy* enemy)
{
	const FPOINT pos = enemy->GetPos();
	const float dist = fabs(slopeEntry.x - pos.x);

	if (dist < 3.f)
	{
		int playerFloor = SnapShotManager::GetInstance()->GetPlayer()->GetFloorIndex(g_FloorZones);
		return new ERunOnSlope(slopeEntry, slopeExit, playerFloor);
	}
	if (enemy->IsHitted())
	{
		return new EDead();
	}

	return nullptr;
}

ERunOnSlope::ERunOnSlope(const FPOINT& entry, const FPOINT& exit, int targetFloor)
	: slopeEntry(entry), slopeExit(exit), targetFloor(targetFloor)
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
	int currPlayerFloor = SnapShotManager::GetInstance()->GetPlayer()->GetFloorIndex(g_FloorZones);
	if (currPlayerFloor != targetFloor)
	{
		return new ERun();
	}
	float dist = fabs(slopeExit.x - enemy->GetPos().x);
	if (dist < 3.f)
	{
		return new ERun();
	}
	if (enemy->IsHitted())
	{
		return new EDead();
	}
	return nullptr;
}

void PompGroggy::Enter(Enemy& enemy)
{
	state = "Groggy";
	groggyTimer = 0.f;
	groggyDuration = 1.5f;
	bCanStand = false;
	enemy.ChangeAnimation(EImageType::Dead);
	enemy.GetRigidBody()->SetMaxVelocity({ enemy.GetSpeed() * 2.f, 400.f });
	enemy.GetRigidBody()->AddVelocity({ -enemy.GetDir() * enemy.GetSpeed() * 2.f , -200.f});
}

void PompGroggy::Update(Enemy& enemy)
{
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	groggyTimer += dt;
	if (groggyTimer >= groggyDuration)
	{
		bCanStand = true;
	}
	if (enemy.GetCurrFrame() == enemy.GetImage()->getMaxFrame() - 7)
		enemy.GetRigidBody()->SetVelocity({ 0.f, 0.f });
	enemy.GetRigidBody()->Update();
}

void PompGroggy::Exit(Enemy& enemy)
{
	enemy.GetRigidBody()->SetVelocity({ 0.f, 0.f });
}

EnemyState* PompGroggy::CheckTransition(Enemy* enemy)
{
	if (bCanStand)
		return new ERun();
	return nullptr;
}

void GangsterMeleeAttack::Enter(Enemy& enemy)
{
	EAttack::Enter(enemy);
}

void GangsterMeleeAttack::Update(Enemy& enemy)
{
	if (enemy.GetCurrFrame() >= enemy.GetImage()->getMaxFrame() - 1)
	{
		isAttackFinish = true;
	}
	enemy.GetRigidBody()->Update();
}

void GangsterMeleeAttack::Exit(Enemy& enemy)
{
}

EnemyState* GangsterMeleeAttack::CheckTransition(Enemy* enemy)
{
	if (isAttackFinish && enemy->GetCurrFrame() == enemy->GetMaxAttackFrame() - 1)
	{
		return new ERun();
	}
	return nullptr;
}
