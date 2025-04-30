#include "PlayerWallAction.h"
#include "Player.h"
#include "RigidBody.h"

PlayerWallAction::PlayerWallAction(Player* player)
{
	this->player = player;
}

void PlayerWallAction::onEnter()
{
	
	this->player->GetRigidBody()->SetAccelerationAlpha({ 0,300.f });
}

void PlayerWallAction::Update()
{
	

}

void PlayerWallAction::onExit()
{
	this->player->GetRigidBody()->SetAccelerationAlpha({ 0,800.f });
}
