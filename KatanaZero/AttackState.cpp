#include "AttackState.h"
#include "Player.h"
#include "RigidBody.h"
#include "Image.h"
#include "CommonFunction.h"


PlayerState* AttackState::GetInput(Player* player)
{
	playerInfo* playerInfo = player->GetInfo();
	
	// Attack is the first priority
	if (playerInfo->bIsAttack == false)
	{
        player->SetSwitchTime(0.02f);
		player->SetEffectImage(nullptr);
		return player->GetStates()->Idle;
    }
	
	return nullptr;
}

void AttackState::Enter(Player* player)
{
	updateCount = 0;
	imageFrequency = 3;
	
	player->SetFrameIndex(0);
	player->SetSwitchTime(0.04f);
	
	player->SetImage(ImageManager::GetInstance()->FindImage("zeroattack"));
	player->SetEffectImage(ImageManager::GetInstance()->FindImage("normalslash"));
	player->SetEState(EPlayerState::Attack);
	player->GetInfo()->bIsAttack = true;
	
	if (player->GetPos().x > g_ptMouse.x) player->SetDirection(EDirection::Left);
	if (player->GetPos().x < g_ptMouse.x) player->SetDirection(EDirection::Right);

	// direction from player to mouse point
	FPOINT attackDir = { 0.f, 0.f };
	attackDir.x = g_ptMouse.x - player->GetPos().x;
	attackDir.y = g_ptMouse.y - player->GetPos().y;
	Normalize(attackDir);

	player->GetRigidBody()->AddVelocity(attackDir * 100.f);
}

void AttackState::Update(Player* player)
{
	// move

	// collision

	// render
	
	if (player->GetFrameIndex() >= ImageManager::GetInstance()->FindImage("zeroattack")->GetMaxFrameX()-1)
		player->GetInfo()->bIsAttack = false;
}
