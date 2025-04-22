#include "FallState.h"
#include "Player.h"
#include "RigidBody.h"


PlayerState* FallState::GetInput(Player* player)
{
	if (player->GetRigidBody()->IsGround())
	{
		player->GetRigidBody()->SetDown(false);
		return player->GetStates()->Idle;
	}

	return nullptr;
}

void FallState::Enter(Player* player)
{
	player->SetImage(ImageManager::GetInstance()->FindImage("zerofall"));
	player->SetEState(EPlayerState::Fall);	
}

void FallState::Update(Player* player)
{
	//player->GetRigidBody()->AddVelocity({0.f, 10.f});
}
