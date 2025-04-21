#include "AttackState.h"
#include "Player.h"
#include "RigidBody.h"
#include "Image.h"

PlayerState* AttackState::GetInput(Player* player)
{
	playerInfo* playerInfo = player->GetInfo();
	
	// Attack is the first priority
	if (playerInfo->bIsAttack == false)
	{
        player->SetSwitchTime(0.02f);
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
	player->SetEState(EPlayerState::Attack);
	player->GetInfo()->bIsAttack = true;
	
	if (player->GetPos().x > g_ptMouse.x) player->SetDirection(EDirection::Left);
	if (player->GetPos().x < g_ptMouse.x) player->SetDirection(EDirection::Right);

	if (player->GetDirection() == EDirection::Left)
		player->GetRigidBody()->AddVelocity({ -300.f, -200.f });
	else
		player->GetRigidBody()->AddVelocity({ 300.f, -200.f });
}

void AttackState::Update(Player* player)
{
	// move

	// collision

	// render
	

	if (player->GetFrameIndex() >= ImageManager::GetInstance()->FindImage("zeroattack")->GetMaxFrameX()-1)
		player->GetInfo()->bIsAttack = false;
}
