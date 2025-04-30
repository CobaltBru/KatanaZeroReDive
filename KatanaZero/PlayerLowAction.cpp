#include "PlayerLowAction.h"
#include "Player.h"
#include "RigidBody.h"

PlayerLowAction::PlayerLowAction(Player* player)
{
	this->player = player;
}

void PlayerLowAction::onEnter()
{
	
}

void PlayerLowAction::Update()
{
	if (KeyManager::GetInstance()->IsOnceKeyUp('S'))
	{
		player->changeState(STATE::IDLE);
	}
}

void PlayerLowAction::onExit()
{
}
