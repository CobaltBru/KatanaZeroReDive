#include "FallState.h"
#include "Player.h"
#include "RigidBody.h"
#include "EffectManager.h"

PlayerState* FallState::GetInput(Player* player)
{
	if (player->GetRigidBody()->IsGround())
	{
		EffectManager::GetInstance()->Activefx("landcloud", player->GetPos() + FPOINT{ 0.f, player->GetHalfHeight() * 1.75f }, 0.0f, false);
		player->GetRigidBody()->SetDown(false);
		return player->GetStates()->Idle;
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON) && player->GetInfo()->bCanAttack)
		return player->GetStates()->Attack;
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

	if (!player->GetInfo()->bIsShiftChanged) return;

	if (player->GetInfo()->bIsShift)
		player->SetImage(ImageManager::GetInstance()->FindImage("zerofallshadow"));
	else
		player->SetImage(ImageManager::GetInstance()->FindImage("zerofall"));
}
